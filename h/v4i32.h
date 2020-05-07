//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

#include "v4f.h"
#include "v4u32.h"

#ifdef __SSE2__
#include <emmintrin.h>

#ifdef __SSE4_1__
#include <smmintrin.h>
#endif

namespace ej {

template <unsigned A0, unsigned A1, unsigned A2, unsigned A3>
inline __m128i shuffle4i32(__m128i a) noexcept {
	static_assert(A0 < 4 && A1 < 4 && A2 < 4 && A3 < 4);

	if constexpr (A0 == 0 && A1 == 0 && A2 == 1 && A3 == 1) {
		return _mm_unpacklo_epi32(a, a);
	} else if constexpr (A0 == 0 && A1 == 1 && A2 == 2 && A3 == 3) {
		return a;
	} else if constexpr (A0 == 2 && A1 == 2 && A2 == 3 && A3 == 3) {
		return _mm_unpackhi_epi32(a, a);
	} else {
		return _mm_shuffle_epi32(a, A0 | (A1 << 2) | (A2 << 4) | (A3 << 6));
	}
}

struct v4i32 {
	__m128i Value;

	void set(int32_t x, int32_t y, int32_t z, int32_t w) noexcept {
		Value = _mm_setr_epi32(x, y, z, w);
	}

	void set1(int32_t a) noexcept {
		Value = _mm_set1_epi32(a);
	}

	void setZero() noexcept {
		Value = _mm_setzero_si128();
	}

	void load(const int32_t v[4]) noexcept {
		Value = _mm_load_si128(reinterpret_cast<const __m128i *>(v));
	}

	void loadu(const int32_t v[4]) noexcept {
		Value = _mm_loadu_si128(reinterpret_cast<const __m128i *>(v));
	}

	void store(int32_t v[4]) noexcept {
		_mm_store_si128(reinterpret_cast<__m128i *>(v), Value);
	}

	void storeu(int32_t v[4]) noexcept {
		_mm_storeu_si128(reinterpret_cast<__m128i *>(v), Value);
	}

	template <unsigned A0, unsigned A1, unsigned A2, unsigned A3>
	void shuffle() noexcept {
		Value = shuffle4i32<A0, A1, A2, A3>(Value);
	}

	template <unsigned I>
	int get() const noexcept {
		if constexpr (I == 0) {
			return _mm_cvtsi128_si32(Value);
		} else if constexpr (I == 1) {
			return _mm_cvtsi128_si32(shuffle4i32<1, 0, 2, 3>(Value));
		} else if constexpr (I == 2) {
			return _mm_cvtsi128_si32(shuffle4i32<2, 2, 3, 3>(Value));
		} else {
			static_assert(I == 3);
			return _mm_cvtsi128_si32(shuffle4i32<3, 2, 1, 0>(Value));
		}
	}

	inline v4i32 &operator +=(v4i32 a) noexcept;
	inline v4i32 &operator -=(v4i32 a) noexcept;
	inline v4i32 &operator *=(v4i32 a) noexcept;

	inline v4i32 &operator <<=(int count) noexcept;
	inline v4i32 &operator >>=(int count) noexcept;

	inline v4i32 &operator &=(v4i32 a) noexcept;
	inline v4i32 &operator |=(v4i32 a) noexcept;
	inline v4i32 &operator ^=(v4i32 a) noexcept;

	v4i32 andnot(v4i32 a) const noexcept {
		return v4i32{ _mm_andnot_si128(Value, a.Value) };
	}

	inline v4i32 ror(int count) const noexcept;
	inline v4i32 rol(int count) const noexcept;
};

inline v4i32 make_v4i32(int32_t x, int32_t y, int32_t z, int32_t w) noexcept {
	v4i32 r;
	r.set(x, y, z, w);
	return r;
}

inline v4i32 make1_v4i32(int32_t a) noexcept {
	v4i32 r;
	r.set1(a);
	return r;
}

inline v4i32 make_zero_v4i32() noexcept {
	v4i32 r;
	r.setZero();
	return r;
}

inline v4f make_v4f(v4i32 a) noexcept {
	return v4f{ _mm_cvtepi32_ps(a.Value) };
}

inline v4i32 make_v4i32(v4u32 a) noexcept {
	return v4i32{ a.Value };
}

inline v4u32 make_v4u32(v4i32 a) noexcept {
	return v4u32{ a.Value };
}

inline bool operator ==(v4i32 a, v4i32 b) noexcept {
	return _mm_movemask_epi8(_mm_cmpeq_epi32(a.Value, b.Value)) == 0xFFFF;
}

inline v4i32 operator +(v4i32 a, v4i32 b) noexcept {
	return v4i32{ _mm_add_epi32(a.Value, b.Value) };
}

inline v4i32 operator -(v4i32 a, v4i32 b) noexcept {
	return v4i32{ _mm_sub_epi32(a.Value, b.Value) };
}

inline v4i32 operator *(v4i32 a, v4i32 b) noexcept {
#ifdef __SSE4_1__
	return v4i32{ _mm_mullo_epi32(a.Value, b.Value) };
#else
	auto r0 = _mm_mul_epu32(a.Value, b.Value);
	auto r1 = _mm_mul_epu32(_mm_srli_si128(a.Value, 4), _mm_srli_si128(b.Value, 4));
	return v4i32{ _mm_unpacklo_epi32(shuffle4i32<0, 2, 0, 2>(v4i32{r0}).Value, shuffle4i32<0, 2, 0, 2>(v4i32{r1}).Value) };
#endif
}

inline v4i32 operator &(v4i32 a, v4i32 b) noexcept {
	return v4i32{ _mm_and_si128(a.Value, b.Value) };
}

inline v4i32 operator |(v4i32 a, v4i32 b) noexcept {
	return v4i32{ _mm_or_si128(a.Value, b.Value) };
}

inline v4i32 operator ^(v4i32 a, v4i32 b) noexcept {
	return v4i32{ _mm_xor_si128(a.Value, b.Value) };
}

inline v4i32 &v4i32::operator +=(v4i32 a) noexcept {
	*this = *this + a;
	return *this;
}

inline v4i32 &v4i32::operator *=(v4i32 a) noexcept {
	*this = *this * a;
	return *this;
}

inline v4i32 &v4i32::operator ^=(v4i32 a) noexcept {
	*this = *this ^ a;
	return *this;
}

}

#endif
