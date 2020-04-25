//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#ifdef __SSE__

#include <xmmintrin.h>

namespace ej {

template <unsigned A0, unsigned A1, unsigned A2, unsigned A3>
inline __m128 shuffle4f(__m128 value) noexcept {
	static_assert(A0 < 4 && A1 < 4 && A2 < 4 && A3 < 4);

	if constexpr (A0 == 0 && A1 == 0 && A2 == 1 && A3 == 1) {
		return _mm_unpacklo_ps(value, value);
	} else if constexpr (A0 == 0 && A1 == 1 && A2 == 0 && A3 == 1) {
		return _mm_movelh_ps(value, value);
	} else if constexpr (A0 == 0 && A1 == 1 && A2 == 2 && A3 == 3) {
		return value;
	} else if constexpr (A0 == 2 && A1 == 2 && A2 == 3 && A3 == 3) {
		return _mm_unpackhi_ps(value, value);
	} else if constexpr (A0 == 2 && A1 == 3 && A2 == 2 && A3 == 3) {
		return _mm_movehl_ps(value, value);
	} else {
		return _mm_shuffle_ps(value, value, A0 | (A1 << 2) | (A2 << 4) | (A3 << 6));
	}
}

struct v4f {
	__m128 Value;

	void set(float x, float y, float z, float w) noexcept {
		Value = _mm_setr_ps(x, y, z, w);
	}

	void set1(float v) noexcept {
		Value = _mm_set1_ps(v);
	}

	void setZero() noexcept {
		Value = _mm_setzero_ps();
	}

	void load(const float v[4]) noexcept {
		Value = _mm_load_ps(v);
	}

	void loadu(const float v[4]) noexcept {
		Value = _mm_loadu_ps(v);
	}

	void store(float v[4]) const noexcept {
		_mm_store_ps(v, Value);
	}

	void storeu(float v[4]) const noexcept {
		_mm_storeu_ps(v, Value);
	}

	template <unsigned A0, unsigned A1, unsigned A2, unsigned A3>
	void shuffle() noexcept {
		Value = shuffle4f<A0, A1, A2, A3>(Value);
	}

	template <unsigned I>
	float get() const noexcept {
		if constexpr (I == 0) {
			return _mm_cvtss_f32(Value);
		} else if constexpr (I == 1) {
			return _mm_cvtss_f32(shuffle4f<1, 0, 2, 3>(Value));
		} else if constexpr (I == 2) {
			return _mm_cvtss_f32(shuffle4f<2, 3, 2, 3>(Value));
		} else {
			static_assert(I == 3);
			return _mm_cvtss_f32(shuffle4f<3, 2, 1, 0>(Value));
		}
	}

	inline v4f &operator +=(v4f a) noexcept;
	inline v4f &operator -=(v4f a) noexcept;
	inline v4f &operator *=(v4f a) noexcept;
	inline v4f &operator /=(v4f a) noexcept;

	inline v4f &operator &=(v4f a) noexcept;
	inline v4f &operator |=(v4f a) noexcept;
	inline v4f &operator ^=(v4f a) noexcept;

	v4f andnot(v4f a) const noexcept {
		return v4f{ _mm_andnot_ps(Value, a.Value) };
	}

	v4f eq(v4f a) const noexcept {
		return v4f{ _mm_cmpeq_ps(Value, a.Value) };
	}
	v4f ne(v4f a) const noexcept {
		return v4f{ _mm_cmpneq_ps(Value, a.Value) };
	}
	v4f ge(v4f a) const noexcept {
		return v4f{ _mm_cmpge_ps(Value, a.Value) };
	}
	v4f gt(v4f a) const noexcept {
		return v4f{ _mm_cmpgt_ps(Value, a.Value) };
	}
	v4f le(v4f a) const noexcept {
		return v4f{ _mm_cmple_ps(Value, a.Value) };
	}
	v4f lt(v4f a) const noexcept {
		return v4f{ _mm_cmplt_ps(Value, a.Value) };
	}

	int movemask() const noexcept {
		return _mm_movemask_ps(Value);
	}
};

inline v4f make_v4f(float x, float y, float z, float w) noexcept {
	return v4f{ _mm_setr_ps(x, y, z, w) };
}

inline v4f make1_v4f(float v) noexcept {
	return v4f{ _mm_set1_ps(v) };
}

inline v4f make_zero_v4f() noexcept {
	return v4f{ _mm_setzero_ps() };
}

template <unsigned A0, unsigned A1, unsigned A2, unsigned A3>
inline v4f shuffle(v4f value) noexcept {
	return v4f{ shuffle4f<A0, A1, A2, A3>(value.Value) };
}

inline bool operator ==(v4f a, v4f b) noexcept {
	return _mm_movemask_ps(_mm_cmpeq_ps(a.Value, b.Value)) == 0xF;
}

inline bool operator !=(v4f a, v4f b) noexcept {
	return _mm_movemask_ps(_mm_cmpneq_ps(a.Value, b.Value)) != 0;
}

inline v4f operator +(v4f a, v4f b) noexcept {
	return v4f{ _mm_add_ps(a.Value, b.Value) };
}

inline v4f operator -(v4f a, v4f b) noexcept {
	return v4f{ _mm_sub_ps(a.Value, b.Value) };
}

inline v4f operator *(v4f a, v4f b) noexcept {
	return v4f{ _mm_mul_ps(a.Value, b.Value) };
}

inline v4f operator /(v4f a, v4f b) noexcept {
	return v4f{ _mm_div_ps(a.Value, b.Value) };
}

inline v4f operator &(v4f a, v4f b) noexcept {
	return v4f{ _mm_and_ps(a.Value, b.Value) };
}

inline v4f operator |(v4f a, v4f b) noexcept {
	return v4f{ _mm_or_ps(a.Value, b.Value) };
}

inline v4f operator ^(v4f a, v4f b) noexcept {
	return v4f{ _mm_xor_ps(a.Value, b.Value) };
}

inline v4f max(v4f a, v4f b) noexcept {
	return v4f{ _mm_max_ps(a.Value, b.Value) };
}

inline v4f min(v4f a, v4f b) noexcept {
	return v4f{ _mm_min_ps(a.Value, b.Value) };
}

inline v4f lerp(v4f l, v4f r, v4f t) noexcept {
	const v4f one = { 1.0f, 1.0f, 1.0f, 1.0f };
	return (one - t) * l + t * r;
}

inline v4f lerp(v4f l, v4f r, float t) noexcept {
	return lerp(l, r, make1_v4f(t));
}

inline v4f &v4f::operator +=(v4f a) noexcept {
	*this = *this + a;
	return *this;
}

inline v4f &v4f::operator -=(v4f a) noexcept {
	*this = *this - a;
	return *this;
}

inline v4f &v4f::operator *=(v4f a) noexcept {
	*this = *this * a;
	return *this;
}

inline v4f &v4f::operator /=(v4f a) noexcept {
	*this = *this / a;
	return *this;
}

inline v4f &v4f::operator &=(v4f a) noexcept {
	*this = *this & a;
	return *this;
}

inline v4f &v4f::operator |=(v4f a) noexcept {
	*this = *this | a;
	return *this;
}

inline v4f &v4f::operator ^=(v4f a) noexcept {
	*this = *this ^ a;
	return *this;
}

}

#endif
