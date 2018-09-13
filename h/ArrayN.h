//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_ARRAY_N_H
#define EJ_ARRAY_N_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __AVX__
#include <immintrin.h>
#endif

#ifdef __SSE__
#include <xmmintrin.h>
#endif

namespace ej {

//A fixed size array of objects
template <
	typename T,
	//Size of the array
	size_t N,
	//Alignment of the array
	size_t ALIGNMENT = alignof(T)>
struct ArrayN {
	static_assert(N > 0 && (ALIGNMENT % alignof(T)) == 0);

	typedef T value_type;
	typedef value_type *iterator;
	typedef const value_type *const_iterator;
	typedef size_t size_type;

	alignas(ALIGNMENT) value_type Data[N];

	constexpr size_type size() const noexcept {
		return N;
	}

	constexpr bool empty() const noexcept {
		return N == 0;
	}

	constexpr iterator begin() noexcept {
		return Data;
	}

	constexpr const_iterator begin() const noexcept {
		return Data;
	}

	constexpr const_iterator cbegin() const noexcept {
		return Data;
	}

	constexpr iterator end() noexcept {
		return Data + N;
	}

	constexpr const_iterator end() const noexcept {
		return Data + N;
	}

	constexpr const_iterator cend() const noexcept {
		return Data + N;
	}

	constexpr T &at(size_type index) noexcept {
		if (index < N) {
			return Data[index];
		} else {
			abort();
		}
	}

	constexpr const T &at(size_type index) const noexcept {
		if (index < N) {
			return Data[index];
		} else {
			abort();
		}
	}

	constexpr T &operator [](size_type index) noexcept {
		return Data[index];
	}

	constexpr const T &operator [](size_type index) const noexcept {
		return Data[index];
	}

	constexpr T *data() noexcept {
		return Data;
	}

	constexpr const T *data() const noexcept {
		return Data;
	}
};

template <typename T1, size_t N1, size_t ALIGNMENT1, typename T2, size_t N2, size_t ALIGNMENT2>
bool equal(const ArrayN<T1, N1, ALIGNMENT1> &a, const ArrayN<T2, N2, ALIGNMENT2> &b) noexcept {
	if constexpr (N1 == N2) {
		constexpr size_t unroll_factor = 4;
		constexpr size_t aligned_n = (N1 / unroll_factor) * unroll_factor;
		const T1 *a_aligned_end = a.Data + aligned_n;
		const T1 *a_iter;
		const T2 *b_iter;

		for (a_iter = a.Data, b_iter = b.Data; a_iter < a_aligned_end; a_iter += unroll_factor, b_iter += unroll_factor) {
			if (a_iter[0] != b_iter[0]) {
				return false;
			}
			if (a_iter[1] != b_iter[1]) {
				return false;
			}
			if (a_iter[2] != b_iter[2]) {
				return false;
			}
			if (a_iter[3] != b_iter[3]) {
				return false;
			}
		}

		constexpr size_t remainder = N1 % unroll_factor;
		auto a_end = a.end();
		auto b_end = b.end();
		if constexpr (remainder == 3) {
			if (a_end[-3] != b_end[-3]) {
				return false;
			}
		}
		if constexpr (remainder == 2) {
			if (a_end[-2] != b_end[-2]) {
				return false;
			}
		}
		if constexpr (remainder == 1) {
			return a_end[-1] == b_end[-1];
		}
		return true;
	} else {
		return false;
	}
}

template <size_t N, size_t ALIGNMENT1, size_t ALIGNMENT2>
bool equal(const ArrayN<float, N, ALIGNMENT1> &a, const ArrayN<float, N, ALIGNMENT2> &b) noexcept {
#ifdef __AVX__
	if constexpr (N >= 8 && ALIGNMENT1 >= 32 && ALIGNMENT2 >= 32) {
		constexpr size_t unroll_factor = 32;
		constexpr size_t aligned_n = (N / unroll_factor) * unroll_factor;
		const float *a_aligned_end = a.Data + aligned_n;
		const float *a_iter;
		const float *b_iter;

		for (a_iter = a.Data, b_iter = b.Data; a_iter < a_aligned_end; a_iter += unroll_factor, b_iter += unroll_factor) {
			if (_mm256_movemask_ps(_mm256_cmp_ps(_mm256_load_ps(a_iter), _mm256_load_ps(b_iter), _CMP_NEQ_UQ)) != 0) {
				return false;
			}
			if (_mm256_movemask_ps(_mm256_cmp_ps(_mm256_load_ps(a_iter + 8), _mm256_load_ps(b_iter + 8), _CMP_NEQ_UQ)) != 0) {
				return false;
			}
			if (_mm256_movemask_ps(_mm256_cmp_ps(_mm256_load_ps(a_iter + 16), _mm256_load_ps(b_iter + 16), _CMP_NEQ_UQ)) != 0) {
				return false;
			}
			if (_mm256_movemask_ps(_mm256_cmp_ps(_mm256_load_ps(a_iter + 24), _mm256_load_ps(b_iter + 24), _CMP_NEQ_UQ)) != 0) {
				return false;
			}
		}

		size_t remainder = N % unroll_factor;
		if (remainder >= 16) {
			if (_mm256_movemask_ps(_mm256_cmp_ps(_mm256_load_ps(a_iter), _mm256_load_ps(b_iter), _CMP_NEQ_UQ)) != 0) {
				return false;
			}
			if (_mm256_movemask_ps(_mm256_cmp_ps(_mm256_load_ps(a_iter + 8), _mm256_load_ps(b_iter + 8), _CMP_NEQ_UQ)) != 0) {
				return false;
			}
			a_iter += 16, b_iter += 16, remainder -= 16;
		}
		if (remainder >= 8) {
			if (_mm256_movemask_ps(_mm256_cmp_ps(_mm256_load_ps(a_iter), _mm256_load_ps(b_iter), _CMP_NEQ_UQ)) != 0) {
				return false;
			}
			a_iter += 8, b_iter += 8, remainder -= 8;
		}
		if (remainder >= 4) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_iter), _mm_load_ps(b_iter))) != 0) {
				return false;
			}
			a_iter += 4, b_iter += 4, remainder -= 4;
		}
		switch (remainder) {
		case 3:
			if (a_iter[0] != b_iter[0]) {
				return false;
			}
			if (a_iter[1] != b_iter[1]) {
				return false;
			}
			return a_iter[2] == b_iter[2];

		case 2:
			if (a_iter[0] != b_iter[0]) {
				return false;
			}
			return a_iter[1] == b_iter[1];

		case 1:
			return a_iter[0] == b_iter[0];
		}
		return true;
	} else
#endif
#ifdef __SSE__
	if constexpr (N >= 4 && ALIGNMENT1 >= 16 && ALIGNMENT2 >= 16) {
		constexpr size_t unroll_factor = 16;
		constexpr size_t aligned_n = (N / unroll_factor) * unroll_factor;
		const float *a_aligned_end = a.Data + aligned_n;
		const float *a_iter;
		const float *b_iter;

		for (a_iter = a.Data, b_iter = b.Data; a_iter < a_aligned_end; a_iter += unroll_factor, b_iter += unroll_factor) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_iter), _mm_load_ps(b_iter))) != 0) {
				return false;
			}
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_iter + 4), _mm_load_ps(b_iter + 4))) != 0) {
				return false;
			}
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_iter + 8), _mm_load_ps(b_iter + 8))) != 0) {
				return false;
			}
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_iter + 12), _mm_load_ps(b_iter + 12))) != 0) {
				return false;
			}
		}

		constexpr size_t remainder = N % unroll_factor;
		auto a_end = a.end();
		auto b_end = b.end();
		if constexpr (remainder == 15) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 15), _mm_load_ps(b_end - 15))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 14) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 14), _mm_load_ps(b_end - 14))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 13) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 13), _mm_load_ps(b_end - 13))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 12) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 12), _mm_load_ps(b_end - 12))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 15 || remainder == 11) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 11), _mm_load_ps(b_end - 11))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 14 || remainder == 10) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 10), _mm_load_ps(b_end - 10))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 13 || remainder == 9) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 9), _mm_load_ps(b_end - 9))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 12 || remainder == 8) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 8), _mm_load_ps(b_end - 8))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 15 || remainder == 11 || remainder == 7) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 7), _mm_load_ps(b_end - 7))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 14 || remainder == 10 || remainder == 6) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 6), _mm_load_ps(b_end - 6))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 13 || remainder == 9 || remainder == 5) {
			if (_mm_movemask_ps(_mm_cmpneq_ps(_mm_load_ps(a_end - 5), _mm_load_ps(b_end - 5))) != 0) {
				return false;
			}
		}
		if constexpr (remainder == 12 || remainder == 8 || remainder == 4) {
			return _mm_movemask_ps(_mm_cmpeq_ps(_mm_load_ps(a_end - 4), _mm_load_ps(b_end - 4))) != 0;
		}
		if constexpr ((remainder % 4) >= 3) {
			if (a_end[-3] != b_end[-3]) {
				return false;
			}
		}
		if constexpr ((remainder % 4) >= 2) {
			if (a_end[-2] != b_end[-2]) {
				return false;
			}
		}
		if constexpr ((remainder % 4) >= 1) {
			return a_end[-1] == b_end[-1];
		}
		return true;
	} else
#endif
	if constexpr (N >= 1) {
		constexpr size_t unroll_factor = 4;
		constexpr size_t aligned_n = (N / unroll_factor) * unroll_factor;
		const float *a_aligned_end = a.Data + aligned_n;
		const float *a_iter;
		const float *b_iter;

		for (a_iter = a.Data, b_iter = b.Data; a_iter < a_aligned_end; a_iter += unroll_factor, b_iter += unroll_factor) {
			if (a_iter[0] != b_iter[0]) {
				return false;
			}
			if (a_iter[1] != b_iter[1]) {
				return false;
			}
			if (a_iter[2] != b_iter[2]) {
				return false;
			}
			if (a_iter[3] != b_iter[3]) {
				return false;
			}
		}

		constexpr size_t remainder = N % unroll_factor;
		auto a_end = a.end();
		auto b_end = b.end();
		if constexpr (remainder >= 3) {
			if (a_end[-3] != b_end[-3]) {
				return false;
			}
		}
		if constexpr (remainder >= 2) {
			if (a_end[-2] != b_end[-2]) {
				return false;
			}
		}
		if constexpr (remainder >= 1) {
			return a_end[-1] == b_end[-1];
		}
		return true;
	} else {
		return false;
	}
}

template <typename T1, size_t N1, size_t ALIGNMENT1, typename T2, size_t N2, size_t ALIGNMENT2>
inline bool operator ==(const ArrayN<T1, N1, ALIGNMENT1> &a, const ArrayN<T2, N2, ALIGNMENT2> &b) noexcept {
	return equal(a, b);
}

}

#endif
