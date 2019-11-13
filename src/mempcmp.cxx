//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <emmintrin.h>
#include <immintrin.h>
#include <x86intrin.h>

#include "mempcmp.h"

namespace ej {

#ifdef __AVX2__
size_t mempcmp_avx2_unaligned(const void *a, const void *b, size_t n) noexcept {
	__m256i a256, b256;
	__m128i a128, b128;
	uint64_t a64, b64, byte_mask64;
	uint32_t a32, b32, byte_mask32;
	uint32_t mask, pos;
	uint16_t a16, b16;

	const char *ca = static_cast<const char *>(a);
	uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
	size_t aligned_n = n & ~static_cast<size_t>(127);
	for (const char *aligned_ca_end = ca + aligned_n; ca < aligned_ca_end; ca += 128) {
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 32));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 32));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_offset_32;
		}
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 64));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 64));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_offset_64;
		}
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 96));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 96));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_offset_96;
		}
	}

	n &= static_cast<size_t>(127);
	switch (n) {
	case 127:
	case 126:
	case 125:
	case 124:
	case 123:
	case 122:
	case 121:
	case 120:
	case 119:
	case 118:
	case 117:
	case 116:
	case 115:
	case 114:
	case 113:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 95:
	case 94:
	case 93:
	case 92:
	case 91:
	case 90:
	case 89:
	case 88:
	case 87:
	case 86:
	case 85:
	case 84:
	case 83:
	case 82:
	case 81:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 63:
	case 62:
	case 61:
	case 60:
	case 59:
	case 58:
	case 57:
	case 56:
	case 55:
	case 54:
	case 53:
	case 52:
	case 51:
	case 50:
	case 49:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
		ca += (n & 31) - 32;
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 112:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 80:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 48:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 16:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 111:
	case 110:
	case 109:
	case 108:
	case 107:
	case 106:
	case 105:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 79:
	case 78:
	case 77:
	case 76:
	case 75:
	case 74:
	case 73:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 47:
	case 46:
	case 45:
	case 44:
	case 43:
	case 42:
	case 41:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
		ca += (n & 15) - 16;
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 104:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 72:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 40:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 8:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 103:
	case 102:
	case 101:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 71:
	case 70:
	case 69:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 39:
	case 38:
	case 37:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
		ca += (n & 7) - 8;
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 100:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 68:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 36:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 4:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 99:
	case 98:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 67:
	case 66:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 35:
	case 34:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 97:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 65:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 33:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 1:
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 96:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 64:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
	case 32:
		a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
		b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
		mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
		if (mask != 0) {
			goto run_tzcnt_avx;
		}
		ca += 32;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 31:
	case 30:
	case 29:
	case 28:
	case 27:
	case 26:
	case 25:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		ca += (n & 15) - 16;
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 24:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 23:
	case 22:
	case 21:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		ca += (n & 7) - 8;
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 20:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 19:
	case 18:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 17:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 15:
	case 14:
	case 13:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		ca += (n & 7) - 8;
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 12:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 11:
	case 10:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 9:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 7:
	case 6:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 5:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 3:
		memcpy(&a16, ca + 0, 2);
		memcpy(&b16, (ca + d) + 0, 2);
		byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
		if (byte_mask32 != 0) {
			goto run_tzcnt_16;
		}
		ca += 2;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 2:
		memcpy(&a16, ca + 0, 2);
		memcpy(&b16, (ca + d) + 0, 2);
		byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
		if (byte_mask32 != 0) {
			goto run_tzcnt_16;
		}
		ca += 2;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 0:
	default:
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	}
run_tzcnt_offset_96:
	ca += 32;
run_tzcnt_offset_64:
	ca += 32;
run_tzcnt_offset_32:
	ca += 32;
run_tzcnt_avx:
	pos = static_cast<uint32_t>(__bsfd(mask));
run_tzcnt_sse:
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_64:
	pos = static_cast<uint32_t>(__bsfq(byte_mask64)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_32:
run_tzcnt_16:
	pos = static_cast<uint32_t>(__bsfd(byte_mask32)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
}

size_t mempcmp_avx2_aligned(const void *a, const void *b, size_t n) noexcept {
	__m256i a256, b256;
	__m128i a128, b128;
	uint64_t a64, b64, byte_mask64;
	uint32_t a32, b32, byte_mask32;
	uint32_t mask, pos;
	uint16_t a16, b16;

	const char *ca = static_cast<const char *>(a);
	if (n < 128) {
		uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
		switch (n) {
		case 127:
		case 126:
		case 125:
		case 124:
		case 123:
		case 122:
		case 121:
		case 120:
		case 119:
		case 118:
		case 117:
		case 116:
		case 115:
		case 114:
		case 113:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 95:
		case 94:
		case 93:
		case 92:
		case 91:
		case 90:
		case 89:
		case 88:
		case 87:
		case 86:
		case 85:
		case 84:
		case 83:
		case 82:
		case 81:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 63:
		case 62:
		case 61:
		case 60:
		case 59:
		case 58:
		case 57:
		case 56:
		case 55:
		case 54:
		case 53:
		case 52:
		case 51:
		case 50:
		case 49:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
			ca += (n & 31) - 32;
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 112:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 80:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 48:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 16:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 111:
		case 110:
		case 109:
		case 108:
		case 107:
		case 106:
		case 105:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 79:
		case 78:
		case 77:
		case 76:
		case 75:
		case 74:
		case 73:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 47:
		case 46:
		case 45:
		case 44:
		case 43:
		case 42:
		case 41:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
			ca += (n & 15) - 16;
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 104:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 72:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 40:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 8:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 103:
		case 102:
		case 101:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 71:
		case 70:
		case 69:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 39:
		case 38:
		case 37:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
			ca += (n & 7) - 8;
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 100:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 68:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 36:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 4:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 99:
		case 98:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 67:
		case 66:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 35:
		case 34:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 97:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 65:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 33:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 1:
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 96:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 64:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
		case 32:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			ca += 32;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 31:
		case 30:
		case 29:
		case 28:
		case 27:
		case 26:
		case 25:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			ca += (n & 15) - 16;
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 24:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 23:
		case 22:
		case 21:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			ca += (n & 7) - 8;
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 20:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 19:
		case 18:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 17:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 15:
		case 14:
		case 13:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			ca += (n & 7) - 8;
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 12:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 11:
		case 10:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 9:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 7:
		case 6:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 5:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 3:
			memcpy(&a16, ca + 0, 2);
			memcpy(&b16, (ca + d) + 0, 2);
			byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
			if (byte_mask32 != 0) {
				goto run_tzcnt_16;
			}
			ca += 2;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 2:
			memcpy(&a16, ca + 0, 2);
			memcpy(&b16, (ca + d) + 0, 2);
			byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
			if (byte_mask32 != 0) {
				goto run_tzcnt_16;
			}
			ca += 2;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 0:
		default:
			return 0;
		}
	} else {
		const char *cb = static_cast<const char *>(b);
		uint32_t initial_offset = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a)) & 31;
		switch (initial_offset) {
		case 31:
			if (*ca != *cb) {
				return 0;
			}
			break;
		case 30:
		case 29:
		case 28:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, cb + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			break;
		case 27:
		case 26:
		case 25:
		case 24:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, cb + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			break;
		case 23:
		case 22:
		case 21:
		case 20:
		case 19:
		case 18:
		case 17:
		case 16:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(cb + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			break;
		case 15:
		case 14:
		case 13:
		case 12:
		case 11:
		case 10:
		case 9:
		case 8:
		case 7:
		case 6:
		case 5:
		case 4:
		case 3:
		case 2:
		case 1:
			a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
			b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(cb + 0));
			mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
			if (mask != 0) {
				goto run_tzcnt_avx;
			}
			break;
		case 0:
			break;
		}
		uint32_t initial_n = (-initial_offset) & 31;
		uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
		ca += initial_n;
		n -= initial_n;
		size_t aligned_n = n & ~static_cast<size_t>(127);
		n &= static_cast<size_t>(127);
		if ((reinterpret_cast<uintptr_t>(ca + d) & 31) != 0) {
			for (const char *aligned_ca_end = ca + aligned_n; ca < aligned_ca_end; ca += 128) {
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 32));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 32));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_offset_32;
				}
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 64));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 64));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_offset_64;
				}
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 96));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 96));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_offset_96;
				}
			}
			switch (n) {
			case 127:
			case 126:
			case 125:
			case 124:
			case 123:
			case 122:
			case 121:
			case 120:
			case 119:
			case 118:
			case 117:
			case 116:
			case 115:
			case 114:
			case 113:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 95:
			case 94:
			case 93:
			case 92:
			case 91:
			case 90:
			case 89:
			case 88:
			case 87:
			case 86:
			case 85:
			case 84:
			case 83:
			case 82:
			case 81:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 63:
			case 62:
			case 61:
			case 60:
			case 59:
			case 58:
			case 57:
			case 56:
			case 55:
			case 54:
			case 53:
			case 52:
			case 51:
			case 50:
			case 49:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
				ca += (n & 31) - 32;
				a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 112:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 80:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 48:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 16:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 111:
			case 110:
			case 109:
			case 108:
			case 107:
			case 106:
			case 105:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 79:
			case 78:
			case 77:
			case 76:
			case 75:
			case 74:
			case 73:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 47:
			case 46:
			case 45:
			case 44:
			case 43:
			case 42:
			case 41:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
				ca += (n & 15) - 16;
				a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 104:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 72:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 40:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 8:
				memcpy(&a64, ca + 0, 8);
				memcpy(&b64, (ca + d) + 0, 8);
				byte_mask64 = a64 ^ b64;
				if (byte_mask64 != 0) {
					goto run_tzcnt_64;
				}
				ca += 8;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 103:
			case 102:
			case 101:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 71:
			case 70:
			case 69:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 39:
			case 38:
			case 37:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 7:
			case 6:
			case 5:
				ca += (n & 7) - 8;
				memcpy(&a64, ca + 0, 8);
				memcpy(&b64, (ca + d) + 0, 8);
				byte_mask64 = a64 ^ b64;
				if (byte_mask64 != 0) {
					goto run_tzcnt_64;
				}
				ca += 8;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 100:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 68:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 36:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 4:
				memcpy(&a32, ca + 0, 4);
				memcpy(&b32, (ca + d) + 0, 4);
				byte_mask32 = a32 ^ b32;
				if (byte_mask32 != 0) {
					goto run_tzcnt_32;
				}
				ca += 4;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 99:
			case 98:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 67:
			case 66:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 35:
			case 34:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 3:
			case 2:
				ca += (n & 3) - 4;
				memcpy(&a32, ca + 0, 4);
				memcpy(&b32, (ca + d) + 0, 4);
				byte_mask32 = a32 ^ b32;
				if (byte_mask32 != 0) {
					goto run_tzcnt_32;
				}
				ca += 4;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 97:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 65:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 33:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 1:
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
			case 96:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 64:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 32:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 0:
			default:
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			}
		} else {
			for (const char *aligned_ca_end = ca + aligned_n; ca < aligned_ca_end; ca += 128) {
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 32));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 32));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_offset_32;
				}
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 64));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 64));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_offset_64;
				}
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 96));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 96));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_offset_96;
				}
			}
			switch (n) {
			case 127:
			case 126:
			case 125:
			case 124:
			case 123:
			case 122:
			case 121:
			case 120:
			case 119:
			case 118:
			case 117:
			case 116:
			case 115:
			case 114:
			case 113:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 95:
			case 94:
			case 93:
			case 92:
			case 91:
			case 90:
			case 89:
			case 88:
			case 87:
			case 86:
			case 85:
			case 84:
			case 83:
			case 82:
			case 81:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 63:
			case 62:
			case 61:
			case 60:
			case 59:
			case 58:
			case 57:
			case 56:
			case 55:
			case 54:
			case 53:
			case 52:
			case 51:
			case 50:
			case 49:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
				ca += (n & 31) - 32;
				a256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 112:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 80:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 48:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 16:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 111:
			case 110:
			case 109:
			case 108:
			case 107:
			case 106:
			case 105:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 79:
			case 78:
			case 77:
			case 76:
			case 75:
			case 74:
			case 73:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 47:
			case 46:
			case 45:
			case 44:
			case 43:
			case 42:
			case 41:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
				ca += (n & 15) - 16;
				a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 104:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 72:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 40:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 8:
				memcpy(&a64, ca + 0, 8);
				memcpy(&b64, (ca + d) + 0, 8);
				byte_mask64 = a64 ^ b64;
				if (byte_mask64 != 0) {
					goto run_tzcnt_64;
				}
				ca += 8;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 103:
			case 102:
			case 101:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 71:
			case 70:
			case 69:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 39:
			case 38:
			case 37:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 7:
			case 6:
			case 5:
				ca += (n & 7) - 8;
				memcpy(&a64, ca + 0, 8);
				memcpy(&b64, (ca + d) + 0, 8);
				byte_mask64 = a64 ^ b64;
				if (byte_mask64 != 0) {
					goto run_tzcnt_64;
				}
				ca += 8;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 100:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 68:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 36:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 4:
				memcpy(&a32, ca + 0, 4);
				memcpy(&b32, (ca + d) + 0, 4);
				byte_mask32 = a32 ^ b32;
				if (byte_mask32 != 0) {
					goto run_tzcnt_32;
				}
				ca += 4;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 99:
			case 98:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 67:
			case 66:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 35:
			case 34:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 3:
			case 2:
				ca += (n & 3) - 4;
				memcpy(&a32, ca + 0, 4);
				memcpy(&b32, (ca + d) + 0, 4);
				byte_mask32 = a32 ^ b32;
				if (byte_mask32 != 0) {
					goto run_tzcnt_32;
				}
				ca += 4;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 97:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 65:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 33:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 1:
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
			case 96:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 64:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
			case 32:
				a256 = _mm256_load_si256(reinterpret_cast<const __m256i *>(ca + 0));
				b256 = _mm256_load_si256(reinterpret_cast<const __m256i *>((ca + d) + 0));
				mask = ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a256, b256));
				if (mask != 0) {
					goto run_tzcnt_avx;
				}
				ca += 32;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 0:
			default:
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			}
		}
	}
run_tzcnt_offset_96:
	ca += 32;
run_tzcnt_offset_64:
	ca += 32;
run_tzcnt_offset_32:
	ca += 32;
run_tzcnt_avx:
	pos = static_cast<uint32_t>(__bsfd(mask));
run_tzcnt_sse:
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_64:
	pos = static_cast<uint32_t>(__bsfq(byte_mask64)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_32:
run_tzcnt_16:
	pos = static_cast<uint32_t>(__bsfd(byte_mask32)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
}
#endif

#ifdef __SSE2__
size_t mempcmp_sse2_unaligned(const void *a, const void *b, size_t n) noexcept {
	__m128i a128, b128;
	uint64_t a64, b64, byte_mask64;
	uint32_t a32, b32, byte_mask32;
	uint32_t mask, pos;
	uint16_t a16, b16;

	const char *ca = static_cast<const char *>(a);
	uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
	size_t aligned_n = n & ~static_cast<size_t>(127);
	for (const char *aligned_ca_end = ca + aligned_n; ca < aligned_ca_end; ca += 128) {
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 16));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 16));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_offset_16;
		}
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 32));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 32));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_offset_32;
		}
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 48));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 48));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_offset_48;
		}
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 64));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 64));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_offset_64;
		}
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 80));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 80));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_offset_80;
		}
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 96));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 96));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_offset_96;
		}
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 112));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 112));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_offset_112;
		}
	}

	n &= static_cast<size_t>(127);
	switch (n) {
	case 127:
	case 126:
	case 125:
	case 124:
	case 123:
	case 122:
	case 121:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 111:
	case 110:
	case 109:
	case 108:
	case 107:
	case 106:
	case 105:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 95:
	case 94:
	case 93:
	case 92:
	case 91:
	case 90:
	case 89:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 79:
	case 78:
	case 77:
	case 76:
	case 75:
	case 74:
	case 73:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 63:
	case 62:
	case 61:
	case 60:
	case 59:
	case 58:
	case 57:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 47:
	case 46:
	case 45:
	case 44:
	case 43:
	case 42:
	case 41:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 31:
	case 30:
	case 29:
	case 28:
	case 27:
	case 26:
	case 25:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		ca += (n & 15) - 16;
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 120:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 104:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 88:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 72:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 56:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 40:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 24:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 8:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 119:
	case 118:
	case 117:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 103:
	case 102:
	case 101:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 87:
	case 86:
	case 85:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 71:
	case 70:
	case 69:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 55:
	case 54:
	case 53:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 39:
	case 38:
	case 37:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 23:
	case 22:
	case 21:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		ca += (n & 7) - 8;
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 116:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 100:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 84:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 68:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 52:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 36:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 20:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 4:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 115:
	case 114:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 99:
	case 98:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 83:
	case 82:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 67:
	case 66:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 51:
	case 50:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 35:
	case 34:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 19:
	case 18:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 113:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 97:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 81:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 65:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 49:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 33:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 17:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 1:
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 112:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 96:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 80:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 64:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 48:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 32:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
	case 16:
		a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
		b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
		mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
		pos = static_cast<uint32_t>(__bsfd(mask));
		if (pos < 16) {
			goto run_tzcnt_sse;
		}
		ca += 16;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 15:
	case 14:
	case 13:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		ca += (n & 7) - 8;
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 12:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 11:
	case 10:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 9:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 7:
	case 6:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 5:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 3:
		memcpy(&a16, ca + 0, 2);
		memcpy(&b16, (ca + d) + 0, 2);
		byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
		if (byte_mask32 != 0) {
			goto run_tzcnt_16;
		}
		ca += 2;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 2:
		memcpy(&a16, ca + 0, 2);
		memcpy(&b16, (ca + d) + 0, 2);
		byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
		if (byte_mask32 != 0) {
			goto run_tzcnt_16;
		}
		ca += 2;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 0:
	default:
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	}
run_tzcnt_offset_112:
	ca += 16;
run_tzcnt_offset_96:
	ca += 16;
run_tzcnt_offset_80:
	ca += 16;
run_tzcnt_offset_64:
	ca += 16;
run_tzcnt_offset_48:
	ca += 16;
run_tzcnt_offset_32:
	ca += 16;
run_tzcnt_offset_16:
	ca += 16;
run_tzcnt_sse:
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_64:
	pos = static_cast<uint32_t>(__bsfq(byte_mask64)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_32:
run_tzcnt_16:
	pos = static_cast<uint32_t>(__bsfd(byte_mask32)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
}

size_t mempcmp_sse2_aligned(const void *a, const void *b, size_t n) noexcept {
	__m128i a128, b128;
	uint64_t a64, b64, byte_mask64;
	uint32_t a32, b32, byte_mask32;
	uint32_t mask, pos;
	uint16_t a16, b16;

	const char *ca = static_cast<const char *>(a);
	if (n < 128) {
		uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
		switch (n) {
		case 127:
		case 126:
		case 125:
		case 124:
		case 123:
		case 122:
		case 121:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 111:
		case 110:
		case 109:
		case 108:
		case 107:
		case 106:
		case 105:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 95:
		case 94:
		case 93:
		case 92:
		case 91:
		case 90:
		case 89:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 79:
		case 78:
		case 77:
		case 76:
		case 75:
		case 74:
		case 73:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 63:
		case 62:
		case 61:
		case 60:
		case 59:
		case 58:
		case 57:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 47:
		case 46:
		case 45:
		case 44:
		case 43:
		case 42:
		case 41:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 31:
		case 30:
		case 29:
		case 28:
		case 27:
		case 26:
		case 25:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			ca += (n & 15) - 16;
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 120:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 104:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 88:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 72:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 56:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 40:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 24:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 8:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 119:
		case 118:
		case 117:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 103:
		case 102:
		case 101:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 87:
		case 86:
		case 85:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 71:
		case 70:
		case 69:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 55:
		case 54:
		case 53:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 39:
		case 38:
		case 37:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 23:
		case 22:
		case 21:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			ca += (n & 7) - 8;
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 116:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 100:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 84:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 68:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 52:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 36:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 20:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 4:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 115:
		case 114:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 99:
		case 98:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 83:
		case 82:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 67:
		case 66:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 51:
		case 50:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 35:
		case 34:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 19:
		case 18:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 113:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 97:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 81:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 65:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 49:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 33:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 17:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 1:
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 112:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 96:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 80:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 64:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 48:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 32:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
		case 16:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			ca += 16;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 15:
		case 14:
		case 13:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			ca += (n & 7) - 8;
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 12:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 11:
		case 10:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 9:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 7:
		case 6:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 5:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 3:
			memcpy(&a16, ca + 0, 2);
			memcpy(&b16, (ca + d) + 0, 2);
			byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
			if (byte_mask32 != 0) {
				goto run_tzcnt_16;
			}
			ca += 2;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 2:
			memcpy(&a16, ca + 0, 2);
			memcpy(&b16, (ca + d) + 0, 2);
			byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
			if (byte_mask32 != 0) {
				goto run_tzcnt_16;
			}
			ca += 2;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 0:
		default:
			return 0;
		}
	} else {
		const char *cb = static_cast<const char *>(b);
		uint32_t initial_offset = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a)) & 15;
		switch (initial_offset) {
		case 15:
			if (*ca != *cb) {
				return 0;
			}
			break;
		case 14:
		case 13:
		case 12:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, cb + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			break;
		case 11:
		case 10:
		case 9:
		case 8:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, cb + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			break;
		case 7:
		case 6:
		case 5:
		case 4:
		case 3:
		case 2:
		case 1:
			a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
			b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(cb + 0));
			mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
			pos = static_cast<uint32_t>(__bsfd(mask));
			if (pos < 16) {
				goto run_tzcnt_sse;
			}
			break;
		case 0:
			break;
		}
		uint32_t initial_n = (-initial_offset) & 15;
		uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
		ca += initial_n;
		n -= initial_n;
		size_t aligned_n = n & ~static_cast<size_t>(127);
		n &= static_cast<size_t>(127);
		if ((reinterpret_cast<uintptr_t>(ca + d) & 15) != 0) {
			for (const char *aligned_ca_end = ca + aligned_n; ca < aligned_ca_end; ca += 128) {
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 16));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 16));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_16;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 32));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 32));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_32;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 48));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 48));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_48;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 64));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 64));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_64;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 80));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 80));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_80;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 96));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 96));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_96;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 112));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 112));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_112;
				}
			}
			switch (n) {
			case 127:
			case 126:
			case 125:
			case 124:
			case 123:
			case 122:
			case 121:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 111:
			case 110:
			case 109:
			case 108:
			case 107:
			case 106:
			case 105:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 95:
			case 94:
			case 93:
			case 92:
			case 91:
			case 90:
			case 89:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 79:
			case 78:
			case 77:
			case 76:
			case 75:
			case 74:
			case 73:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 63:
			case 62:
			case 61:
			case 60:
			case 59:
			case 58:
			case 57:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 47:
			case 46:
			case 45:
			case 44:
			case 43:
			case 42:
			case 41:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
				ca += (n & 15) - 16;
				a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 120:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 104:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 88:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 72:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 56:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 40:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 24:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 8:
				memcpy(&a64, ca + 0, 8);
				memcpy(&b64, (ca + d) + 0, 8);
				byte_mask64 = a64 ^ b64;
				if (byte_mask64 != 0) {
					goto run_tzcnt_64;
				}
				ca += 8;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 119:
			case 118:
			case 117:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 103:
			case 102:
			case 101:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 87:
			case 86:
			case 85:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 71:
			case 70:
			case 69:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 55:
			case 54:
			case 53:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 39:
			case 38:
			case 37:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 23:
			case 22:
			case 21:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 7:
			case 6:
			case 5:
				ca += (n & 7) - 8;
				memcpy(&a64, ca + 0, 8);
				memcpy(&b64, (ca + d) + 0, 8);
				byte_mask64 = a64 ^ b64;
				if (byte_mask64 != 0) {
					goto run_tzcnt_64;
				}
				ca += 8;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 116:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 100:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 84:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 68:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 52:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 36:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 20:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 4:
				memcpy(&a32, ca + 0, 4);
				memcpy(&b32, (ca + d) + 0, 4);
				byte_mask32 = a32 ^ b32;
				if (byte_mask32 != 0) {
					goto run_tzcnt_32;
				}
				ca += 4;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 115:
			case 114:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 99:
			case 98:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 83:
			case 82:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 67:
			case 66:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 51:
			case 50:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 35:
			case 34:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 19:
			case 18:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 3:
			case 2:
				ca += (n & 3) - 4;
				memcpy(&a32, ca + 0, 4);
				memcpy(&b32, (ca + d) + 0, 4);
				byte_mask32 = a32 ^ b32;
				if (byte_mask32 != 0) {
					goto run_tzcnt_32;
				}
				ca += 4;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 113:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 97:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 81:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 65:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 49:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 33:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 17:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 1:
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
			case 112:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 96:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 80:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 64:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 48:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 32:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 16:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 0:
			default:
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			}
		} else {
			for (const char *aligned_ca_end = ca + aligned_n; ca < aligned_ca_end; ca += 128) {
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 16));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 16));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_16;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 32));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 32));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_32;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 48));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 48));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_48;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 64));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 64));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_64;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 80));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 80));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_80;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 96));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 96));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_96;
				}
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 112));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 112));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_offset_112;
				}
			}
			switch (n) {
			case 127:
			case 126:
			case 125:
			case 124:
			case 123:
			case 122:
			case 121:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 111:
			case 110:
			case 109:
			case 108:
			case 107:
			case 106:
			case 105:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 95:
			case 94:
			case 93:
			case 92:
			case 91:
			case 90:
			case 89:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 79:
			case 78:
			case 77:
			case 76:
			case 75:
			case 74:
			case 73:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 63:
			case 62:
			case 61:
			case 60:
			case 59:
			case 58:
			case 57:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 47:
			case 46:
			case 45:
			case 44:
			case 43:
			case 42:
			case 41:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
				ca += (n & 15) - 16;
				a128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_loadu_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 120:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 104:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 88:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 72:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 56:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 40:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 24:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 8:
				memcpy(&a64, ca + 0, 8);
				memcpy(&b64, (ca + d) + 0, 8);
				byte_mask64 = a64 ^ b64;
				if (byte_mask64 != 0) {
					goto run_tzcnt_64;
				}
				ca += 8;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 119:
			case 118:
			case 117:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 103:
			case 102:
			case 101:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 87:
			case 86:
			case 85:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 71:
			case 70:
			case 69:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 55:
			case 54:
			case 53:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 39:
			case 38:
			case 37:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 23:
			case 22:
			case 21:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 7:
			case 6:
			case 5:
				ca += (n & 7) - 8;
				memcpy(&a64, ca + 0, 8);
				memcpy(&b64, (ca + d) + 0, 8);
				byte_mask64 = a64 ^ b64;
				if (byte_mask64 != 0) {
					goto run_tzcnt_64;
				}
				ca += 8;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 116:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 100:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 84:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 68:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 52:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 36:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 20:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 4:
				memcpy(&a32, ca + 0, 4);
				memcpy(&b32, (ca + d) + 0, 4);
				byte_mask32 = a32 ^ b32;
				if (byte_mask32 != 0) {
					goto run_tzcnt_32;
				}
				ca += 4;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 115:
			case 114:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 99:
			case 98:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 83:
			case 82:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 67:
			case 66:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 51:
			case 50:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 35:
			case 34:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 19:
			case 18:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 3:
			case 2:
				ca += (n & 3) - 4;
				memcpy(&a32, ca + 0, 4);
				memcpy(&b32, (ca + d) + 0, 4);
				byte_mask32 = a32 ^ b32;
				if (byte_mask32 != 0) {
					goto run_tzcnt_32;
				}
				ca += 4;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 113:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 97:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 81:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 65:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 49:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 33:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 17:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 1:
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
			case 112:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 96:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 80:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 64:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 48:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 32:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
			case 16:
				a128 = _mm_load_si128(reinterpret_cast<const __m128i *>(ca + 0));
				b128 = _mm_load_si128(reinterpret_cast<const __m128i *>((ca + d) + 0));
				mask = ~_mm_movemask_epi8(_mm_cmpeq_epi8(a128, b128));
				pos = static_cast<uint32_t>(__bsfd(mask));
				if (pos < 16) {
					goto run_tzcnt_sse;
				}
				ca += 16;
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			case 0:
			default:
				return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
			}
		}
	}
run_tzcnt_offset_112:
	ca += 16;
run_tzcnt_offset_96:
	ca += 16;
run_tzcnt_offset_80:
	ca += 16;
run_tzcnt_offset_64:
	ca += 16;
run_tzcnt_offset_48:
	ca += 16;
run_tzcnt_offset_32:
	ca += 16;
run_tzcnt_offset_16:
	ca += 16;
run_tzcnt_sse:
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_64:
	pos = static_cast<uint32_t>(__bsfq(byte_mask64)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_32:
run_tzcnt_16:
	pos = static_cast<uint32_t>(__bsfd(byte_mask32)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
}
#endif

size_t mempcmp_uint64_unaligned(const void *a, const void *b, size_t n) noexcept {
	uint64_t a64, b64, byte_mask64;
	uint32_t a32, b32, byte_mask32;
	uint32_t pos;
	uint16_t a16, b16;

	const char *ca = static_cast<const char *>(a);
	uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
	size_t aligned_n = n & ~static_cast<size_t>(31);
	for (const char *aligned_ca_end = ca + aligned_n; ca < aligned_ca_end; ca += 32) {
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		memcpy(&a64, ca + 8, 8);
		memcpy(&b64, (ca + d) + 8, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_offset_8;
		}
		memcpy(&a64, ca + 16, 8);
		memcpy(&b64, (ca + d) + 16, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_offset_16;
		}
		memcpy(&a64, ca + 24, 8);
		memcpy(&b64, (ca + d) + 24, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_offset_24;
		}
	}

	n &= static_cast<size_t>(31);
	switch (n) {
	case 31:
	case 30:
	case 29:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 23:
	case 22:
	case 21:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 15:
	case 14:
	case 13:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		ca += (n & 7) - 8;
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 28:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 20:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 12:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 4:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 27:
	case 26:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 19:
	case 18:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 11:
	case 10:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 25:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 17:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 9:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 1:
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 24:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 16:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
	case 8:
		memcpy(&a64, ca + 0, 8);
		memcpy(&b64, (ca + d) + 0, 8);
		byte_mask64 = a64 ^ b64;
		if (byte_mask64 != 0) {
			goto run_tzcnt_64;
		}
		ca += 8;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 7:
	case 6:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		ca += (n & 3) - 4;
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 5:
		memcpy(&a32, ca + 0, 4);
		memcpy(&b32, (ca + d) + 0, 4);
		byte_mask32 = a32 ^ b32;
		if (byte_mask32 != 0) {
			goto run_tzcnt_32;
		}
		ca += 4;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 3:
		memcpy(&a16, ca + 0, 2);
		memcpy(&b16, (ca + d) + 0, 2);
		byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
		if (byte_mask32 != 0) {
			goto run_tzcnt_16;
		}
		ca += 2;
		return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
	case 2:
		memcpy(&a16, ca + 0, 2);
		memcpy(&b16, (ca + d) + 0, 2);
		byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
		if (byte_mask32 != 0) {
			goto run_tzcnt_16;
		}
		ca += 2;
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	case 0:
	default:
		return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
	}
run_tzcnt_offset_24:
	ca += 8;
run_tzcnt_offset_16:
	ca += 8;
run_tzcnt_offset_8:
	ca += 8;
run_tzcnt_64:
	pos = static_cast<uint32_t>(__bsfq(byte_mask64)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_32:
run_tzcnt_16:
	pos = static_cast<uint32_t>(__bsfd(byte_mask32)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
}

size_t mempcmp_uint64_aligned(const void *a, const void *b, size_t n) noexcept {
	uint64_t a64, b64, byte_mask64;
	uint32_t a32, b32, byte_mask32;
	uint32_t pos;
	uint16_t a16, b16;

	const char *ca = static_cast<const char *>(a);
	if (n < 32) {
		uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
		switch (n) {
		case 31:
		case 30:
		case 29:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 23:
		case 22:
		case 21:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 15:
		case 14:
		case 13:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			ca += (n & 7) - 8;
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 28:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 20:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 12:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 4:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 27:
		case 26:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 19:
		case 18:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 11:
		case 10:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 25:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 17:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 9:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 1:
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 24:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 16:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 8:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 7:
		case 6:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 5:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 3:
			memcpy(&a16, ca + 0, 2);
			memcpy(&b16, (ca + d) + 0, 2);
			byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
			if (byte_mask32 != 0) {
				goto run_tzcnt_16;
			}
			ca += 2;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 2:
			memcpy(&a16, ca + 0, 2);
			memcpy(&b16, (ca + d) + 0, 2);
			byte_mask32 = static_cast<uint32_t>(a16) ^ static_cast<uint32_t>(b16);
			if (byte_mask32 != 0) {
				goto run_tzcnt_16;
			}
			ca += 2;
			return reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a);
		case 0:
		default:
			return 0;
		}
	} else {
		const char *cb = static_cast<const char *>(b);
		uint32_t initial_offset = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a)) & 7;
		switch (initial_offset) {
		case 7:
			if (*ca != *cb) {
				return 0;
			}
			break;
		case 6:
		case 5:
		case 4:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, cb + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			break;
		case 3:
		case 2:
		case 1:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, cb + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			break;
		case 0:
			break;
		}
		uint32_t initial_n = (-initial_offset) & 7;
		uintptr_t d = reinterpret_cast<uintptr_t>(b) - reinterpret_cast<uintptr_t>(a);
		ca += initial_n;
		n -= initial_n;
		size_t aligned_n = n & ~static_cast<size_t>(31);
		n &= static_cast<size_t>(31);
		for (const char *aligned_ca_end = ca + aligned_n; ca < aligned_ca_end; ca += 32) {
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			memcpy(&a64, ca + 8, 8);
			memcpy(&b64, (ca + d) + 8, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_offset_8;
			}
			memcpy(&a64, ca + 16, 8);
			memcpy(&b64, (ca + d) + 16, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_offset_16;
			}
			memcpy(&a64, ca + 24, 8);
			memcpy(&b64, (ca + d) + 24, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_offset_24;
			}
		}
		switch (n) {
		case 31:
		case 30:
		case 29:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 23:
		case 22:
		case 21:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 15:
		case 14:
		case 13:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 7:
		case 6:
		case 5:
			ca += (n & 7) - 8;
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
		case 28:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 20:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 12:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 4:
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
		case 27:
		case 26:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 19:
		case 18:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 11:
		case 10:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 3:
		case 2:
			ca += (n & 3) - 4;
			memcpy(&a32, ca + 0, 4);
			memcpy(&b32, (ca + d) + 0, 4);
			byte_mask32 = a32 ^ b32;
			if (byte_mask32 != 0) {
				goto run_tzcnt_32;
			}
			ca += 4;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
		case 25:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 17:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 9:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 1:
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + (*ca == *(ca + d) ? 1 : 0);
		case 24:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 16:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
		case 8:
			memcpy(&a64, ca + 0, 8);
			memcpy(&b64, (ca + d) + 0, 8);
			byte_mask64 = a64 ^ b64;
			if (byte_mask64 != 0) {
				goto run_tzcnt_64;
			}
			ca += 8;
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
		case 0:
		default:
			return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a));
		}
	}
run_tzcnt_offset_24:
	ca += 8;
run_tzcnt_offset_16:
	ca += 8;
run_tzcnt_offset_8:
	ca += 8;
run_tzcnt_64:
	pos = static_cast<uint32_t>(__bsfq(byte_mask64)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
run_tzcnt_32:
run_tzcnt_16:
	pos = static_cast<uint32_t>(__bsfd(byte_mask32)) / 8u;
	return (reinterpret_cast<uintptr_t>(ca) - reinterpret_cast<uintptr_t>(a)) + pos;
}

}
