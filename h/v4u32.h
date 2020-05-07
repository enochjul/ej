//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

#ifdef __SSE2__

#include <emmintrin.h>
#ifdef __SSE4_1__
#include <smmintrin.h>
#endif

namespace ej {

struct v4u32 {
	__m128i Value;

	void load(const uint32_t v[4]) noexcept {
		Value = _mm_load_si128(reinterpret_cast<const __m128i *>(v));
	}

	void loadu(const uint32_t v[4]) noexcept {
		Value = _mm_loadu_si128(reinterpret_cast<const __m128i *>(v));
	}

	void store(uint32_t v[4]) noexcept {
		_mm_store_si128(reinterpret_cast<__m128i *>(v), Value);
	}

	void storeu(uint32_t v[4]) noexcept {
		_mm_storeu_si128(reinterpret_cast<__m128i *>(v), Value);
	}

	void set(uint32_t x, uint32_t y, uint32_t z, uint32_t w) noexcept {
		Value = _mm_setr_epi32(x, y, z, w);
	}

	void set1(uint32_t a) noexcept {
		Value = _mm_set1_epi32(a);
	}

	inline v4u32 &operator ^=(v4u32 a) noexcept;
	inline v4u32 &operator +=(v4u32 a) noexcept;
	inline v4u32 &operator *=(v4u32 a) noexcept;
};

inline v4u32 make_v4u32(uint32_t x, uint32_t y, uint32_t z, uint32_t w) noexcept {
	v4u32 r;
	r.set(x, y, z, w);
	return r;
}

inline v4u32 make1_v4u32(uint32_t a) noexcept {
	v4u32 r;
	r.set1(a);
	return r;
}

template <unsigned A0, unsigned A1, unsigned A2, unsigned A3>
inline v4u32 shuffle4u32(v4u32 a) noexcept {
	static_assert(A0 < 4 && A1 < 4 && A2 < 4 && A3 < 4);
	return v4u32{ _mm_shuffle_epi32(a.Value, A0 | (A1 << 2) | (A2 << 4) | (A3 << 6)) };
}

inline bool operator ==(v4u32 a, v4u32 b) noexcept {
	return _mm_movemask_epi8(_mm_cmpeq_epi32(a.Value, b.Value)) == 0xFFFF;
}

inline v4u32 operator &(v4u32 a, v4u32 b) noexcept {
	return v4u32{ _mm_and_si128(a.Value, b.Value) };
}

inline v4u32 operator |(v4u32 a, v4u32 b) noexcept {
	return v4u32{ _mm_or_si128(a.Value, b.Value) };
}

inline v4u32 operator ^(v4u32 a, v4u32 b) noexcept {
	return v4u32{ _mm_xor_si128(a.Value, b.Value) };
}

inline v4u32 &v4u32::operator ^=(v4u32 a) noexcept {
	*this = *this ^ a;
	return *this;
}

inline v4u32 operator +(v4u32 a, v4u32 b) noexcept {
	return v4u32{ _mm_add_epi32(a.Value, b.Value) };
}

inline v4u32 &v4u32::operator +=(v4u32 a) noexcept {
	*this = *this + a;
	return *this;
}

inline v4u32 operator -(v4u32 a, v4u32 b) noexcept {
	return v4u32{ _mm_sub_epi32(a.Value, b.Value) };
}

inline v4u32 operator *(v4u32 a, v4u32 b) noexcept {
#ifdef __SSE4_1__
	return v4u32{ _mm_mullo_epi32(a.Value, b.Value) };
#else
	auto r0 = _mm_mul_epu32(a.Value, b.Value);
	auto r1 = _mm_mul_epu32(_mm_srli_si128(a.Value, 4), _mm_srli_si128(b.Value, 4));
	return v4u32{ _mm_unpacklo_epi32(shuffle4u32<0, 2, 0, 2>(v4u32{r0}).Value, shuffle4u32<0, 2, 0, 2>(v4u32{r1}).Value) };
#endif
}

inline v4u32 &v4u32::operator *=(v4u32 a) noexcept {
	*this = *this * a;
	return *this;
}

template <unsigned N>
inline v4u32 shift_left(v4u32 a) noexcept {
	static_assert(N > 0 && N < 32);
	return v4u32{ _mm_slli_epi32(a.Value, N) };
}

template <unsigned N>
inline v4u32 shift_right(v4u32 a) noexcept {
	static_assert(N > 0 && N < 32);
	return v4u32{ _mm_srli_epi32(a.Value, N) };
}

template <unsigned N>
inline v4u32 rotate_left(v4u32 a) noexcept {
	return shift_left<N>(a) | shift_right<32 - N>(a);
}

template <unsigned N>
inline v4u32 rotate_right(v4u32 a) noexcept {
	return shift_right<N>(a) | shift_left<32 - N>(a);
}

}

#endif
