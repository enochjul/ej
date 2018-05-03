//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_CONSTRUCT_H
#define EJ_CONSTRUCT_H

#include <stddef.h>
#include <string.h>

#include <type_traits>
#include <utility>

#include "CallType.h"
#include "fill.h"
#include "TypeTraits.h"

//Define a potentially throwing version of placement new so that the compiler does not emit a null pointer check
inline void *operator new(size_t, void *ptr, int) { return ptr; }

namespace ej {

//! Helper class to call the default constructor on allocated memory
template <class T, bool always, bool trivial = std::is_trivially_constructible<T>::value>
class DefaultConstruct;

template <class T, bool trivial>
class DefaultConstruct<T, true, trivial> {
public:
	static void run(T *ptr) noexcept(std::is_nothrow_default_constructible<T>::value) {
		new(ptr, 0) T();
	}

	static void run_array(T *first, T *last) noexcept(std::is_nothrow_default_constructible<T>::value) {
		if constexpr (is_zero_default_constructible<T>::value) {
			memset(first, 0, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
		} else {
			for (auto i = first, e = last; i != e; ++i) {
				new(i, 0) T();
			}
		}
	}

	static void run_array_n(T *first, size_t n) noexcept(std::is_nothrow_default_constructible<T>::value) {
		if constexpr (is_zero_default_constructible<T>::value) {
			memset(first, 0, n * sizeof(T));
		} else {
			for (auto i = first, e = first + n; i != e; ++i) {
				new(i, 0) T();
			}
		}
	}
};

template <typename T>
class DefaultConstruct<T, true, true> {
public:
	static void run(T *ptr) noexcept {
		new(ptr, 0) T();
	}

	static void run_array(T *first, T *last) noexcept {
		memset(first, 0, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	}

	static void run_array_n(T *first, size_t n) noexcept {
		memset(first, 0, n * sizeof(T));
	}
};

template <class T>
class DefaultConstruct<T, false, true> {
public:
	static void run(T *) noexcept {
	}

	static void run_array(T *, T *) noexcept {
	}

	static void run_array_n(T *, size_t) noexcept {
	}
};

template <typename T, bool always = true>
inline void default_construct(T *ptr) noexcept(std::is_nothrow_default_constructible<T>::value) {
	DefaultConstruct<T, always>::run(ptr);
}

template <typename T, bool always = true>
inline void default_construct_array(T *first, T *last) noexcept(std::is_nothrow_default_constructible<T>::value) {
	DefaultConstruct<T, always>::run_array(first, last);
}

template <typename T, bool always = true>
inline void default_construct_array_n(T *first, size_t n) noexcept(std::is_nothrow_default_constructible<T>::value) {
	DefaultConstruct<T, always>::run_array_n(first, n);
}

template <typename T>
inline void copy_construct(T *ptr, typename CallType<T>::param_type a) noexcept(std::is_nothrow_copy_constructible<T>::value) {
	new(ptr, 0) T(a);
}

template <typename T>
inline void copy_construct_array(T *dst, const T *first, const T *last) noexcept(std::is_nothrow_copy_constructible<T>::value) {
	if constexpr (std::is_trivially_copy_constructible<T>::value) {
		memcpy(dst, first, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		for (auto i = first, e = last; i != e; ++i, ++dst) {
			new(dst, 0) T(*i);
		}
	}
}

template <typename T>
inline void copy_construct_array_n(T *dst, const T *first, size_t n) noexcept(std::is_nothrow_copy_constructible<T>::value) {
	if constexpr (std::is_trivially_copy_constructible<T>::value) {
		memcpy(dst, first, n * sizeof(T));
	} else {
		for (auto i = first, e = first + n; i != e; ++i, ++dst) {
			new(dst, 0) T(*i);
		}
	}
}

template <typename T>
inline void copy_construct_fill_array(T *first, T *last, typename CallType<T>::param_type a) noexcept(std::is_nothrow_copy_constructible<T>::value) {
	if constexpr (std::is_scalar<T>::value) {
		fill(first, last, a);
	} else {
		for (auto i = first, e = last; i != e; ++i) {
			new(i, 0) T(a);
		}
	}
}

template <typename T>
inline void copy_construct_fill_array_n(T *first, size_t n, typename CallType<T>::param_type a) noexcept(std::is_nothrow_copy_constructible<T>::value) {
	if constexpr (std::is_scalar<T>::value) {
		fill_n(first, n, a);
	} else {
		for (auto i = first, e = first + n; i != e; ++i) {
			new(i, 0) T(a);
		}
	}
}

template <typename T>
inline void move_construct(T *ptr, T &&a) noexcept(std::is_nothrow_move_constructible<T>::value) {
	new(ptr, 0) T(std::move(a));
}

template <typename T>
inline void move_construct_array(T *dst, T *first, T *last) noexcept(std::is_nothrow_move_constructible<T>::value) {
	if constexpr (std::is_trivially_move_constructible<T>::value) {
		memcpy(dst, first, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		for (auto i = first, e = last; i != e; ++i, ++dst) {
			new(dst, 0) T(std::move(*i));
		}
	}
}

template <typename T>
inline void move_construct_array_n(T *dst, T *first, size_t n) noexcept(std::is_nothrow_move_constructible<T>::value) {
	if constexpr (std::is_trivially_move_constructible<T>::value) {
		memcpy(dst, first, n * sizeof(T));
	} else {
		for (auto i = first, e = first + n; i != e; ++i, ++dst) {
			new(dst, 0) T(std::move(*i));
		}
	}
}

template <typename T, typename ... Args>
inline void forward_construct(T *ptr, Args && ... a) noexcept(std::is_nothrow_constructible<T, Args ...>::value) {
	new(ptr, 0) T(std::forward<Args>(a) ...);
}

template <class T, typename ... Args>
inline void forward_construct_array(T *first, T *last, Args && ... a) noexcept(std::is_nothrow_constructible<T, Args ...>::value) {
	for (auto i = first, e = last; i != e; ++i) {
		new(i, 0) T(std::forward<Args>(a) ...);
	}
}

template <class T, typename ... Args>
inline void forward_construct_array_n(T *first, size_t n, Args && ... a) noexcept(std::is_nothrow_constructible<T, Args ...>::value) {
	for (auto i = first, e = first + n; i != e; ++i) {
		new(i, 0) T(std::forward<Args>(a) ...);
	}
}

}

#endif
