//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>
#include <limits.h>
#include <stdint.h>

#include <x86intrin.h>

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

template <typename T>
EJ_ALWAYS_INLINE void duint_small_shl(T *result_low, T *result_high, T value_low, T value_high, int n) noexcept {
	assert(n > 0 && static_cast<unsigned>(n) < sizeof(T) * CHAR_BIT);

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

#if EJ_WORD_SIZE == 64

typedef unsigned __int128 duint64;

union duint64_u {
	duint64 Value;
	struct {
		uint64_t Low;
		uint64_t High;
	};

	constexpr duint64_u(duint64 v) noexcept : Value(v) {
	}
	constexpr duint64_u(uint64_t low, uint64_t high) noexcept : Low(low), High(high) {
	}
};

EJ_ALWAYS_INLINE constexpr duint64 make_duint64(uint64_t low, uint64_t high) noexcept {
	return duint64_u(low, high).Value;
}

EJ_ALWAYS_INLINE constexpr uint64_t duint64_get_low(duint64 value) noexcept {
	return duint64_u(value).Low;
}

EJ_ALWAYS_INLINE constexpr uint64_t duint64_get_high(duint64 value) noexcept {
	return duint64_u(value).High;
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

EJ_ALWAYS_INLINE duint64 duint64_add(duint64 a, uint32_t b) noexcept {
	return a + b;
}

EJ_ALWAYS_INLINE duint64 duint64_add(duint64 a, int32_t b) noexcept {
	return a + b;
}

EJ_ALWAYS_INLINE duint64 duint64_add(duint64 a, uint64_t b) noexcept {
	return a + b;
}

EJ_ALWAYS_INLINE duint64 duint64_add(duint64 a, int64_t b) noexcept {
	return a + b;
}

EJ_ALWAYS_INLINE duint64 duint64_add(uint64_t a, duint64 b) noexcept {
	return a + b;
}

EJ_ALWAYS_INLINE duint64 duint64_add(int64_t a, duint64 b) noexcept {
	return a + b;
}

EJ_ALWAYS_INLINE duint64 duint64_add(duint64 a, duint64 b) noexcept {
	return a + b;
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

EJ_ALWAYS_INLINE duint64 duint64_sub(duint64 a, uint64_t b) noexcept {
	return a - b;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(duint64 a, int64_t b) noexcept {
	return a - b;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(uint64_t a, duint64 b) noexcept {
	return a - b;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(int64_t a, duint64 b) noexcept {
	return a - b;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(duint64 a, duint64 b) noexcept {
	return a - b;
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

EJ_ALWAYS_INLINE duint64 duint64_mul(duint64 a, uint32_t b) noexcept {
	return a * b;
}

EJ_ALWAYS_INLINE duint64 duint64_mul(duint64 a, uint64_t b) noexcept {
	return a * b;
}

EJ_ALWAYS_INLINE duint64 duint64_mul(uint64_t a, duint64 b) noexcept {
	return a * b;
}

EJ_ALWAYS_INLINE duint64 duint64_shl(uint64_t a, int n) noexcept {
	assert(n > 0 && static_cast<unsigned>(n) < sizeof(uint64_t) * CHAR_BIT * 2);
	return static_cast<duint64>(a) << n;
}

EJ_ALWAYS_INLINE duint64 duint64_shl(int64_t a, int n) noexcept {
	assert(n > 0 && static_cast<unsigned>(n) < sizeof(uint64_t) * CHAR_BIT * 2);
	return static_cast<duint64>(a) << n;
}

#else

union duint64 {
	struct {
		uint64_t Low;
		uint64_t High;
	};
	uint32_t Data[4];

	EJ_ALWAYS_INLINE constexpr duint64 &operator =(uint32_t a) noexcept {
		Data[0] = a;
		Data[1] = 0;
		Data[2] = 0;
		Data[3] = 0;
		return *this;
	}

	EJ_ALWAYS_INLINE constexpr duint64 &operator =(int32_t a) noexcept {
		uint32_t sign = static_cast<uint32_t>(a >> 31);
		Data[0] = static_cast<uint32_t>(a);
		Data[1] = sign;
		Data[2] = sign;
		Data[3] = sign;
		return *this;
	}

	EJ_ALWAYS_INLINE constexpr duint64 &operator =(uint64_t a) noexcept {
		Low = a, High = 0;
		return *this;
	}

	EJ_ALWAYS_INLINE constexpr duint64 &operator =(int64_t a) noexcept {
		Low = a, High = a >> 63;
		return *this;
	}
};

EJ_ALWAYS_INLINE constexpr duint64 make_duint64(uint64_t low, uint64_t high) noexcept {
	return duint64{ low, high };
}

EJ_ALWAYS_INLINE constexpr uint64_t duint64_get_low(const duint64 &value) noexcept {
	return value.Low;
}

EJ_ALWAYS_INLINE constexpr uint64_t duint64_get_high(const duint64 &value) noexcept {
	return value.High;
}

EJ_ALWAYS_INLINE duint64 duint64_add(uint64_t a, uint64_t b) noexcept {
	duint64 r;

	auto carry = __builtin_add_overflow(static_cast<uint32_t>(a), static_cast<uint32_t>(b), r.Data + 0);
	r.Data[2] = _addcarry_u32(carry, static_cast<uint32_t>(a >> 32), static_cast<uint32_t>(b >> 32), r.Data + 1);
	r.Data[3] = 0;

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(int64_t a, uint64_t b) noexcept {
	duint64 r;

	int64_t sign_and_high = a >> 32;
	uint32_t high = static_cast<uint32_t>(sign_and_high);
	uint32_t sign = static_cast<uint32_t>(static_cast<uint64_t>(sign_and_high) >> 32);
	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		static_cast<uint32_t>(a), static_cast<uint32_t>(b), r.Data + 0),
		high, static_cast<uint32_t>(b >> 32), r.Data + 1),
		sign, 0, r.Data + 2),
		sign, 0, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(uint64_t a, int64_t b) noexcept {
	duint64 r;

	int64_t sign_and_high = b >> 32;
	uint32_t high = static_cast<uint32_t>(sign_and_high);
	uint32_t sign = static_cast<uint32_t>(static_cast<uint64_t>(sign_and_high) >> 32);
	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		static_cast<uint32_t>(a), static_cast<uint32_t>(b), r.Data + 0),
		static_cast<uint32_t>(a >> 32), high, r.Data + 1),
		0, sign, r.Data + 2),
		0, sign, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(int64_t a, int64_t b) noexcept {
	duint64 r;

	int64_t a_sign_and_high = a >> 32;
	uint32_t a_high = static_cast<uint32_t>(a_sign_and_high);
	uint32_t a_sign = static_cast<uint32_t>(static_cast<uint64_t>(a_sign_and_high) >> 32);
	int64_t b_sign_and_high = b >> 32;
	uint32_t b_high = static_cast<uint32_t>(b_sign_and_high);
	uint32_t b_sign = static_cast<uint32_t>(static_cast<uint64_t>(b_sign_and_high) >> 32);
	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		static_cast<uint32_t>(a), static_cast<uint32_t>(b), r.Data + 0),
		a_high, b_high, r.Data + 1),
		a_sign, b_sign, r.Data + 2),
		a_sign, b_sign, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(const duint64 &a, uint32_t b) noexcept {
	duint64 r;

	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		a.Data[0], b, r.Data + 0),
		a.Data[1], 0, r.Data + 1),
		a.Data[2], 0, r.Data + 2),
		a.Data[3], 0, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(const duint64 &a, int32_t b) noexcept {
	duint64 r;

	uint32_t sign = static_cast<uint32_t>(b >> 31);
	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		a.Data[0], static_cast<uint32_t>(b), r.Data + 0),
		a.Data[1], sign, r.Data + 1),
		a.Data[2], sign, r.Data + 2),
		a.Data[3], sign, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(const duint64 &a, uint64_t b) noexcept {
	duint64 r;

	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		a.Data[0], static_cast<uint32_t>(b), r.Data + 0),
		a.Data[1], static_cast<uint32_t>(b >> 32), r.Data + 1),
		a.Data[2], 0, r.Data + 2),
		a.Data[3], 0, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(const duint64 &a, int64_t b) noexcept {
	duint64 r;

	int64_t sign_and_high = b >> 32;
	uint32_t high = static_cast<uint32_t>(sign_and_high);
	uint32_t sign = static_cast<uint32_t>(static_cast<uint64_t>(sign_and_high) >> 32);
	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		a.Data[0], static_cast<uint32_t>(b), r.Data + 0),
		a.Data[1], high, r.Data + 1),
		a.Data[2], sign, r.Data + 2),
		a.Data[3], sign, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(uint64_t a, const duint64 &b) noexcept {
	duint64 r;

	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		static_cast<uint32_t>(a), b.Data[0], r.Data + 0),
		static_cast<uint32_t>(a >> 32), b.Data[1], r.Data + 1),
		0, b.Data[2], r.Data + 2),
		0, b.Data[3], r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(int64_t a, const duint64 &b) noexcept {
	duint64 r;

	int64_t sign_and_high = a >> 32;
	uint32_t high = static_cast<uint32_t>(sign_and_high);
	uint32_t sign = static_cast<uint32_t>(static_cast<uint64_t>(sign_and_high) >> 32);
	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		static_cast<uint32_t>(a), b.Data[0], r.Data + 0),
		high, b.Data[1], r.Data + 1),
		sign, b.Data[2], r.Data + 2),
		sign, b.Data[3], r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_add(const duint64 &a, const duint64 &b) noexcept {
	duint64 r;

	_addcarry_u32(_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		a.Data[0], b.Data[0], r.Data + 0),
		a.Data[1], b.Data[1], r.Data + 1),
		a.Data[2], b.Data[2], r.Data + 2),
		a.Data[3], b.Data[3], r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(uint64_t a, uint64_t b) noexcept {
	duint64 r;
	uint32_t borrow;

	_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		static_cast<uint32_t>(a), static_cast<uint32_t>(b), r.Data + 0),
		static_cast<uint32_t>(a >> 32), static_cast<uint32_t>(b >> 32), r.Data + 1),
		0, 0, &borrow);
	r.Data[2] = borrow;
	r.Data[3] = borrow;

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(int64_t a, uint64_t b) noexcept {
	duint64 r;

	int64_t sign_and_high = a >> 32;
	uint32_t high = static_cast<uint32_t>(sign_and_high);
	uint32_t sign = static_cast<uint32_t>(static_cast<uint64_t>(sign_and_high) >> 32);
	_subborrow_u32(_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		static_cast<uint32_t>(a), static_cast<uint32_t>(b), r.Data + 0),
		high, static_cast<uint32_t>(b >> 32), r.Data + 1),
		sign, 0, r.Data + 2),
		sign, 0, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(uint64_t a, int64_t b) noexcept {
	duint64 r;

	int64_t sign_and_high = b >> 32;
	uint32_t high = static_cast<uint32_t>(sign_and_high);
	uint32_t sign = static_cast<uint32_t>(static_cast<uint64_t>(sign_and_high) >> 32);
	_subborrow_u32(_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		static_cast<uint32_t>(a), static_cast<uint32_t>(b), r.Data + 0),
		static_cast<uint32_t>(a >> 32), high, r.Data + 1),
		0, sign, r.Data + 2),
		0, sign, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(int64_t a, int64_t b) noexcept {
	duint64 r;

	int64_t a_sign_and_high = a >> 32;
	uint32_t a_high = static_cast<uint32_t>(a_sign_and_high);
	uint32_t a_sign = static_cast<uint32_t>(static_cast<uint64_t>(a_sign_and_high) >> 32);
	int64_t b_sign_and_high = b >> 32;
	uint32_t b_high = static_cast<uint32_t>(b_sign_and_high);
	uint32_t b_sign = static_cast<uint32_t>(static_cast<uint64_t>(b_sign_and_high) >> 32);
	_subborrow_u32(_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		static_cast<uint32_t>(a), static_cast<uint32_t>(b), r.Data + 0),
		a_high, b_high, r.Data + 1),
		a_sign, b_sign, r.Data + 2),
		a_sign, b_sign, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(const duint64 &a, uint64_t b) noexcept {
	duint64 r;

	_subborrow_u32(_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		a.Data[0], static_cast<uint32_t>(b), r.Data + 0),
		a.Data[1], static_cast<uint32_t>(b >> 32), r.Data + 1),
		a.Data[2], 0, r.Data + 2),
		a.Data[3], 0, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(const duint64 &a, int64_t b) noexcept {
	duint64 r;

	int64_t sign_and_high = b >> 32;
	uint32_t high = static_cast<uint32_t>(sign_and_high);
	uint32_t sign = static_cast<uint32_t>(static_cast<uint64_t>(sign_and_high) >> 32);
	_subborrow_u32(_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		a.Data[0], static_cast<uint32_t>(b), r.Data + 0),
		a.Data[1], high, r.Data + 1),
		a.Data[2], sign, r.Data + 2),
		a.Data[3], sign, r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(uint64_t a, const duint64 &b) noexcept {
	duint64 r;

	_subborrow_u32(_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		static_cast<uint32_t>(a), b.Data[0], r.Data + 0),
		static_cast<uint32_t>(a >> 32), b.Data[1], r.Data + 1),
		0, b.Data[2], r.Data + 2),
		0, b.Data[3], r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(int64_t a, const duint64 &b) noexcept {
	duint64 r;

	int64_t sign_and_high = a >> 32;
	uint32_t high = static_cast<uint32_t>(sign_and_high);
	uint32_t sign = static_cast<uint32_t>(static_cast<uint64_t>(sign_and_high) >> 32);
	_subborrow_u32(_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		static_cast<uint32_t>(a), b.Data[0], r.Data + 0),
		high, b.Data[1], r.Data + 1),
		sign, b.Data[2], r.Data + 2),
		sign, b.Data[3], r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_sub(const duint64 &a, const duint64 &b) noexcept {
	duint64 r;

	_subborrow_u32(_subborrow_u32(_subborrow_u32(__builtin_sub_overflow(
		a.Data[0], b.Data[0], r.Data + 0),
		a.Data[1], b.Data[1], r.Data + 1),
		a.Data[2], b.Data[2], r.Data + 2),
		a.Data[3], b.Data[3], r.Data + 3);

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_mul(uint64_t a, uint64_t b) noexcept {
	uint32_t a_l = static_cast<uint32_t>(a), a_h = static_cast<uint32_t>(a >> 32);
	uint32_t b_l = static_cast<uint32_t>(b), b_h = static_cast<uint32_t>(b >> 32);

	uint64_t p0 = static_cast<uint64_t>(a_l) * b_l;
	uint64_t p1 = static_cast<uint64_t>(a_l) * b_h;
	uint64_t p2 = static_cast<uint64_t>(a_h) * b_l;
	uint64_t p3 = static_cast<uint64_t>(a_h) * b_h;

	uint32_t p0_l = static_cast<uint32_t>(p0), p0_h = static_cast<uint32_t>(p0 >> 32);
	uint32_t p1_l = static_cast<uint32_t>(p1), p1_h = static_cast<uint32_t>(p1 >> 32);
	uint32_t p2_l = static_cast<uint32_t>(p2), p2_h = static_cast<uint32_t>(p2 >> 32);
	uint32_t p3_l = static_cast<uint32_t>(p3), p3_h = static_cast<uint32_t>(p3 >> 32);

	uint32_t s0, s1, s2, s3;
	s0 = p0_l;
	_addcarry_u32(_addcarry_u32(__builtin_add_overflow(p0_h, p1_l, &s1), p1_h, p2_h, &s2), p3_h, 0, &s3);
	_addcarry_u32(_addcarry_u32(__builtin_add_overflow(s1, p2_l, &s1), s2, p3_l, &s2), s3, 0, &s3);

	duint64 r;
	r.Data[0] = s0;
	r.Data[1] = s1;
	r.Data[2] = s2;
	r.Data[3] = s3;
	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_mul(const duint64 &a, uint32_t b) noexcept {
	uint64_t p0 = static_cast<uint64_t>(a.Data[0]) * b;
	uint64_t p1 = static_cast<uint64_t>(a.Data[1]) * b;
	uint64_t p2 = static_cast<uint64_t>(a.Data[2]) * b;
	uint64_t p3 = static_cast<uint64_t>(a.Data[3]) * b;

	uint32_t p0_l = static_cast<uint32_t>(p0), p0_h = static_cast<uint32_t>(p0 >> 32);
	uint32_t p1_l = static_cast<uint32_t>(p1), p1_h = static_cast<uint32_t>(p1 >> 32);
	uint32_t p2_l = static_cast<uint32_t>(p2), p2_h = static_cast<uint32_t>(p2 >> 32);
	uint32_t p3_l = static_cast<uint32_t>(p3);

	uint32_t s0, s1, s2, s3;
	s0 = p0_l;
	_addcarry_u32(_addcarry_u32(__builtin_add_overflow(
		p0_h, p1_l, &s1),
		p1_h, p2_l, &s2),
		p2_h, p3_l, &s3);

	duint64 r;
	r.Data[0] = s0;
	r.Data[1] = s1;
	r.Data[2] = s2;
	r.Data[3] = s3;
	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_mul(const duint64 &a, uint64_t b) noexcept {
	duint64 r;

	duint64 p0 = duint64_mul(a.Low, b);
	duint64 p1 = duint64_mul(a.High, b);

	r.Low = p0.Low;
	r.High = p0.High + p1.Low;

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_mul(uint64_t a, const duint64 &b) noexcept {
	duint64 r;

	duint64 p0 = duint64_mul(a, b.Low);
	duint64 p1 = duint64_mul(a, b.High);

	r.Low = p0.Low;
	r.High = p0.High + p1.Low;

	return r;
}

EJ_ALWAYS_INLINE duint64 duint64_shl(uint64_t a, int n) noexcept {
	duint64 r;

	duint_small_shl<uint64_t>(&r.Low, &r.High, a, 0, n);
	return r;
}

EJ_ALWAYS_INLINE bool operator ==(const duint64 &a, const duint64 &b) noexcept {
	return a.Low == b.Low && a.High == b.High;
}

#endif

EJ_ALWAYS_INLINE void duint64_mul_assign(duint64 *a, uint32_t b) noexcept {
	*a = duint64_mul(*a, b);
}

}
