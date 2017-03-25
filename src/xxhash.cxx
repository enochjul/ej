/*
xxHash - Fast Hash algorithm
Copyright (C) 2012-2016, Yann Collet
BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
You can contact the author at :
- xxHash source repository : https://github.com/Cyan4973/xxHash
*/

#include "rotate.h"
#include "xxhash.h"

namespace ej {

enum : uint32_t {
	XXHASH_32_PRIME_1 = 2654435761U,
	XXHASH_32_PRIME_2 = 2246822519U,
	XXHASH_32_PRIME_3 = 3266489917U,
	XXHASH_32_PRIME_4 = 668265263U,
	XXHASH_32_PRIME_5 = 374761393U,
};

uint32_t xxhash_32(const void *s, size_t n, uint32_t seed) noexcept {
	const uint32_t *start;
	const void *end;
	size_t aligned_n;
	uint32_t hash, v1, v2, v3, v4;

	end = s;
	aligned_n = (n / 16) * 16;
	if (aligned_n > 0) {
		start = static_cast<const uint32_t *>(s);
		end = static_cast<const uint8_t *>(end) + aligned_n;

		v1 = seed + XXHASH_32_PRIME_1 + XXHASH_32_PRIME_2;
		v2 = seed + XXHASH_32_PRIME_2;
		v3 = seed;
		v4 = seed - XXHASH_32_PRIME_1;

		do {
			v1 += start[0] * XXHASH_32_PRIME_2;
			v1 = rotate_left<uint32_t>(v1, 13);
			v1 *= XXHASH_32_PRIME_1;

			v2 += start[1] * XXHASH_32_PRIME_2;
			v2 = rotate_left<uint32_t>(v2, 13);
			v2 *= XXHASH_32_PRIME_1;

			v3 += start[2] * XXHASH_32_PRIME_2;
			v3 = rotate_left<uint32_t>(v3, 13);
			v3 *= XXHASH_32_PRIME_1;

			v4 += start[3] * XXHASH_32_PRIME_2;
			v4 = rotate_left<uint32_t>(v4, 13);
			v4 *= XXHASH_32_PRIME_1;

			start += 4;
		} while (start < end);

		hash = rotate_left<uint32_t>(v1, 1) + rotate_left<uint32_t>(v2, 7) + rotate_left<uint32_t>(v3, 12) + rotate_left<uint32_t>(v4, 18);
	}
	else
	{
		hash = seed + XXHASH_32_PRIME_5;
	}

	hash += static_cast<uint32_t>(n);
	switch (n & 15) {
	case 15:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[1] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[2] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[12] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[13] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[14] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 14:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[1] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[2] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[12] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[13] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 13:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[1] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[2] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[12] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 12:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[1] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[2] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		break;

	case 11:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[1] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[8] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[9] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[10] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 10:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[1] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[8] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[9] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 9:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[1] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[8] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 8:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint32_t *>(end)[1] * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		break;

	case 7:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[4] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[5] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[6] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 6:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[4] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[5] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 5:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		hash += static_cast<const uint8_t *>(end)[4] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 4:
		hash += *static_cast<const uint32_t *>(end) * XXHASH_32_PRIME_3;
		hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
		break;

	case 3:
		hash += *static_cast<const uint8_t *>(end) * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[1] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[2] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 2:
		hash += *static_cast<const uint8_t *>(end) * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		hash += static_cast<const uint8_t *>(end)[1] * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;

	case 1:
		hash += *static_cast<const uint8_t *>(end) * XXHASH_32_PRIME_5;
		hash = rotate_left<uint32_t>(hash, 11) * XXHASH_32_PRIME_1;
		break;
	}

	hash ^= hash >> 15;
	hash *= XXHASH_32_PRIME_2;
	hash ^= hash >> 13;
	hash *= XXHASH_32_PRIME_3;
	hash ^= hash >> 16;

	return hash;
}

uint32_t xxhash_32(uint32_t value, uint32_t seed) noexcept {
	uint32_t hash;

	hash = seed + XXHASH_32_PRIME_5;

	hash += sizeof(value);
	hash += value * XXHASH_32_PRIME_3;
	hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;

	hash ^= hash >> 15;
	hash *= XXHASH_32_PRIME_2;
	hash ^= hash >> 13;
	hash *= XXHASH_32_PRIME_3;
	hash ^= hash >> 16;

	return hash;
}

uint32_t xxhash_32(uint64_t value, uint32_t seed) noexcept {
	uint32_t hash;

	hash = seed + XXHASH_32_PRIME_5;

	hash += sizeof(value);
	hash += static_cast<uint32_t>(value) * XXHASH_32_PRIME_3;
	hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;
	hash += static_cast<uint32_t>(value >> 32) * XXHASH_32_PRIME_3;
	hash = rotate_left<uint32_t>(hash, 17) * XXHASH_32_PRIME_4;

	hash ^= hash >> 15;
	hash *= XXHASH_32_PRIME_2;
	hash ^= hash >> 13;
	hash *= XXHASH_32_PRIME_3;
	hash ^= hash >> 16;

	return hash;
}

enum : uint64_t {
	XXHASH_64_PRIME_1 = UINT64_C(11400714785074694791),
	XXHASH_64_PRIME_2 = UINT64_C(14029467366897019727),
	XXHASH_64_PRIME_3 = UINT64_C(1609587929392839161),
	XXHASH_64_PRIME_4 = UINT64_C(9650029242287828579),
	XXHASH_64_PRIME_5 = UINT64_C(2870177450012600261),
};

uint64_t xxhash_64(const void *s, size_t n, uint64_t seed) noexcept {
	const uint64_t *start;
	const void *end;
	const uint8_t *remaining_bytes;
	size_t aligned_n;
	uint64_t hash, v1, v2, v3, v4, k1;

	end = s;
	aligned_n = (n / 32) * 32;
	if (aligned_n > 0) {
		start = static_cast<const uint64_t *>(s);
		end = static_cast<const uint8_t *>(end) + aligned_n;

		v1 = seed + XXHASH_64_PRIME_1 + XXHASH_64_PRIME_2;
		v2 = seed + XXHASH_64_PRIME_2;
		v3 = seed + 0;
		v4 = seed - XXHASH_64_PRIME_1;

		do {
			v1 += start[0] * XXHASH_64_PRIME_2;
			v1 = rotate_left<uint64_t>(v1, 31);
			v1 *= XXHASH_64_PRIME_1;

			v2 += start[1] * XXHASH_64_PRIME_2;
			v2 = rotate_left<uint64_t>(v2, 31);
			v2 *= XXHASH_64_PRIME_1;

			v3 += start[2] * XXHASH_64_PRIME_2;
			v3 = rotate_left<uint64_t>(v3, 31);
			v3 *= XXHASH_64_PRIME_1;

			v4 += start[3] * XXHASH_64_PRIME_2;
			v4 = rotate_left<uint64_t>(v4, 31);
			v4 *= XXHASH_64_PRIME_1;

			start += 4;
		} while (start < end);

		hash = rotate_left<uint64_t>(v1, 1) + rotate_left<uint64_t>(v2, 7) + rotate_left<uint64_t>(v3, 12) + rotate_left<uint64_t>(v4, 18);

		v1 *= XXHASH_64_PRIME_2;
		v1 = rotate_left<uint64_t>(v1, 31);
		v1 *= XXHASH_64_PRIME_1;
		hash ^= v1;
		hash = hash * XXHASH_64_PRIME_1 + XXHASH_64_PRIME_4;

		v2 *= XXHASH_64_PRIME_2;
		v2 = rotate_left<uint64_t>(v2, 31);
		v2 *= XXHASH_64_PRIME_1;
		hash ^= v2;
		hash = hash * XXHASH_64_PRIME_1 + XXHASH_64_PRIME_4;

		v3 *= XXHASH_64_PRIME_2;
		v3 = rotate_left<uint64_t>(v3, 31);
		v3 *= XXHASH_64_PRIME_1;
		hash ^= v3;
		hash = hash * XXHASH_64_PRIME_1 + XXHASH_64_PRIME_4;

		v4 *= XXHASH_64_PRIME_2;
		v4 = rotate_left<uint64_t>(v4, 31);
		v4 *= XXHASH_64_PRIME_1;
		hash ^= v4;
		hash = hash * XXHASH_64_PRIME_1 + XXHASH_64_PRIME_4;
	}
	else
	{
		hash = seed + XXHASH_64_PRIME_5;
	}

	hash += n;
	aligned_n = ((n & 31) / 8) * 8;
	if (aligned_n > 0) {
		start = static_cast<const uint64_t *>(end);
		end = static_cast<const uint8_t *>(end) + aligned_n;
		do {
			k1 = *start;
			k1 *= XXHASH_64_PRIME_2;
			k1 = rotate_left<uint64_t>(k1, 31);
			k1 *= XXHASH_64_PRIME_1;
			hash ^= k1;
			hash = rotate_left<uint64_t>(hash, 27) * XXHASH_64_PRIME_1 + XXHASH_64_PRIME_4;

			++start;
		} while (start < end);
	}

	if ((n & 4) != 0) {
		hash ^= static_cast<uint64_t>(*static_cast<const uint32_t *>(end)) * XXHASH_64_PRIME_1;
		hash = rotate_left<uint64_t>(hash, 23) * XXHASH_64_PRIME_2 + XXHASH_64_PRIME_3;

		end = static_cast<const uint32_t *>(end) + 1;
	}

	for (remaining_bytes = static_cast<const uint8_t *>(end), end = static_cast<const uint8_t *>(s) + n; remaining_bytes < end; ++remaining_bytes) {
		hash ^= static_cast<uint64_t>(*remaining_bytes) * XXHASH_64_PRIME_5;
		hash = rotate_left<uint64_t>(hash, 11) * XXHASH_64_PRIME_1;
	}

	hash ^= hash >> 33;
	hash *= XXHASH_64_PRIME_2;
	hash ^= hash >> 29;
	hash *= XXHASH_64_PRIME_3;
	hash ^= hash >> 32;

	return hash;
}

uint64_t xxhash_64(uint32_t value, uint64_t seed) noexcept {
	uint64_t hash;

	hash = seed + XXHASH_64_PRIME_5;

	hash += sizeof(value);
	hash ^= static_cast<uint64_t>(value) * XXHASH_64_PRIME_1;
	hash = rotate_left<uint64_t>(hash, 23) * XXHASH_64_PRIME_2 + XXHASH_64_PRIME_3;

	hash ^= hash >> 33;
	hash *= XXHASH_64_PRIME_2;
	hash ^= hash >> 29;
	hash *= XXHASH_64_PRIME_3;
	hash ^= hash >> 32;

	return hash;
}

uint64_t xxhash_64(uint64_t value, uint64_t seed) noexcept {
	uint64_t hash, k1;

	hash = seed + XXHASH_64_PRIME_5;
	hash += sizeof(value);

	k1 = value;
	k1 *= XXHASH_64_PRIME_2;
	k1 = rotate_left<uint64_t>(k1, 31);
	k1 *= XXHASH_64_PRIME_1;
	hash ^= k1;
	hash = rotate_left<uint64_t>(hash, 27) * XXHASH_64_PRIME_1 + XXHASH_64_PRIME_4;

	hash ^= hash >> 33;
	hash *= XXHASH_64_PRIME_2;
	hash ^= hash >> 29;
	hash *= XXHASH_64_PRIME_3;
	hash ^= hash >> 32;

	return hash;
}

}
