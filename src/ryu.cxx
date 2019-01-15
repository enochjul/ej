//Boost Software License - Version 1.0 - August 17th, 2003
//
//Permission is hereby granted, free of charge, to any person or organization
//obtaining a copy of the software and accompanying documentation covered by
//this license (the "Software") to use, reproduce, display, distribute,
//execute, and transmit the Software, and to prepare derivative works of the
//Software, and to permit third-parties to whom the Software is furnished to
//do so, all subject to the following:
//
//The copyright notices in the Software and this entire statement, including
//the above license grant, this restriction and the following disclaimer,
//must be included in all copies of the Software, in whole or in part, and
//all derivative works of the Software, unless such copies or derivative
//works are solely in the form of machine-executable object code generated by
//a source language processor.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.

#include <assert.h>
#include <float.h>
#include <stdint.h>

#include "Common.h"
#include "StringTo.h"

namespace ej {

namespace ryu {

constexpr unsigned RECIPROCAL_OF_POW5_32_NUMBER_OF_BITS = 59;

static const uint64_t ReciprocalOfPow5_32[] = {
	UINT64_C(576460752303423489),
	UINT64_C(461168601842738791),
	UINT64_C(368934881474191033),
	UINT64_C(295147905179352826),
	UINT64_C(472236648286964522),
	UINT64_C(377789318629571618),
	UINT64_C(302231454903657294),
	UINT64_C(483570327845851670),
	UINT64_C(386856262276681336),
	UINT64_C(309485009821345069),
	UINT64_C(495176015714152110),
	UINT64_C(396140812571321688),
	UINT64_C(316912650057057351),
	UINT64_C(507060240091291761),
	UINT64_C(405648192073033409),
	UINT64_C(324518553658426727),
	UINT64_C(519229685853482763),
	UINT64_C(415383748682786211),
	UINT64_C(332306998946228969),
	UINT64_C(531691198313966350),
	UINT64_C(425352958651173080),
	UINT64_C(340282366920938464),
	UINT64_C(544451787073501542),
	UINT64_C(435561429658801234),
	UINT64_C(348449143727040987),
	UINT64_C(557518629963265579),
	UINT64_C(446014903970612463),
	UINT64_C(356811923176489971),
	UINT64_C(570899077082383953),
	UINT64_C(456719261665907162),
	UINT64_C(365375409332725730),
};

constexpr unsigned POWER_OF_5_32_NUMBER_OF_BITS = 61;

static const uint64_t PowerOf5_32[] = {
	UINT64_C(1152921504606846976),
	UINT64_C(1441151880758558720),
	UINT64_C(1801439850948198400),
	UINT64_C(2251799813685248000),
	UINT64_C(1407374883553280000),
	UINT64_C(1759218604441600000),
	UINT64_C(2199023255552000000),
	UINT64_C(1374389534720000000),
	UINT64_C(1717986918400000000),
	UINT64_C(2147483648000000000),
	UINT64_C(1342177280000000000),
	UINT64_C(1677721600000000000),
	UINT64_C(2097152000000000000),
	UINT64_C(1310720000000000000),
	UINT64_C(1638400000000000000),
	UINT64_C(2048000000000000000),
	UINT64_C(1280000000000000000),
	UINT64_C(1600000000000000000),
	UINT64_C(2000000000000000000),
	UINT64_C(1250000000000000000),
	UINT64_C(1562500000000000000),
	UINT64_C(1953125000000000000),
	UINT64_C(1220703125000000000),
	UINT64_C(1525878906250000000),
	UINT64_C(1907348632812500000),
	UINT64_C(1192092895507812500),
	UINT64_C(1490116119384765625),
	UINT64_C(1862645149230957031),
	UINT64_C(1164153218269348144),
	UINT64_C(1455191522836685180),
	UINT64_C(1818989403545856475),
	UINT64_C(2273736754432320594),
	UINT64_C(1421085471520200371),
	UINT64_C(1776356839400250464),
	UINT64_C(2220446049250313080),
	UINT64_C(1387778780781445675),
	UINT64_C(1734723475976807094),
	UINT64_C(2168404344971008868),
	UINT64_C(1355252715606880542),
	UINT64_C(1694065894508600678),
	UINT64_C(2117582368135750847),
	UINT64_C(1323488980084844279),
	UINT64_C(1654361225106055349),
	UINT64_C(2067951531382569187),
	UINT64_C(1292469707114105741),
	UINT64_C(1615587133892632177),
	UINT64_C(2019483917365790221),
};

struct div_pow5_32_table_entry {
	uint32_t Divisor;
	uint32_t Reciprocal;
};

const div_pow5_32_table_entry DivPow5_32_Table[] = {
	{ 1u, 4294967295u },
	{ 5u, 858993459u },
	{ 25u, 171798691u },
	{ 125u, 34359738u },
	{ 625u, 6871947u },
	{ 3125u, 1374389u },
	{ 15625u, 274877u },
	{ 78125u, 54975u },
	{ 390625u, 10995u },
	{ 1953125u, 2199u },
	{ 9765625u, 439u },
	{ 48828125u, 87u },
	{ 244140625u, 17u },
	{ 1220703125u, 3u },
};

EJ_ALWAYS_INLINE uint32_t mod_pow5_32(uint32_t value, uint32_t exponent) noexcept {
	assert(exponent < (sizeof(DivPow5_32_Table) / sizeof(DivPow5_32_Table[0])));

	const auto &entry = DivPow5_32_Table[exponent];
	auto divisor = entry.Divisor, reciprocal = entry.Reciprocal;
	uint32_t quotient = (static_cast<uint64_t>(value) * reciprocal) >> 32;
	uint32_t remainder = value - divisor * quotient;
	remainder -= (remainder >= divisor ? divisor : 0);

	assert(remainder < divisor);

	return remainder;
}

EJ_ALWAYS_INLINE bool divisible_by_pow5(uint32_t value, uint32_t exponent) noexcept {
	return mod_pow5_32(value, exponent) == 0;
}

EJ_ALWAYS_INLINE bool divisible_by_pow2(uint32_t value, uint32_t exponent) noexcept {
	assert(value > 0);
	return static_cast<uint32_t>(__builtin_ctz(value)) >= exponent;
}

EJ_ALWAYS_INLINE int approximate_ceil_log2_5_e(int exponent) noexcept {
	assert(exponent >= 0 && exponent <= 788);
	return (static_cast<unsigned>(exponent) * 304339u / 131072u) + 1;
}

EJ_ALWAYS_INLINE int approximate_floor_log10_2_e(int exponent) noexcept {
	assert(exponent >= 0 && exponent <= 680);
	return static_cast<unsigned>(exponent) * 1233u / 4096u;
}

EJ_ALWAYS_INLINE int approximate_floor_log10_5_e(int exponent) noexcept {
	assert(exponent >= 0 && exponent <= 484);
	return static_cast<unsigned>(exponent) * 91615u / 131072u;
}

EJ_ALWAYS_INLINE uint32_t mul_and_shr_32(uint32_t a, uint64_t b, unsigned s) noexcept {
	assert(s > 32 && s < 64);
	//This is slightly faster than using 128-bit integers possibly because gcc does not know that
	//the shift count is small enough that it only needs a shrd and the latency of shlx/shrx is less than shrd
	//Inline assembly of shrd does not work well either, perjhaps gcc does not know how to schedule it?
	unsigned __int128 product = static_cast<unsigned __int128>(a) * b;
	uint64_t product_h = static_cast<uint64_t>(product >> 64);
	uint64_t product_l = static_cast<uint64_t>(product);

	return (product_l >> s) | (product_h << (64 - s));
}

EJ_ALWAYS_INLINE uint32_t div_pow5_and_shr_32(uint32_t a, uint32_t exponent5, int exponent2) noexcept {
	return mul_and_shr_32(a, ReciprocalOfPow5_32[exponent5], exponent2);
}

EJ_ALWAYS_INLINE uint32_t mul_pow5_and_shr_32(uint32_t a, uint32_t exponent5, int exponent2) noexcept {
	return mul_and_shr_32(a, PowerOf5_32[exponent5], exponent2);
}

}

char *float_to_string_no_nul(char *s, float value) noexcept {
	constexpr unsigned number_of_exponent_bits = 32 - FLT_MANT_DIG;
	constexpr unsigned bias = (1u << (number_of_exponent_bits - 1)) - 1;
	constexpr uint32_t sign_mask = 1u << (32 - 1);
	constexpr uint32_t biased_exponent_mask = ((1u << number_of_exponent_bits) - 1) << (FLT_MANT_DIG - 1);
	constexpr uint32_t partial_mantissa_mask = (1u << (FLT_MANT_DIG - 1)) - 1;

	union {
		float f;
		uint32_t u;
	} u;
	static_assert(sizeof(u) == sizeof(uint32_t));

	uint32_t sign, biased_exponent, partial_mantissa;
	int exponent2_less_2;
	uint32_t full_mantissa;
	uint32_t scaled_mantissa, scaled_mantissa_ub, scaled_mantissa_lb_offset, scaled_mantissa_lb;
	uint32_t value10, value10_ub, value10_lb;
	uint32_t q;
	int exponent10;
	int i, j, k, l;
	uint32_t significand10;
	uint32_t last_removed_digit;
	bool value10_lb_has_trailing_zeros;
	bool value10_has_trailing_zeros;
	bool accept_bounds;

	//Step 1: Decode the sign, exponent, mantissa and determine if the number is 0/inf/nan
	u.f = value;
	sign = u.u & sign_mask;
	if (sign != 0) {
		s[0] = '-';
		s++;
	}

	biased_exponent = (u.u & biased_exponent_mask) >> (FLT_MANT_DIG - 1);
	partial_mantissa = u.u & partial_mantissa_mask;

	if (biased_exponent != 0) {
		if (biased_exponent != ((1u << (32 - FLT_MANT_DIG)) - 1)) {
			exponent2_less_2 = biased_exponent - bias - (FLT_MANT_DIG - 1) - 2;
			full_mantissa = (1u << (FLT_MANT_DIG - 1)) | partial_mantissa;
		} else {
			if (partial_mantissa != 0) {
				s[0] = 'n', s[1] = 'a', s[2] = 'n', s[3] = '\0';
			} else {
				s[0] = 'i', s[1] = 'n', s[2] = 'f', s[3] = '\0';
			}
			return s + 3;
		}
	} else {
		if (partial_mantissa != 0) {
			exponent2_less_2 = 1 - bias - (FLT_MANT_DIG - 1) - 2;
			full_mantissa = partial_mantissa;
		} else {
			s[0] = '0';
			return s + 1;
		}
	}
	accept_bounds = (full_mantissa & 1) == 0;

	//Step 2: Determine the interval of legal decimal representations
	scaled_mantissa = 4 * full_mantissa;
	scaled_mantissa_ub = 4 * full_mantissa + 2;
	scaled_mantissa_lb_offset = partial_mantissa != 0 || biased_exponent <= 1 ? 1 : 0;
	scaled_mantissa_lb = 4 * full_mantissa - 1 - scaled_mantissa_lb_offset;

	//Step 3: Convert to decimal power base
	value10_lb_has_trailing_zeros = false;
	value10_has_trailing_zeros = false;
	last_removed_digit = 0;
	if (exponent2_less_2 >= 0) {
		q = ryu::approximate_floor_log10_2_e(exponent2_less_2);
		exponent10 = q;
		k = ryu::RECIPROCAL_OF_POW5_32_NUMBER_OF_BITS + ryu::approximate_ceil_log2_5_e(q) - 1;
		i = -exponent2_less_2 + q + k;
		value10 = ryu::div_pow5_and_shr_32(scaled_mantissa, q, i);
		value10_ub = ryu::div_pow5_and_shr_32(scaled_mantissa_ub, q, i);
		value10_lb = ryu::div_pow5_and_shr_32(scaled_mantissa_lb, q, i);

		if (q != 0 && (value10_ub - 1) / 10 <= value10_lb / 10) {
			l = ryu::RECIPROCAL_OF_POW5_32_NUMBER_OF_BITS + ryu::approximate_ceil_log2_5_e(q - 1) - 1;
			last_removed_digit = ryu::div_pow5_and_shr_32(scaled_mantissa, q - 1, -exponent2_less_2 + q - 1 + l) % 10;
		}
		if (q <= 9) {
			if (scaled_mantissa % 5 == 0) {
				value10_has_trailing_zeros = ryu::divisible_by_pow5(scaled_mantissa, q);
			} else if (accept_bounds) {
				value10_lb_has_trailing_zeros = ryu::divisible_by_pow5(scaled_mantissa_lb, q);
			} else {
				value10_ub -= ryu::divisible_by_pow5(scaled_mantissa_ub, q);
			}
		}
	} else {
		q = ryu::approximate_floor_log10_5_e(-exponent2_less_2);
		exponent10 = q + exponent2_less_2;
		i = -exponent2_less_2 - q;
		k = ryu::approximate_ceil_log2_5_e(i) - ryu::POWER_OF_5_32_NUMBER_OF_BITS;
		j = q - k;
		value10 = ryu::mul_pow5_and_shr_32(scaled_mantissa, i, j);
		value10_ub = ryu::mul_pow5_and_shr_32(scaled_mantissa_ub, i, j);
		value10_lb = ryu::mul_pow5_and_shr_32(scaled_mantissa_lb, i, j);

		if (q != 0 && (value10_ub - 1) / 10 <= value10_lb / 10) {
			j = q - 1 - (ryu::approximate_ceil_log2_5_e(i + 1) - ryu::POWER_OF_5_32_NUMBER_OF_BITS);
			last_removed_digit = ryu::mul_pow5_and_shr_32(scaled_mantissa, i + 1, j) % 10;
		}
		if (q <= 1) {
			value10_has_trailing_zeros = true;
			if (accept_bounds) {
				value10_lb_has_trailing_zeros = scaled_mantissa_lb_offset == 1;
			} else {
				--value10_ub;
			}
		} else if (q < 31) {
			value10_has_trailing_zeros = ryu::divisible_by_pow2(scaled_mantissa, q - 1);
		}
	}

	//Step 4: Find the shortest decimal representation in the interval of legal representations
	if (!(value10_lb_has_trailing_zeros || value10_has_trailing_zeros)) {
		while (value10_ub / 10 > value10_lb / 10) {
			last_removed_digit = value10 % 10;
			value10 /= 10;
			value10_ub /= 10;
			value10_lb /= 10;
			++exponent10;
		}
		significand10 = value10 + (value10 == value10_lb || last_removed_digit >= 5);
	} else {
		while (value10_ub / 10 > value10_lb / 10) {
			value10_lb_has_trailing_zeros &= value10_lb % 10 == 0;
			value10_has_trailing_zeros &= last_removed_digit == 0;
			last_removed_digit = value10 % 10;
			value10 /= 10;
			value10_ub /= 10;
			value10_lb /= 10;
			++exponent10;
		}

		if (value10_lb_has_trailing_zeros) {
			while (value10_lb % 10 == 0) {
				value10_has_trailing_zeros &= last_removed_digit == 0;
				last_removed_digit = value10 % 10;
				value10 /= 10;
				value10_ub /= 10;
				value10_lb /= 10;
				++exponent10;
			}
		}

		if (value10_has_trailing_zeros && last_removed_digit == 5 && value10 % 2 == 0) {
			last_removed_digit = 4;
		}
		significand10 = value10 + ((value10 == value10_lb && (!accept_bounds || !value10_lb_has_trailing_zeros)) || last_removed_digit >= 5);
	}

	//Step 5: Convert the decimal significand and exponent to a string
	s = ej::uint32_to_string_no_nul(s, significand10);
	if (exponent10 != 0) {
		s[0] = 'e';
		s = ej::int32_to_string_no_nul(s + 1, exponent10);
	}
	return s;
}

}