//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "murmur3.h"

namespace ej {

uint32_t murmur3_32(const void *s, size_t n, uint32_t seed) noexcept {
	const void *start;
	const void *end;
	size_t aligned_n;
	uint32_t hash, k;

	end = s;
	hash = seed;
	aligned_n = (n / 4) * 4;
	if (aligned_n > 0) {
		start = s;
		end = lea(end, aligned_n);
		do {
			k = loadu<uint32_t>(start);

			k *= MURMUR3_32_C1;
			k = rotate_left<uint32_t>(k, 15);
			k *= MURMUR3_32_C2;

			hash ^= k;
			hash = rotate_left<uint32_t>(hash, 13);
			hash = hash * 5 + UINT32_C(0xe6546b64);

			start = lea(start, sizeof(uint32_t));
		} while (start < end);
	}

	//last block
	switch(n & 3)
	{
	case 3:
		k = loadu<uint16_t>(end);
		k |= static_cast<uint32_t>(load<uint8_t>(lea(end, sizeof(uint16_t)))) << 16;

		k *= MURMUR3_32_C1;
		k = rotate_left<uint32_t>(k, 15);
		k *= MURMUR3_32_C2;
		hash ^= k;
		break;

	case 2:
		k = loadu<uint16_t>(end);

		k *= MURMUR3_32_C1;
		k = rotate_left<uint32_t>(k, 15);
		k *= MURMUR3_32_C2;
		hash ^= k;
		break;

	case 1:
		k = load<uint8_t>(end);

		k *= MURMUR3_32_C1;
		k = rotate_left<uint32_t>(k, 15);
		k *= MURMUR3_32_C2;
		hash ^= k;
		break;
	};

	hash ^= static_cast<uint32_t>(n);
	hash ^= hash >> 16;
	hash *= UINT32_C(0x85ebca6b);
	hash ^= hash >> 13;
	hash *= UINT32_C(0xc2b2ae35);
	hash ^= hash >> 16;

	return hash;
}

uint32_t murmur3_32(uint64_t value, uint32_t seed) noexcept {
	uint32_t hash, k;

	hash = seed;

	k = static_cast<uint32_t>(value);

	k *= MURMUR3_32_C1;
	k = rotate_left<uint32_t>(k, 15);
	k *= MURMUR3_32_C2;

	hash ^= k;
	hash = rotate_left<uint32_t>(hash, 13);
	hash = hash * 5 + UINT32_C(0xe6546b64);

	k = static_cast<uint32_t>(value >> 32);

	k *= MURMUR3_32_C1;
	k = rotate_left<uint32_t>(k, 15);
	k *= MURMUR3_32_C2;

	hash ^= k;
	hash = rotate_left<uint32_t>(hash, 13);
	hash = hash * 5 + UINT32_C(0xe6546b64);

	hash ^= sizeof(value);
	hash ^= hash >> 16;
	hash *= UINT32_C(0x85ebca6b);
	hash ^= hash >> 13;
	hash *= UINT32_C(0xc2b2ae35);
	hash ^= hash >> 16;

	return hash;
}

uint32_t murmur3_32(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t seed) noexcept {
	uint32_t hash;

	hash = seed;
	hash = murmur3_32_round(hash, v0);
	hash = murmur3_32_round(hash, v1);
	hash = murmur3_32_round(hash, v2);

	hash ^= sizeof(uint32_t) * 3;
	hash ^= hash >> 16;
	hash *= UINT32_C(0x85ebca6b);
	hash ^= hash >> 13;
	hash *= UINT32_C(0xc2b2ae35);
	hash ^= hash >> 16;

	return hash;
}

v4u32 murmur3_32x4(v4u32 value, uint32_t seed) noexcept {
	v4u32 hash;

	hash.set1(seed);
	hash = murmur3_32x4_round(hash, value);

	hash ^= make1_v4u32(sizeof(uint32_t));
	hash = murmur3_32x4_final_mix(hash);

	return hash;
}

enum : uint64_t {
	MURMUR3_128_C1 = UINT64_C(0x87c37b91114253d5),
	MURMUR3_128_C2 = UINT64_C(0x4cf5ad432745937f),
};

inline uint64_t murmur3_128_final_mix(uint64_t k) {
	k ^= k >> 33;
	k *= UINT64_C(0xff51afd7ed558ccd);
	k ^= k >> 33;
	k *= UINT64_C(0xc4ceb9fe1a85ec53);
	k ^= k >> 33;

	return k;
}

duint64 murmur3_128(const void *s, size_t n, uint64_t seed) noexcept {
	const void *start;
	const void *end;
	size_t aligned_n;
	uint64_t hash_low, hash_high;
	uint64_t k_low, k_high;

	end = s;
	hash_low = seed;
	hash_high = seed;
	aligned_n = (n / 16) * 16;
	if (aligned_n > 0) {
		start = s;
		end = lea(end, aligned_n);
		do {
			k_low = loadu<uint64_t>(start);
			k_high = loadu<uint64_t>(lea(start, sizeof(uint64_t)));

			k_low *= MURMUR3_128_C1;
			k_low = rotate_left<uint64_t>(k_low, 31);
			k_low *= MURMUR3_128_C2;
			hash_low ^= k_low;
			hash_low = rotate_left<uint64_t>(hash_low, 27);
			hash_low += hash_high;
			hash_low = hash_low * 5 + UINT32_C(0x52dce729);

			k_high *= MURMUR3_128_C2;
			k_high = rotate_left<uint64_t>(k_high, 33);
			k_high *= MURMUR3_128_C1;
			hash_high ^= k_high;
			hash_high = rotate_left<uint64_t>(hash_high, 31);
			hash_high += hash_low;
			hash_high = hash_high * 5 + UINT32_C(0x38495ab5);

			start = lea(start, sizeof(uint64_t) * 2);
		} while (start < end);
	}

	switch(n & 15)
	{
	case 15:
		k_high = loadu<uint32_t>(lea(end, sizeof(uint64_t)));
		k_high |= static_cast<uint64_t>(loadu<uint16_t>(lea(end, sizeof(uint64_t) + sizeof(uint32_t)))) << 32;
		k_high |= static_cast<uint64_t>(load<uint8_t>(lea(end, sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint16_t)))) << 48;
		goto second_last_block;

	case 14:
		k_high = loadu<uint32_t>(lea(end, sizeof(uint64_t)));
		k_high |= static_cast<uint64_t>(loadu<uint16_t>(lea(end, sizeof(uint64_t) + sizeof(uint32_t)))) << 32;
		goto second_last_block;

	case 13:
		k_high = loadu<uint32_t>(lea(end, sizeof(uint64_t)));
		k_high |= static_cast<uint64_t>(load<uint8_t>(lea(end, sizeof(uint64_t) + sizeof(uint32_t)))) << 32;
		goto second_last_block;

	case 12:
		k_high = loadu<uint32_t>(lea(end, sizeof(uint64_t)));
		goto second_last_block;

	case 11:
		k_high = loadu<uint16_t>(lea(end, sizeof(uint64_t)));
		k_high |= static_cast<uint64_t>(load<uint8_t>(lea(end, sizeof(uint64_t) + sizeof(uint16_t)))) << 16;
		goto second_last_block;

	case 10:
		k_high = loadu<uint16_t>(lea(end, sizeof(uint64_t)));
		goto second_last_block;

	case 9:
		k_high = load<uint8_t>(lea(end, sizeof(uint64_t)));
		goto second_last_block;

	case 8:
		k_low = loadu<uint64_t>(end);
		goto last_block;

	case 7:
		k_low = loadu<uint32_t>(end);
		k_low |= static_cast<uint64_t>(loadu<uint16_t>(lea(end, sizeof(uint32_t)))) << 32;
		k_low |= static_cast<uint64_t>(load<uint8_t>(lea(end, sizeof(uint32_t) + sizeof(uint16_t)))) << 48;
		goto last_block;

	case 6:
		k_low = loadu<uint32_t>(end);
		k_low |= static_cast<uint64_t>(loadu<uint16_t>(lea(end, sizeof(uint32_t)))) << 32;
		goto last_block;

	case 5:
		k_low = loadu<uint32_t>(end);
		k_low |= static_cast<uint64_t>(load<uint8_t>(lea(end, sizeof(uint32_t)))) << 32;
		goto last_block;

	case 4:
		k_low = loadu<uint32_t>(end);
		goto last_block;

	case 3:
		k_low = loadu<uint16_t>(end);
		k_low |= static_cast<uint64_t>(load<uint8_t>(lea(end, sizeof(uint16_t)))) << 16;
		goto last_block;

	case 2:
		k_low = loadu<uint16_t>(end);
		goto last_block;

	case 1:
		k_low = load<uint8_t>(end);
		goto last_block;

	case 0:
		goto final_mix;
	}

second_last_block:
	k_high *= MURMUR3_128_C2;
	k_high = rotate_left<uint64_t>(k_high, 33);
	k_high *= MURMUR3_128_C1;
	hash_high ^= k_high;

	k_low = loadu<uint64_t>(end);

last_block:
	k_low *= MURMUR3_128_C1;
	k_low = rotate_left<uint64_t>(k_low, 31);
	k_low *= MURMUR3_128_C2;
	hash_low ^= k_low;

final_mix:
	hash_low ^= n;
	hash_high ^= n;

	hash_low += hash_high;
	hash_high += hash_low;

	hash_low = murmur3_128_final_mix(hash_low);
	hash_high = murmur3_128_final_mix(hash_high);

	hash_low += hash_high;
	hash_high += hash_low;

	return make_duint64(hash_low, hash_high);
}

duint64 murmur3_128(uint32_t value, uint64_t seed) noexcept {
	uint64_t hash_low, hash_high;
	uint64_t k_low;

	hash_low = seed;
	hash_high = seed;

	k_low = value;

	k_low *= MURMUR3_128_C1;
	k_low = rotate_left<uint64_t>(k_low, 31);
	k_low *= MURMUR3_128_C2;
	hash_low ^= k_low;

	hash_low ^= sizeof(value);
	hash_high ^= sizeof(value);

	hash_low += hash_high;
	hash_high += hash_low;

	hash_low = murmur3_128_final_mix(hash_low);
	hash_high = murmur3_128_final_mix(hash_high);

	hash_low += hash_high;
	hash_high += hash_low;

	return make_duint64(hash_low, hash_high);
}

duint64 murmur3_128(uint64_t value, uint64_t seed) noexcept {
	uint64_t hash_low, hash_high;
	uint64_t k_low;

	hash_low = seed;
	hash_high = seed;

	k_low = value;

	k_low *= MURMUR3_128_C1;
	k_low = rotate_left<uint64_t>(k_low, 31);
	k_low *= MURMUR3_128_C2;
	hash_low ^= k_low;

	hash_low ^= sizeof(value);
	hash_high ^= sizeof(value);

	hash_low += hash_high;
	hash_high += hash_low;

	hash_low = murmur3_128_final_mix(hash_low);
	hash_high = murmur3_128_final_mix(hash_high);

	hash_low += hash_high;
	hash_high += hash_low;

	return make_duint64(hash_low, hash_high);
}

}
