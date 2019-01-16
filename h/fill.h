//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <emmintrin.h>

#include "CallType.h"
#include "Common.h"
#include "TypeTraits.h"

namespace ej {

void fill_int16(int16_t *first, int16_t *last, int16_t value) noexcept;
void fill_uint16(uint16_t *first, uint16_t *last, uint16_t value) noexcept;
void fill_int32(int32_t *first, int32_t *last, int32_t value) noexcept;
void fill_uint32(uint32_t *first, uint32_t *last, uint32_t value) noexcept;
void fill_int64(int64_t *first, int64_t *last, int64_t value) noexcept;
void fill_uint64(uint64_t *first, uint64_t *last, uint64_t value) noexcept;
void fill_float(float *first, float *last, float value) noexcept;
void fill_double(double *first, double *last, double value) noexcept;

EJ_ALWAYS_INLINE constexpr bool same_byte_value(uint16_t value) noexcept {
	return ((value >> 8) & 0xFF) == (value & 0xFF);
}

EJ_ALWAYS_INLINE constexpr bool same_byte_value(int16_t value) noexcept {
	return same_byte_value(static_cast<uint16_t>(value));
}

EJ_ALWAYS_INLINE constexpr bool same_byte_value(uint32_t value) noexcept {
	return (((value >> 24) & 0xFF) == (value & 0xFF)) && (((value >> 16) & 0xFF) == (value & 0xFF)) && (((value >> 8) & 0xFF) == (value & 0xFF));
}

EJ_ALWAYS_INLINE constexpr bool same_byte_value(int32_t value) noexcept {
	return same_byte_value(static_cast<uint32_t>(value));
}

EJ_ALWAYS_INLINE constexpr bool same_byte_value(uint64_t value) noexcept {
	return (((value >> 56) & 0xFF) == (value & 0xFF)) &&
	(((value >> 48) & 0xFF) == (value & 0xFF)) &&
	(((value >> 40) & 0xFF) == (value & 0xFF)) &&
	(((value >> 32) & 0xFF) == (value & 0xFF)) &&
	(((value >> 24) & 0xFF) == (value & 0xFF)) &&
	(((value >> 16) & 0xFF) == (value & 0xFF)) &&
	(((value >> 8) & 0xFF) == (value & 0xFF));
}

EJ_ALWAYS_INLINE constexpr bool same_byte_value(int64_t value) noexcept {
	return same_byte_value(static_cast<uint64_t>(value));
}

EJ_ALWAYS_INLINE constexpr bool same_byte_value(float value) noexcept {
	return value == 0;
}

EJ_ALWAYS_INLINE constexpr bool same_byte_value(double value) noexcept {
	return value == 0;
}

template <typename T>
inline void fill(T *first, T *last, typename CallType<T>::param_type value) noexcept(std::is_nothrow_copy_assignable<T>::value) {
	assert(&value < first || &value >= last);
	for (auto i = first; i != last; ++i) {
		*i = value;
	}
}

template <typename T>
EJ_ALWAYS_INLINE typename std::enable_if<is_any_char<T>::value, void>::type fill(T *first, T *last, typename CallType<T>::param_type value) noexcept {
	memset(first, value, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
}

EJ_ALWAYS_INLINE void fill(int16_t *first, int16_t *last, int16_t value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(value)) {
		memset(first, value, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_int16(first, last, value);
	}
}

EJ_ALWAYS_INLINE void fill(uint16_t *first, uint16_t *last, uint16_t value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(value)) {
		memset(first, value, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_uint16(first, last, value);
	}
}

EJ_ALWAYS_INLINE void fill(int32_t *first, int32_t *last, int32_t value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(value)) {
		memset(first, value, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_int32(first, last, value);
	}
}

EJ_ALWAYS_INLINE void fill(uint32_t *first, uint32_t *last, uint32_t value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(value)) {
		memset(first, value, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_uint32(first, last, value);
	}
}

EJ_ALWAYS_INLINE void fill(int64_t *first, int64_t *last, int64_t value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(value)) {
		memset(first, static_cast<int>(value), reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_int64(first, last, value);
	}
}

EJ_ALWAYS_INLINE void fill(uint64_t *first, uint64_t *last, uint64_t value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(value)) {
		memset(first, static_cast<int>(value), reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_uint64(first, last, value);
	}
}

EJ_ALWAYS_INLINE void fill(float *first, float *last, float value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(value)) {
		memset(first, static_cast<int>(value), reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_float(first, last, value);
	}
}

EJ_ALWAYS_INLINE void fill(double *first, double *last, double value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(value)) {
		memset(first, static_cast<int>(value), reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_double(first, last, value);
	}
}

template <typename T>
EJ_ALWAYS_INLINE typename std::enable_if<sizeof(T *) == sizeof(int32_t), void>::type fill(T **first, T **last, T *value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(reinterpret_cast<uintptr_t>(value))) {
		memset(first, reinterpret_cast<uintptr_t>(value), reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_int32(reinterpret_cast<int32_t *>(first), reinterpret_cast<int32_t *>(last), reinterpret_cast<int32_t>(value));
	}
}

template <typename T>
EJ_ALWAYS_INLINE typename std::enable_if<sizeof(T *) == sizeof(int64_t), void>::type fill(T **first, T **last, T *value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(reinterpret_cast<uintptr_t>(value))) {
		memset(first, reinterpret_cast<uintptr_t>(value), reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill_int64(reinterpret_cast<int64_t *>(first), reinterpret_cast<int64_t *>(last), reinterpret_cast<int64_t>(value));
	}
}

template <typename T>
EJ_ALWAYS_INLINE void fill(T **first, T **last, std::nullptr_t) noexcept {
	memset(first, 0, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
}

template <typename T>
EJ_ALWAYS_INLINE typename std::enable_if<std::is_enum<T>::value, void>::type fill(T *first, T *last, T value) noexcept {
	if (__builtin_constant_p(value) && same_byte_value(static_cast<typename std::underlying_type<T>::type>(value))) {
		memset(first, static_cast<typename std::underlying_type<T>::type>(value), reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
	} else {
		fill(reinterpret_cast<typename std::underlying_type<T>::type *>(first), reinterpret_cast<typename std::underlying_type<T>::type *>(last), static_cast<typename std::underlying_type<T>::type>(value));
	}
}

template <typename T>
inline typename std::enable_if<!std::is_scalar<T>::value, void>::type fill_n(T *first, size_t n, typename CallType<T>::param_type value) noexcept(std::is_nothrow_copy_assignable<T>::value) {
	assert(&value < first || &value >= first + n);
	for (auto i = first, last = first + n; i != last; ++i) {
		*i = value;
	}
}

template <size_t N>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned(int16_t *first, __m128i v) noexcept {
	_mm_storeu_si128(reinterpret_cast<__m128i *>(first), v);
	fill_fixed_n_vec128_unaligned<N - 1>(first + 8, v);
}

template <>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned<0>(int16_t *first, __m128i v) noexcept {
}

template <size_t N>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned(uint16_t *first, __m128i v) noexcept {
	_mm_storeu_si128(reinterpret_cast<__m128i *>(first), v);
	fill_fixed_n_vec128_unaligned<N - 1>(first + 8, v);
}

template <>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned<0>(uint16_t *first, __m128i v) noexcept {
}

template <size_t N>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned(int32_t *first, __m128i v) noexcept {
	_mm_storeu_si128(reinterpret_cast<__m128i *>(first), v);
	fill_fixed_n_vec128_unaligned<N - 1>(first + 4, v);
}

template <>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned<0>(int32_t *first, __m128i v) noexcept {
}

template <size_t N>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned(uint32_t *first, __m128i v) noexcept {
	_mm_storeu_si128(reinterpret_cast<__m128i *>(first), v);
	fill_fixed_n_vec128_unaligned<N - 1>(first + 4, v);
}

template <>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned<0>(uint32_t *first, __m128i v) noexcept {
}

template <size_t N>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned(int64_t *first, __m128i v) noexcept {
	_mm_storeu_si128(reinterpret_cast<__m128i *>(first), v);
	fill_fixed_n_vec128_unaligned<N - 1>(first + 2, v);
}

template <>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned<0>(int64_t *first, __m128i v) noexcept {
}

template <size_t N>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned(uint64_t *first, __m128i v) noexcept {
	_mm_storeu_si128(reinterpret_cast<__m128i *>(first), v);
	fill_fixed_n_vec128_unaligned<N - 1>(first + 2, v);
}

template <>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned<0>(uint64_t *first, __m128i v) noexcept {
}

template <size_t N>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned(float *first, __m128 v) noexcept {
	_mm_storeu_ps(first, v);
	fill_fixed_n_vec128_unaligned<N - 1>(first + 4, v);
}

template <>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned<0>(float *first, __m128 v) noexcept {
}

template <size_t N>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned(double *first, __m128d v) noexcept {
	_mm_storeu_pd(first, v);
	fill_fixed_n_vec128_unaligned<N - 1>(first + 2, v);
}

template <>
EJ_ALWAYS_INLINE void fill_fixed_n_vec128_unaligned<0>(double *first, __m128d v) noexcept {
}

template <typename T, size_t N>
struct FillFixedN {
	EJ_ALWAYS_INLINE static void run(T *first, T v) noexcept {
		*first = v;
		FillFixedN<T, N - 1>::run(first + 1, v);
	}
};

template <typename T>
struct FillFixedN<T, 0> {
	EJ_ALWAYS_INLINE static void run(T *first, T v) noexcept {
	}
};

EJ_ALWAYS_INLINE __m128i vec128_set1(int16_t value) noexcept {
	return _mm_set1_epi16(value);
}

EJ_ALWAYS_INLINE __m128i vec128_set1(uint16_t value) noexcept {
	return _mm_set1_epi16(value);
}

EJ_ALWAYS_INLINE __m128i vec128_set1(int32_t value) noexcept {
	return _mm_set1_epi32(value);
}

EJ_ALWAYS_INLINE __m128i vec128_set1(uint32_t value) noexcept {
	return _mm_set1_epi32(value);
}

EJ_ALWAYS_INLINE __m128i vec128_set1(int64_t value) noexcept {
	return _mm_set1_epi64x(value);
}

EJ_ALWAYS_INLINE __m128i vec128_set1(uint64_t value) noexcept {
	return _mm_set1_epi64x(value);
}

EJ_ALWAYS_INLINE __m128 vec128_set1(float value) noexcept {
	return _mm_set1_ps(value);
}

EJ_ALWAYS_INLINE __m128d vec128_set1(double value) noexcept {
	return _mm_set1_pd(value);
}

template <typename T>
EJ_ALWAYS_INLINE void vec128_storel(T *first, __m128i value) noexcept {
	_mm_storel_epi64(reinterpret_cast<__m128i *>(first), value);
}

EJ_ALWAYS_INLINE void vec128_storel(float *first, __m128 value) noexcept {
	_mm_storel_pi(reinterpret_cast<__m64 *>(first), value);
}

EJ_ALWAYS_INLINE void vec128_storel(double *first, __m128d value) noexcept {
	_mm_store_sd(first, value);
}

template <typename T, typename ScalarType = T>
EJ_ALWAYS_INLINE typename std::enable_if<is_any_char<ScalarType>::value, void>::type fill_n(T *first, size_t n, T value) noexcept {
	memset(first, value, n * sizeof(T));
}

template <typename T, typename ScalarType = T>
EJ_ALWAYS_INLINE typename std::enable_if<std::is_same<ScalarType, int16_t>::value || std::is_same<ScalarType, uint16_t>::value, void>::type fill_n(T *first, size_t n, T value) noexcept {
	static_assert(alignof(T) == 2);
	if (__builtin_constant_p(value) && same_byte_value((ScalarType) value)) {
		memset(first, (ScalarType) value, n * sizeof(T));
	} else if (__builtin_constant_p(n) && n < 128) {
		if (n >= 8) {
			auto vec128_value = vec128_set1((ScalarType) value);
			if ((n & 64) != 0) {
				fill_fixed_n_vec128_unaligned<8>(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 64;
			}
			if ((n & 32) != 0) {
				fill_fixed_n_vec128_unaligned<4>(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 32;
			}
			if ((n & 16) != 0) {
				fill_fixed_n_vec128_unaligned<2>(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 16;
			}
			if ((n & 8) != 0) {
				fill_fixed_n_vec128_unaligned<1>(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 8;
			}
			if ((n & 4) != 0) {
				vec128_storel(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 4;
			}
		} else {
			if ((n & 4) != 0) {
				FillFixedN<T, 4>::run(first, value);
				first += 4;
			}
		}
		if ((n & 2) != 0) {
			FillFixedN<T, 2>::run(first, value);
			first += 2;
		}
		if ((n & 1) != 0) {
			FillFixedN<T, 1>::run(first, value);
			first += 1;
		}
	} else {
		fill(first, first + n, value);
	}
}

template <typename T, typename ScalarType = T>
EJ_ALWAYS_INLINE typename std::enable_if<std::is_same<ScalarType, int32_t>::value || std::is_same<ScalarType, uint32_t>::value || std::is_same<ScalarType, float>::value, void>::type fill_n(T *first, size_t n, T value) noexcept {
	static_assert(alignof(T) == 4);
	if (__builtin_constant_p(value) && same_byte_value((ScalarType) value)) {
		memset(first, (ScalarType) value, n * sizeof(T));
	} else if (__builtin_constant_p(n) && n < 64) {
		if (n >= 4) {
			auto vec128_value = vec128_set1((ScalarType) value);
			if ((n & 32) != 0) {
				fill_fixed_n_vec128_unaligned<8>(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 32;
			}
			if ((n & 16) != 0) {
				fill_fixed_n_vec128_unaligned<4>(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 16;
			}
			if ((n & 8) != 0) {
				fill_fixed_n_vec128_unaligned<2>(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 8;
			}
			if ((n & 4) != 0) {
				fill_fixed_n_vec128_unaligned<1>(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 4;
			}
			if ((n & 2) != 0) {
				vec128_storel(reinterpret_cast<ScalarType *>(first), vec128_value);
				first += 2;
			}
		} else {
			if ((n & 2) != 0) {
				FillFixedN<T, 2>::run(first, value);
				first += 2;
			}
		}
		if ((n & 1) != 0) {
			FillFixedN<T, 1>::run(first, value);
			first += 1;
		}
	} else {
		fill(first, first + n, value);
	}
}

template <typename T, typename ScalarType = T>
EJ_ALWAYS_INLINE typename std::enable_if<std::is_same<ScalarType, int64_t>::value || std::is_same<ScalarType, uint64_t>::value || std::is_same<ScalarType, double>::value, void>::type fill_n(T *first, size_t n, T value) noexcept {
	static_assert(alignof(T) == 8);
	if (__builtin_constant_p(value) && same_byte_value((ScalarType) value)) {
		memset(first, (ScalarType) value, n * sizeof(T));
	} else if (__builtin_constant_p(n) && n < 32) {
		if (n >= 6) {
			auto value_x2 = vec128_set1((ScalarType) value);
			if ((n & 16) != 0) {
				fill_fixed_n_vec128_unaligned<8>(reinterpret_cast<ScalarType *>(first), value_x2);
				first += 16;
			}
			if ((n & 8) != 0) {
				fill_fixed_n_vec128_unaligned<4>(reinterpret_cast<ScalarType *>(first), value_x2);
				first += 8;
			}
			if ((n & 4) != 0) {
				fill_fixed_n_vec128_unaligned<2>(reinterpret_cast<ScalarType *>(first), value_x2);
				first += 4;
			}
			if ((n & 2) != 0) {
				fill_fixed_n_vec128_unaligned<1>(reinterpret_cast<ScalarType *>(first), value_x2);
				first += 2;
			}
			if ((n & 1) != 0) {
				vec128_storel(reinterpret_cast<ScalarType *>(first), value_x2);
				first += 1;
			}
		} else {
			if ((n & 4) != 0) {
				FillFixedN<T, 4>::run(first, value);
				first += 4;
			}
			if ((n & 2) != 0) {
				FillFixedN<T, 2>::run(first, value);
				first += 2;
			}
			if ((n & 1) != 0) {
				FillFixedN<T, 1>::run(first, value);
				first += 1;
			}
		}
	} else {
		fill(first, first + n, value);
	}
}

template <typename T>
EJ_ALWAYS_INLINE typename std::enable_if<sizeof(T *) == sizeof(int32_t), void>::type fill_n(T **first, size_t n, T *value) noexcept {
	fill_n<T *, int32_t>(first, n, value);
}

template <typename T>
EJ_ALWAYS_INLINE typename std::enable_if<sizeof(T *) == sizeof(int64_t), void>::type fill_n(T **first, size_t n, T *value) noexcept {
	fill_n<T *, int64_t>(first, n, value);
}

template <typename T>
EJ_ALWAYS_INLINE void fill_n(T **first, size_t n, std::nullptr_t) noexcept {
	memset(first, 0, n * sizeof(T *));
}

template <typename T>
EJ_ALWAYS_INLINE typename std::enable_if<std::is_enum<T>::value, void>::type fill_n(T *first, size_t n, T value) noexcept {
	fill_n<T, typename std::underlying_type<T>::type>(first, n, value);
}

template <typename T, size_t N>
EJ_ALWAYS_INLINE void fill(T (&data)[N], typename CallType<T>::param_type value) noexcept {
	fill_n(data, N, value);
}

template <typename T>
EJ_ALWAYS_INLINE void fill_zero(T *first, T *last) noexcept {
	memset(first, 0, reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first));
}

template <typename T>
EJ_ALWAYS_INLINE void fill_zero_n(T *first, size_t n) noexcept {
	memset(first, 0, n * sizeof(T));
}

template <typename T, size_t N>
EJ_ALWAYS_INLINE void fill_zero(T (&data)[N]) noexcept {
	memset(data, 0, N * sizeof(T));
}

}
