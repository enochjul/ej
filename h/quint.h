//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>
#include <stdint.h>

#include <type_traits>

#include "duint.h"

namespace ej {

EJ_ALWAYS_INLINE unsigned char adc_u32(unsigned char cin, uint32_t a, uint32_t b, uint32_t *r) noexcept {
	return _addcarry_u32(cin, a, b, r);
}

EJ_ALWAYS_INLINE unsigned char adc_u64(unsigned char cin, uint64_t a, uint64_t b, uint64_t *r) noexcept {
	unsigned long long v;
	unsigned char cout = _addcarry_u64(cin, a, b, &v);
	*r = v;
	return cout;
}

template <typename T>
unsigned char adc(unsigned char cin, T a, T b, T *r) noexcept;

template <>
EJ_ALWAYS_INLINE unsigned char adc<uint32_t>(unsigned char cin, uint32_t a, uint32_t b, uint32_t *r) noexcept {
	return adc_u32(cin, a, b, r);
}

template <>
EJ_ALWAYS_INLINE unsigned char adc<uint64_t>(unsigned char cin, uint64_t a, uint64_t b, uint64_t *r) noexcept {
	return adc_u64(cin, a, b, r);
}

EJ_ALWAYS_INLINE unsigned char sbb_u32(unsigned char cin, uint32_t a, uint32_t b, uint32_t *r) noexcept {
	return _subborrow_u32(cin, a, b, r);
}

EJ_ALWAYS_INLINE unsigned char sbb_u64(unsigned char cin, uint64_t a, uint64_t b, uint64_t *r) noexcept {
	unsigned long long v;
	unsigned char cout = _subborrow_u64(cin, a, b, &v);
	*r = v;
	return cout;
}

template <typename T>
unsigned char sbb(unsigned char cin, T a, T b, T *r) noexcept;

template <>
EJ_ALWAYS_INLINE unsigned char sbb<uint32_t>(unsigned char cin, uint32_t a, uint32_t b, uint32_t *r) noexcept {
	return sbb_u32(cin, a, b, r);
}

template <>
EJ_ALWAYS_INLINE unsigned char sbb<uint64_t>(unsigned char cin, uint64_t a, uint64_t b, uint64_t *r) noexcept {
	return sbb_u64(cin, a, b, r);
}

template <typename T>
inline void quint_add(T r[4], const T a[4], const T b[4]) noexcept {
	T v0, v1, v2, v3;
	adc<T>(adc<T>(adc<T>(adc<T>(0, a[0], b[0], &v0), a[1], b[1], &v1), a[2], b[2], &v2), a[3], b[3], &v3);
	r[0] = v0, r[1] = v1, r[2] = v2, r[3] = v3;
}

template <typename T>
inline void quint_sub(T r[4], const T a[4], const T b[4]) noexcept {
	T v0, v1, v2, v3;
	sbb<T>(sbb<T>(sbb<T>(sbb<T>(0, a[0], b[0], &v0), a[1], b[1], &v1), a[2], b[2], &v2), a[3], b[3], &v3);
	r[0] = v0, r[1] = v1, r[2] = v2, r[3] = v3;
}

template <typename T>
void quint_mul(T r[4], const T a[4], const T b[4]) noexcept {
	//The following shows the multiplication formula for each of the 8 words of the 4x4 multiply:
	//r0 = a0b0_low
	//r1 = (a0b0_high + a1b0_low) + a0b1_low
	//r2 = ((a1b0_high + a2b0_low) + (a0b1_high + a1b1_low)) + a0b2_low
	//r3 = ((a2b0_high + a3b0_low) + (a1b1_high + a2b1_low)) + ((a0b2_high + a1b2_low) + a0b3_low)
	//r4 = (a3b0_high + a2b1_high) + (a3b1_low + a1b2_high) + (a2b2_low + a0b3_high) + a1b3_low
	//r5 = (a3b1_high + a2b2_high) + (a3b2_low + a1b3_high) + a2b3_low
	//r6 = (a3b2_high + a2b3_high) + a3b3_low
	//r7 = a3b3_high

	//Converting the formula so as to compute each variable with a single add and adding additional variables to compensate:
	//r0 = a0b0_low
	//
	//r1_0 = a0b0_high + a1b0_low
	//r1 = r1_0 + a0b1_low
	//
	//r2_0 = a1b0_high + a2b0_low
	//r2_1 = a0b1_high + a1b1_low
	//r2_2 = r2_0 + r2_1
	//r2 = r2_2 + a0b2_low
	//
	//r3_0 = a2b0_high + a3b0_low
	//r3_1 = a1b1_high + a2b1_low
	//r3_2 = a0b2_high + a1b2_low
	//r3_3 = r3_0 + r3_1
	//r3_4 = r3_2 + a0b3_low
	//r3 = r3_3 + r3_4

	auto a0b0 = duint_s<T>::mul(a[0], b[0]);
	auto a0b0_low = duint_s<T>::get_low(a0b0), a0b0_high = duint_s<T>::get_high(a0b0);

	auto a1b0 = duint_s<T>::mul(a[1], b[0]);
	auto a1b0_low = duint_s<T>::get_low(a1b0), a1b0_high = duint_s<T>::get_high(a1b0);

	auto a2b0 = duint_s<T>::mul(a[2], b[0]);
	auto a2b0_low = duint_s<T>::get_low(a2b0), a2b0_high = duint_s<T>::get_high(a2b0);

	auto a3b0 = duint_s<T>::mul(a[3], b[0]);
	auto a3b0_low = duint_s<T>::get_low(a3b0);


	auto a0b1 = duint_s<T>::mul(a[0], b[1]);
	auto a0b1_low = duint_s<T>::get_low(a0b1), a0b1_high = duint_s<T>::get_high(a0b1);

	auto a1b1 = duint_s<T>::mul(a[1], b[1]);
	auto a1b1_low = duint_s<T>::get_low(a1b1), a1b1_high = duint_s<T>::get_high(a1b1);

	auto a2b1 = duint_s<T>::mul(a[2], b[1]);
	auto a2b1_low = duint_s<T>::get_low(a2b1);


	auto a0b2 = duint_s<T>::mul(a[0], b[2]);
	auto a0b2_low = duint_s<T>::get_low(a0b2), a0b2_high = duint_s<T>::get_high(a0b2);

	auto a1b2 = duint_s<T>::mul(a[1], b[2]);
	auto a1b2_low = duint_s<T>::get_low(a1b2);


	auto a0b3 = duint_s<T>::mul(a[0], b[3]);
	auto a0b3_low = duint_s<T>::get_low(a0b3);


	T r1_0, r2_0, r2_1, r2_2, r3_0, r3_1, r3_2, r3_3, r3_4;

	r[0] = a0b0_low;
	adc<T>(adc<T>(adc<T>(0, a0b0_high, a1b0_low, &r1_0), a1b0_high, a2b0_low, &r2_0), a2b0_high, a3b0_low, &r3_0);
	adc<T>(adc<T>(adc<T>(0, r1_0, a0b1_low, r + 1), a0b1_high, a1b1_low, &r2_1), a1b1_high, a2b1_low, &r3_1);
	adc<T>(adc<T>(0, r2_0, r2_1, &r2_2), a0b2_high, a1b2_low, &r3_2);
	adc<T>(adc<T>(0, r2_2, a0b2_low, r + 2), r3_0, r3_1, &r3_3);
	r3_4 = r3_2 + a0b3_low;
	r[3] = r3_3 + r3_4;
}

template <typename T>
class quint {
public:
	typedef T value_type;
	typedef quint<value_type> this_type;

private:
	value_type Data[4];

public:
	constexpr quint() noexcept = default;
	constexpr quint(value_type a0, value_type a1, value_type a2, value_type a3) noexcept : Data{ a0, a1, a2, a3 } {
	}
	constexpr quint(const value_type a[4]) noexcept : Data{ a[0], a[1], a[2], a[3] } {
	}

	constexpr void set(value_type a0, value_type a1, value_type a2, value_type a3) noexcept {
		Data[0] = a0, Data[1] = a1, Data[2] = a2, Data[3] = a3;
	}

	constexpr void set(const value_type a[4]) noexcept {
		Data[0] = a[0], Data[1] = a[1], Data[2] = a[2], Data[3] = a[3];
	}

	constexpr value_type &operator [](unsigned index) noexcept {
		assert(index < 4);
		return Data[index];
	}

	constexpr const value_type &operator [](unsigned index) const noexcept {
		assert(index < 4);
		return Data[index];
	}

	constexpr value_type *data() noexcept {
		return Data;
	}

	constexpr const value_type *data() const noexcept {
		return Data;
	}

	void add(value_type a, value_type b) noexcept {
		Data[1] = static_cast<value_type>(adc(0, a, b, Data + 0)), Data[2] = 0, Data[3] = 0;
	}

	void add(const this_type &a, const this_type &b) noexcept {
		quint_add<value_type>(Data, a.Data, b.Data);
	}

	void sub(value_type a, value_type b) noexcept {
		value_type cout = static_cast<value_type>(0) - sbb(0, a, b, Data + 0);
		Data[1] = cout, Data[2] = cout, Data[3] = cout;
	}

	void sub(const this_type &a, const this_type &b) noexcept {
		quint_sub<value_type>(Data, a.Data, b.Data);
	}

	void mul(const this_type &a, const this_type &b) noexcept {
		quint_mul<value_type>(Data, a.Data, b.Data);
	}
};

typedef quint<uint32_t> quint32;
static_assert(std::is_trivially_constructible<quint32>::value);
static_assert(std::is_trivially_destructible<quint32>::value);
static_assert(std::is_trivially_copyable<quint32>::value);

typedef quint<uint64_t> quint64;
static_assert(std::is_trivially_constructible<quint64>::value);
static_assert(std::is_trivially_destructible<quint64>::value);
static_assert(std::is_trivially_copyable<quint64>::value);

}
