//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>

namespace ej {

#ifdef __AVX2__
size_t mempcmp_avx2_unaligned(const void *a, const void *b, size_t n) noexcept;
size_t mempcmp_avx2_aligned(const void *a, const void *b, size_t n) noexcept;
#endif

#ifdef __SSE2__
size_t mempcmp_sse2_unaligned(const void *a, const void *b, size_t n) noexcept;
size_t mempcmp_sse2_aligned(const void *a, const void *b, size_t n) noexcept;
#endif

size_t mempcmp_uint64_unaligned(const void *a, const void *b, size_t n) noexcept;
size_t mempcmp_uint64_aligned(const void *a, const void *b, size_t n) noexcept;

inline size_t mempcmp(const void *a, const void *b, size_t n) noexcept {
#ifdef __AVX2__
	return mempcmp_avx2_aligned(a, b, n);
#elif defined(__SSE2__)
	return mempcmp_sse2_aligned(a, b, n);
#else
	return mempcmp_uint64_aligned(a, b, n);
#endif
}

}
