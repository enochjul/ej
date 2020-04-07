//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

#include "BitScan.h"

namespace ej {

//! 32-bit Linear Congruential Generator
template <uint32_t A, uint32_t C>
class LCG32 {
	static_assert(A % 4 == 1);
	static_assert(C % 2 == 1);

public:
	typedef uint32_t result_type;

private:
	static const result_type Table[32][2];
	result_type Seed;

	template <result_type N>
	struct Identity {
		static constexpr result_type Value = N;
	};

	//! Finds the value of the multiplier, a, for the given discard value of 2 to the power of n
	constexpr static result_type find_a(unsigned n) {
		result_type a = A;
		for (unsigned i = 0; i < n; i++) {
			a *= a;
		}
		return a;
	}

	//! Finds the value of the increment, c, for the given discard value of 2 to the power of n
	constexpr static result_type find_c(unsigned n) {
		result_type a = A, c = C;
		for (unsigned i = 0; i < n; i++) {
			c = (a + 1) * c;
			a *= a;
		}
		return c;
	}

public:
	//! Initialize with the given seed
	explicit constexpr LCG32(result_type seed = 1) : Seed(seed) {
	}

	//! Reinitialize with the given seed
	constexpr void seed(result_type seed = 1) {
		Seed = seed;
	}

	//! Generate the next value
	constexpr result_type operator ()() {
		result_type s = A * Seed + C;
		Seed = s;
		return s;
	}

	//! Discard the next 2 to the power of n values
	constexpr void discard_pow2(unsigned n);

	//! Discard the next n values
	constexpr void discard(result_type n);
};

template <uint32_t A, uint32_t C>
const typename LCG32<A, C>::result_type LCG32<A, C>::Table[][2] = {
	{ Identity<find_a(0)>::Value, Identity<find_c(0)>::Value },
	{ Identity<find_a(1)>::Value, Identity<find_c(1)>::Value },
	{ Identity<find_a(2)>::Value, Identity<find_c(2)>::Value },
	{ Identity<find_a(3)>::Value, Identity<find_c(3)>::Value },
	{ Identity<find_a(4)>::Value, Identity<find_c(4)>::Value },
	{ Identity<find_a(5)>::Value, Identity<find_c(5)>::Value },
	{ Identity<find_a(6)>::Value, Identity<find_c(6)>::Value },
	{ Identity<find_a(7)>::Value, Identity<find_c(7)>::Value },
	{ Identity<find_a(8)>::Value, Identity<find_c(8)>::Value },
	{ Identity<find_a(9)>::Value, Identity<find_c(9)>::Value },
	{ Identity<find_a(10)>::Value, Identity<find_c(10)>::Value },
	{ Identity<find_a(11)>::Value, Identity<find_c(11)>::Value },
	{ Identity<find_a(12)>::Value, Identity<find_c(12)>::Value },
	{ Identity<find_a(13)>::Value, Identity<find_c(13)>::Value },
	{ Identity<find_a(14)>::Value, Identity<find_c(14)>::Value },
	{ Identity<find_a(15)>::Value, Identity<find_c(15)>::Value },
	{ Identity<find_a(16)>::Value, Identity<find_c(16)>::Value },
	{ Identity<find_a(17)>::Value, Identity<find_c(17)>::Value },
	{ Identity<find_a(18)>::Value, Identity<find_c(18)>::Value },
	{ Identity<find_a(19)>::Value, Identity<find_c(19)>::Value },
	{ Identity<find_a(20)>::Value, Identity<find_c(20)>::Value },
	{ Identity<find_a(21)>::Value, Identity<find_c(21)>::Value },
	{ Identity<find_a(22)>::Value, Identity<find_c(22)>::Value },
	{ Identity<find_a(23)>::Value, Identity<find_c(23)>::Value },
	{ Identity<find_a(24)>::Value, Identity<find_c(24)>::Value },
	{ Identity<find_a(25)>::Value, Identity<find_c(25)>::Value },
	{ Identity<find_a(26)>::Value, Identity<find_c(26)>::Value },
	{ Identity<find_a(27)>::Value, Identity<find_c(27)>::Value },
	{ Identity<find_a(28)>::Value, Identity<find_c(28)>::Value },
	{ Identity<find_a(29)>::Value, Identity<find_c(29)>::Value },
	{ Identity<find_a(30)>::Value, Identity<find_c(30)>::Value },
	{ Identity<find_a(31)>::Value, Identity<find_c(31)>::Value },
};

template <uint32_t A, uint32_t C>
constexpr void LCG32<A, C>::discard_pow2(unsigned n) {
	const auto &entry = Table[n & 31];
	auto a = entry[0], c = entry[1];
	Seed = a * Seed + c;
}

template <uint32_t A, uint32_t C>
constexpr void LCG32<A, C>::discard(result_type n) {
	result_type v = n, s = Seed;
	while (v != 0) {
		const auto &entry = Table[bsf32_nz(v)];
		auto a = entry[0], c = entry[1];
		s = a * s + c;
		v &= (v - 1);
	}
	Seed = s;
}

typedef LCG32<2891336453, 1640531513> LCG32_LEcuyer1;
typedef LCG32<29943829, 1013904223> LCG32_LEcuyer2;
typedef LCG32<32310901, 626627237> LCG32_LEcuyer3;

typedef LCG32<0xadb4a92d, 1> LCG32_SteeleVigna1;
typedef LCG32<0xa13fc965, 1> LCG32_SteeleVigna2;
typedef LCG32<0x8664f205, 1> LCG32_SteeleVigna3;
typedef LCG32<0xcf019d85, 1> LCG32_SteeleVigna4;

//! 64-bit Linear Congruential Generator
template <uint64_t A, uint64_t C>
class LCG64 {
	static_assert(A % 4 == 1);
	static_assert(C % 2 == 1);

public:
	typedef uint64_t result_type;

private:
	static const result_type Table[64][2];
	result_type Seed;

	template <result_type N>
	struct Identity {
		static constexpr result_type Value = N;
	};

	//! Finds the value of the multiplier, a, for the given discard value of 2 to the power of n
	constexpr static result_type find_a(unsigned n) {
		result_type a = A;
		for (unsigned i = 0; i < n; i++) {
			a *= a;
		}
		return a;
	}

	//! Finds the value of the increment, c, for the given discard value of 2 to the power of n
	constexpr static result_type find_c(unsigned n) {
		result_type a = A, c = C;
		for (unsigned i = 0; i < n; i++) {
			c = (a + 1) * c;
			a *= a;
		}
		return c;
	}

public:
	//! Initialize with the given seed
	explicit constexpr LCG64(result_type seed = 1) : Seed(seed) {
	}

	//! Reinitialize with the given seed
	constexpr void seed(result_type seed = 1) {
		Seed = seed;
	}

	//! Generate the next value
	constexpr result_type operator ()() {
		result_type s = A * Seed + C;
		Seed = s;
		return s;
	}

	//! Discard the next 2 to the power of n values
	constexpr void discard_pow2(unsigned n);

	//! Discard the next n values
	constexpr void discard(result_type n);
};

template <uint64_t A, uint64_t C>
const typename LCG64<A, C>::result_type LCG64<A, C>::Table[][2] = {
	{ Identity<find_a(0)>::Value, Identity<find_c(0)>::Value },
	{ Identity<find_a(1)>::Value, Identity<find_c(1)>::Value },
	{ Identity<find_a(2)>::Value, Identity<find_c(2)>::Value },
	{ Identity<find_a(3)>::Value, Identity<find_c(3)>::Value },
	{ Identity<find_a(4)>::Value, Identity<find_c(4)>::Value },
	{ Identity<find_a(5)>::Value, Identity<find_c(5)>::Value },
	{ Identity<find_a(6)>::Value, Identity<find_c(6)>::Value },
	{ Identity<find_a(7)>::Value, Identity<find_c(7)>::Value },
	{ Identity<find_a(8)>::Value, Identity<find_c(8)>::Value },
	{ Identity<find_a(9)>::Value, Identity<find_c(9)>::Value },
	{ Identity<find_a(10)>::Value, Identity<find_c(10)>::Value },
	{ Identity<find_a(11)>::Value, Identity<find_c(11)>::Value },
	{ Identity<find_a(12)>::Value, Identity<find_c(12)>::Value },
	{ Identity<find_a(13)>::Value, Identity<find_c(13)>::Value },
	{ Identity<find_a(14)>::Value, Identity<find_c(14)>::Value },
	{ Identity<find_a(15)>::Value, Identity<find_c(15)>::Value },
	{ Identity<find_a(16)>::Value, Identity<find_c(16)>::Value },
	{ Identity<find_a(17)>::Value, Identity<find_c(17)>::Value },
	{ Identity<find_a(18)>::Value, Identity<find_c(18)>::Value },
	{ Identity<find_a(19)>::Value, Identity<find_c(19)>::Value },
	{ Identity<find_a(20)>::Value, Identity<find_c(20)>::Value },
	{ Identity<find_a(21)>::Value, Identity<find_c(21)>::Value },
	{ Identity<find_a(22)>::Value, Identity<find_c(22)>::Value },
	{ Identity<find_a(23)>::Value, Identity<find_c(23)>::Value },
	{ Identity<find_a(24)>::Value, Identity<find_c(24)>::Value },
	{ Identity<find_a(25)>::Value, Identity<find_c(25)>::Value },
	{ Identity<find_a(26)>::Value, Identity<find_c(26)>::Value },
	{ Identity<find_a(27)>::Value, Identity<find_c(27)>::Value },
	{ Identity<find_a(28)>::Value, Identity<find_c(28)>::Value },
	{ Identity<find_a(29)>::Value, Identity<find_c(29)>::Value },
	{ Identity<find_a(30)>::Value, Identity<find_c(30)>::Value },
	{ Identity<find_a(31)>::Value, Identity<find_c(31)>::Value },
	{ Identity<find_a(32)>::Value, Identity<find_c(32)>::Value },
	{ Identity<find_a(33)>::Value, Identity<find_c(33)>::Value },
	{ Identity<find_a(34)>::Value, Identity<find_c(34)>::Value },
	{ Identity<find_a(35)>::Value, Identity<find_c(35)>::Value },
	{ Identity<find_a(36)>::Value, Identity<find_c(36)>::Value },
	{ Identity<find_a(37)>::Value, Identity<find_c(37)>::Value },
	{ Identity<find_a(38)>::Value, Identity<find_c(38)>::Value },
	{ Identity<find_a(39)>::Value, Identity<find_c(39)>::Value },
	{ Identity<find_a(40)>::Value, Identity<find_c(40)>::Value },
	{ Identity<find_a(41)>::Value, Identity<find_c(41)>::Value },
	{ Identity<find_a(42)>::Value, Identity<find_c(42)>::Value },
	{ Identity<find_a(43)>::Value, Identity<find_c(43)>::Value },
	{ Identity<find_a(44)>::Value, Identity<find_c(44)>::Value },
	{ Identity<find_a(45)>::Value, Identity<find_c(45)>::Value },
	{ Identity<find_a(46)>::Value, Identity<find_c(46)>::Value },
	{ Identity<find_a(47)>::Value, Identity<find_c(47)>::Value },
	{ Identity<find_a(48)>::Value, Identity<find_c(48)>::Value },
	{ Identity<find_a(49)>::Value, Identity<find_c(49)>::Value },
	{ Identity<find_a(50)>::Value, Identity<find_c(50)>::Value },
	{ Identity<find_a(51)>::Value, Identity<find_c(51)>::Value },
	{ Identity<find_a(52)>::Value, Identity<find_c(52)>::Value },
	{ Identity<find_a(53)>::Value, Identity<find_c(53)>::Value },
	{ Identity<find_a(54)>::Value, Identity<find_c(54)>::Value },
	{ Identity<find_a(55)>::Value, Identity<find_c(55)>::Value },
	{ Identity<find_a(56)>::Value, Identity<find_c(56)>::Value },
	{ Identity<find_a(57)>::Value, Identity<find_c(57)>::Value },
	{ Identity<find_a(58)>::Value, Identity<find_c(58)>::Value },
	{ Identity<find_a(59)>::Value, Identity<find_c(59)>::Value },
	{ Identity<find_a(60)>::Value, Identity<find_c(60)>::Value },
	{ Identity<find_a(61)>::Value, Identity<find_c(61)>::Value },
	{ Identity<find_a(62)>::Value, Identity<find_c(62)>::Value },
	{ Identity<find_a(63)>::Value, Identity<find_c(63)>::Value },
};

template <uint64_t A, uint64_t C>
constexpr void LCG64<A, C>::discard_pow2(unsigned n) {
	const auto &entry = Table[n & 63];
	auto a = entry[0], c = entry[1];
	Seed = a * Seed + c;
}

template <uint64_t A, uint64_t C>
constexpr void LCG64<A, C>::discard(result_type n) {
	result_type v = n, s = Seed;
	while (v != 0) {
		const auto &entry = Table[bsf64_nz(v)];
		auto a = entry[0], c = entry[1];
		s = a * s + c;
		v &= (v - 1);
	}
	Seed = s;
}

typedef LCG64<UINT64_C(6364136223846793005), UINT64_C(1442695040888963407)> LCG64_MMIX;
typedef LCG64<UINT64_C(2862933555777941757), UINT64_C(7046029254386353087)> LCG64_LEcuyer1;
typedef LCG64<UINT64_C(3202034522624059733), UINT64_C(4354685564936845319)> LCG64_LEcuyer2;
typedef LCG64<UINT64_C(3935559000370003845), UINT64_C(2691343689449507681)> LCG64_LEcuyer3;
typedef LCG64<UINT64_C(0xd1342543de82ef95), UINT64_C(1)> LCG64_SteeleVigna1;
typedef LCG64<UINT64_C(0xaf251af3b0f025b5), UINT64_C(1)> LCG64_SteeleVigna2;
typedef LCG64<UINT64_C(0xb564ef22ec7aece5), UINT64_C(1)> LCG64_SteeleVigna3;
typedef LCG64<UINT64_C(0xf7c2ebc08f67f2b5), UINT64_C(1)> LCG64_SteeleVigna4;

}
