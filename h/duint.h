//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_DUINT_H
#define EJ_DUINT_H

#include <assert.h>
#include <limits.h>
#include <stdint.h>

#include "Common.h"

namespace ej {

typedef uint64_t duint32;

union duint32_u {
	duint32 Value;
	struct {
		uint32_t Low;
		uint32_t High;
	};
};

EJ_ALWAYS_INLINE duint32 make_duint32(uint32_t low, uint32_t high) noexcept {
	duint32_u u;
	u.Low = low;
	u.High = high;
	return u.Value;
}

EJ_ALWAYS_INLINE uint32_t duint32_get_low(duint32 value) noexcept {
	duint32_u u;
	u.Value = value;
	return u.Low;
}

EJ_ALWAYS_INLINE uint32_t duint32_get_high(duint32 value) noexcept {
	duint32_u u;
	u.Value = value;
	return u.High;
}

EJ_ALWAYS_INLINE duint32 duint32_add(uint32_t a, uint32_t b) noexcept {
	return static_cast<duint32>(a) + b;
}

EJ_ALWAYS_INLINE duint32 duint32_add(int32_t a, uint32_t b) noexcept {
	return static_cast<duint32>(static_cast<int64_t>(a) + b);
}

EJ_ALWAYS_INLINE duint32 duint32_add(uint32_t a, int32_t b) noexcept {
	return static_cast<duint32>(a + static_cast<int64_t>(b));
}

EJ_ALWAYS_INLINE duint32 duint32_add(int32_t a, int32_t b) noexcept {
	return static_cast<duint32>(static_cast<int64_t>(a) + b);
}

EJ_ALWAYS_INLINE duint32 duint32_sub(uint32_t a, uint32_t b) noexcept {
	return static_cast<duint32>(a) - b;
}

EJ_ALWAYS_INLINE duint32 duint32_sub(int32_t a, uint32_t b) noexcept {
	return static_cast<duint32>(static_cast<int64_t>(a) - b);
}

EJ_ALWAYS_INLINE duint32 duint32_sub(uint32_t a, int32_t b) noexcept {
	return static_cast<duint32>(a - static_cast<int64_t>(b));
}

EJ_ALWAYS_INLINE duint32 duint32_sub(int32_t a, int32_t b) noexcept {
	return static_cast<duint32>(static_cast<int64_t>(a) - b);
}

EJ_ALWAYS_INLINE duint32 duint32_mul(uint32_t a, uint32_t b) noexcept {
	return static_cast<duint32>(a) * b;
}

EJ_ALWAYS_INLINE duint32 duint32_mul(int32_t a, uint32_t b) noexcept {
	return static_cast<duint32>(static_cast<int64_t>(a) * b);
}

EJ_ALWAYS_INLINE duint32 duint32_mul(uint32_t a, int32_t b) noexcept {
	return static_cast<duint32>(a * static_cast<int64_t>(b));
}

EJ_ALWAYS_INLINE duint32 duint32_mul(int32_t a, int32_t b) noexcept {
	return static_cast<duint32>(static_cast<int64_t>(a) * b);
}

typedef unsigned __int128 duint64;

union duint64_u {
	duint64 Value;
	struct {
		uint64_t Low;
		uint64_t High;
	};
};

EJ_ALWAYS_INLINE duint64 make_duint64(uint64_t low, uint64_t high) noexcept {
	duint64_u u;
	u.Low = low;
	u.High = high;
	return u.Value;
}

EJ_ALWAYS_INLINE uint64_t duint64_get_low(duint64 value) noexcept {
	duint64_u u;
	u.Value = value;
	return u.Low;
}

EJ_ALWAYS_INLINE uint64_t duint64_get_high(duint64 value) noexcept {
	duint64_u u;
	u.Value = value;
	return u.High;
}

EJ_ALWAYS_INLINE duint64 duint64_add(uint64_t a, uint64_t b) noexcept {
	return static_cast<duint64>(a) + b;
}

EJ_ALWAYS_INLINE duint64 duint64_add(int64_t a, uint64_t b) noexcept {
	return static_cast<duint64>(static_cast<__int128>(a) + b);
}

EJ_ALWAYS_INLINE duint64 duint64_add(uint64_t a, int64_t b) noexcept {
	return static_cast<duint64>(a + static_cast<__int128>(b));
}

EJ_ALWAYS_INLINE duint64 duint64_add(int64_t a, int64_t b) noexcept {
	return static_cast<duint64>(static_cast<__int128>(a) + b);
}

EJ_ALWAYS_INLINE duint64 duint64_sub(uint64_t a, uint64_t b) noexcept {
	return static_cast<duint64>(a) - b;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(int64_t a, uint64_t b) noexcept {
	return static_cast<duint64>(static_cast<__int128>(a) - b);
}

EJ_ALWAYS_INLINE duint64 duint64_sub(uint64_t a, int64_t b) noexcept {
	return static_cast<duint64>(a - static_cast<__int128>(b));
}

EJ_ALWAYS_INLINE duint64 duint64_sub(int64_t a, int64_t b) noexcept {
	return static_cast<duint64>(static_cast<__int128>(a) - b);
}

EJ_ALWAYS_INLINE duint64 duint64_mul(uint64_t a, uint64_t b) noexcept {
	return static_cast<duint64>(a) * b;
}

EJ_ALWAYS_INLINE duint64 duint64_mul(int64_t a, uint64_t b) noexcept {
	return static_cast<duint64>(static_cast<__int128>(a) * b);
}

EJ_ALWAYS_INLINE duint64 duint64_mul(uint64_t a, int64_t b) noexcept {
	return static_cast<duint64>(a * static_cast<__int128>(b));
}

EJ_ALWAYS_INLINE duint64 duint64_mul(int64_t a, int64_t b) noexcept {
	return static_cast<duint64>(static_cast<__int128>(a) * b);
}

EJ_ALWAYS_INLINE duint64 duint64_shl(uint64_t a, unsigned n) noexcept {
	assert(n > 0 && n < sizeof(uint64_t) * CHAR_BIT * 2);
	return static_cast<duint64>(a) << n;
}

EJ_ALWAYS_INLINE duint64 duint64_shl(int64_t a, unsigned n) noexcept {
	assert(n > 0 && n < sizeof(uint64_t) * CHAR_BIT * 2);
	return static_cast<duint64>(a) << n;
}

template <typename T>
EJ_ALWAYS_INLINE void duint_small_shl(T *result_low, T *result_high, T value_low, T value_high, int n) noexcept {
	assert(n > 0 && n < sizeof(T) * CHAR_BIT);

	*result_high = (value_high << n) | (value_low >> (sizeof(T) * CHAR_BIT - n));
	*result_low = value_low << n;
}

template <typename T>
EJ_ALWAYS_INLINE void duint_small_shl_assign(T *low, T *high, int n) noexcept {
	return duint_small_shl(low, high, *low, *high, n);
}

EJ_ALWAYS_INLINE void duint64_small_shl_assign(uint64_t *low, uint64_t *high, int n) noexcept {
	return duint_small_shl_assign(low, high, n);
}

}
#endif
