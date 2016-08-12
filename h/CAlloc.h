//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_C_ALLOC_H
#define EJ_C_ALLOC_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

namespace ej {

//! Helper functions to access the C memory allocation functions such as malloc and realloc
class CAlloc {
	template <typename T>
	static T *allocate(size_t n) noexcept {
		if (alignof(T) <= alignof(max_align_t)) {
			return static_cast<T *>(malloc(n));
		} else {
			assert(n % alignof(T) == 0);
			return static_cast<T *>(aligned_alloc(alignof(T), n));
		}
	}

	template <typename T>
	static T *reallocate(T *ptr, size_t n) noexcept {
		static_assert(alignof(T) <= alignof(max_align_t));
		return static_cast<T *>(::realloc(ptr, n));
	}

public:
	//! Allocates memory for an object of the specified type
	template <typename T, bool may_fail = false>
	static T *alloc() noexcept {
		auto ptr = allocate<T>(sizeof(T));
		if (may_fail || ptr != nullptr) {
			return ptr;
		}
		abort();
	}

	//! Allocates memory for an object of the specified type, and returns nullptr if it fails
	template <typename T>
	static T *try_alloc() noexcept {
		return alloc<T, true>();
	}

	//! Allocates memory for an array of objects of the specified type
	template <typename T, size_t sentinel_n = 0, bool may_fail = false>
	static T *alloc_array(size_t n) noexcept {
		static_assert(sentinel_n <= SIZE_MAX / sizeof(T));

		if ((sizeof(T) <= 1 && sentinel_n == 0) || n <= ((SIZE_MAX / sizeof(T)) - sentinel_n)) {
			auto ptr = allocate<T>(n * sizeof(T) + sentinel_n * sizeof(T));
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Allocates memory for an array of object of the specified type, and returns nullptr if it fails
	template <typename T, size_t sentinel_n = 0>
	static T *try_alloc_array(size_t n) noexcept {
		return alloc_array<T, sentinel_n, true>(n);
	}

	//! Reallocates memory for an array of objects of the specified type
	template <typename T, size_t sentinel_n = 0, bool may_fail = false>
	static T *realloc_array(T *ptr, size_t n) noexcept {
		static_assert(sentinel_n <= SIZE_MAX / sizeof(T));

		if ((sizeof(T) <= 1 && sentinel_n == 0) || n <= ((SIZE_MAX / sizeof(T)) - sentinel_n)) {
			ptr = reallocate<T>(ptr, n * sizeof(T) + sentinel_n * sizeof(T));
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Reallocates memory for an array of objects of the specified type, and returns nullptr if it fails
	template <typename T, size_t sentinel_n = 0>
	static T *try_realloc_array(T *ptr, size_t n) noexcept {
		return realloc_array<T, sentinel_n, true>(ptr, n);
	}

	//! Allocates memory for an array of objects of the specified type, where the size is determined from a range [first, last)
	template <typename T, size_t sentinel_n = 0, bool may_fail = false>
	static T *alloc_array_range(const T *first, const T *last) noexcept {
		static_assert(sentinel_n <= SIZE_MAX / sizeof(T));

		size_t n = (last - first) * sizeof(T);
		if (sentinel_n == 0 || n <= SIZE_MAX - sentinel_n * sizeof(T)) {
			auto ptr = allocate<T>(n + sentinel_n * sizeof(T));
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Allocates memory for an array of objects of the specified type, where the size is determined from a range [first, last), and returns nullptr if it fails
	template <typename T, size_t sentinel_n = 0>
	static T *try_alloc_array_range(const T *first, const T *last) noexcept {
		return alloc_array_range<T, sentinel_n, true>(first, last);
	}

	//! Reallocates memory for an array of objects of the specified type, where the size is determined from a range [first, last)
	template <typename T, size_t sentinel_n = 0, bool may_fail = false>
	static T *realloc_array_range(T *ptr, const T *first, const T *last) noexcept {
		static_assert(sentinel_n <= SIZE_MAX / sizeof(T));

		size_t n = (last - first) * sizeof(T);
		if (sentinel_n == 0 || n <= SIZE_MAX - sentinel_n * sizeof(T)) {
			auto ptr = reallocate<T>(n + sentinel_n * sizeof(T));
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Reallocates memory for an array of objects of the specified type, where the size is determined from a range [first, last), and returns nullptr if it fails
	template <typename T, size_t sentinel_n = 0>
	static T *try_realloc_array_range(const T *first, const T *last) noexcept {
		return realloc_array_range<T, sentinel_n, true>(first, last);
	}

	//! Allocates memory for a flexible array of the specified type
	template <typename T, size_t sentinel_n = 0, bool may_fail = false>
	static T *alloc_flexible_array(size_t n) noexcept {
		static_assert(sentinel_n <= (SIZE_MAX - T::sizeofHeader()) / sizeof(typename T::value_type));

		if (n <= ((SIZE_MAX - T::sizeofHeader()) / sizeof(typename T::value_type) - sentinel_n)) {
			auto ptr = allocate<T>(n * sizeof(typename T::value_type) + sentinel_n * sizeof(typename T::value_type) + T::sizeofHeader());
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Allocates memory for a flexible array of the specified type, and returns nullptr if it fails
	template <typename T, size_t sentinel_n = 0>
	static T *try_alloc_flexible_array(size_t n) noexcept {
		return alloc_flexible_array<T, sentinel_n, true>(n);
	}

	//! Reallocates memory for a flexible array of the specified type
	template <typename T, size_t sentinel_n = 0, bool may_fail = false>
	static T *realloc_flexible_array(T *ptr, size_t n) noexcept {
		static_assert(sentinel_n <= (SIZE_MAX - T::sizeofHeader()) / sizeof(typename T::value_type));

		if (n <= ((SIZE_MAX - T::sizeofHeader()) / sizeof(typename T::value_type) - sentinel_n)) {
			ptr = reallocate<T>(ptr, n * sizeof(typename T::value_type) + sentinel_n * sizeof(typename T::value_type) + T::sizeofHeader());
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Reallocates memory for a flexible array of the specified type, and returns nullptr if it fails
	template <typename T, size_t sentinel_n = 0>
	static T *try_realloc_flexible_array(T *ptr, size_t n) noexcept {
		return realloc_flexible_array<T, sentinel_n, true>(ptr, n);
	}

	//! Allocates memory for a 2 dimensional array of objects of the specified type
	template <typename T, bool may_fail = false>
	static T *alloc_2d_array(size_t width, size_t height) noexcept {
		size_t n;

		if (!__builtin_mul_overflow(width, height, &n) && (sizeof(T) <= 1 || n <= (SIZE_MAX / sizeof(T)))) {
			auto ptr = allocate<T>(height * width * sizeof(T));
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Allocates memory for a 2 dimensional array of objects of the specified type, and returns nullptr if it fails
	template <typename T>
	static T *try_alloc_2d_array(size_t width, size_t height) noexcept {
		return alloc_2d_array<T, true>(width, height);
	}

	//! Reallocates memory for a 2 dimensional array of objects of the specified type
	template <typename T, bool may_fail = false>
	static T *realloc_2d_array(T *ptr, size_t width, size_t height) noexcept {
		size_t n;

		if (!__builtin_mul_overflow(width, height, &n) && (sizeof(T) <= 1 || n <= (SIZE_MAX / sizeof(T)))) {
			ptr = reallocate<T>(ptr, height * width * sizeof(T));
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Reallocates memory for a 2 dimensional array of objects of the specified type, and returns nullptr if it fails
	template <typename T>
	static T *try_realloc_2d_array(T *ptr, size_t width, size_t height) noexcept {
		return realloc_2d_array<T, true>(ptr, width, height);
	}

	//! Allocates memory for a 3 dimensional array of objects of the specified type
	template <typename T, bool may_fail = false>
	static T *alloc_3d_array(size_t width, size_t height, size_t depth) noexcept {
		size_t m, n;

		if (!__builtin_mul_overflow(width, height, &m) && !__builtin_mul_overflow(m, depth, &n) && (sizeof(T) <= 1 || n <= (SIZE_MAX / sizeof(T)))) {
			auto ptr = allocate<T>(depth * height * width * sizeof(T));
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Allocates memory for a 3 dimensional array of objects of the specified type, and returns nullptr if it fails
	template <typename T>
	static T *try_alloc_3d_array(size_t width, size_t height, size_t depth) noexcept {
		return alloc_3d_array<T, true>(width, height, depth);
	}

	//! Reallocates memory for a 3 dimensional array of objects of the specified type
	template <typename T, bool may_fail = false>
	static T *realloc_3d_array(T *ptr, size_t width, size_t height, size_t depth) noexcept {
		size_t m, n;

		if (!__builtin_mul_overflow(width, height, &m) && !__builtin_mul_overflow(m, depth, &n) && (sizeof(T) <= 1 || n <= (SIZE_MAX / sizeof(T)))) {
			ptr = reallocate<T>(ptr, depth * height * width * sizeof(T));
			if (may_fail || ptr != nullptr) {
				return ptr;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Reallocates memory for a 3 dimensional array of objects of the specified type, and returns nullptr if it fails
	template <typename T>
	static T *try_realloc_3d_array(T *ptr, size_t width, size_t height, size_t depth) noexcept {
		return realloc_3d_array<T, true>(ptr, width, height, depth);
	}

	//! Deallocates memory for a single object
	template <typename T>
	static void dealloc(T *ptr) noexcept {
		free(ptr);
	}

	//! Deallocates memory for a single object with an explicit size
	template <typename T>
	static void dealloc_n(T *ptr, size_t) noexcept {
		free(ptr);
	}

	//! Deallocates memory for an array of objects
	template <typename T, size_t sentinel_n = 0>
	static void dealloc_array(T *ptr, size_t) noexcept {
		free(ptr);
	}

	//! Deallocates memory for a flexible array
	template <typename T, size_t sentinel_n = 0>
	static void dealloc_flexible_array(T *ptr, size_t) noexcept {
		free(ptr);
	}

	//! Deallocates memory for a 2d array
	template <typename T>
	static void dealloc_2d_array(T *ptr, size_t, size_t) noexcept {
		free(ptr);
	}

	//! Deallocates memory for a 3d array
	template <typename T>
	static void dealloc_3d_array(T *ptr, size_t, size_t, size_t) noexcept {
		free(ptr);
	}
};

}

#endif
