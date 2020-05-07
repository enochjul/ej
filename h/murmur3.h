//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "Common.h"
#include "duint.h"
#include "rotate.h"
#include "v4u32.h"
#include "v4i32.h"

namespace ej {

enum : uint32_t {
	MURMUR3_32_C1 = 0xcc9e2d51U,
	MURMUR3_32_C2 = 0x1b873593U,
};

EJ_ALWAYS_INLINE uint32_t murmur3_32_round(uint32_t hash, uint32_t k) noexcept {
	k *= MURMUR3_32_C1;
	k = rotate_left<uint32_t>(k, 15);
	k *= MURMUR3_32_C2;

	hash ^= k;
	hash = rotate_left<uint32_t>(hash, 13);
	hash = hash * 5 + UINT32_C(0xe6546b64);

	return hash;
}

EJ_ALWAYS_INLINE v4u32 murmur3_32x4_round(v4u32 hash, v4u32 k) noexcept {
	k *= make1_v4u32(MURMUR3_32_C1);
	k = rotate_left<15>(k);
	k *= make1_v4u32(MURMUR3_32_C2);

	hash ^= k;
	hash = rotate_left<13>(hash);
	hash = hash * make1_v4u32(5) + make1_v4u32(UINT32_C(0xe6546b64));

	return hash;
}

EJ_ALWAYS_INLINE uint32_t murmur3_32_final_mix(uint32_t k) noexcept {
	k ^= k >> 16;
	k *= UINT32_C(0x85ebca6b);
	k ^= k >> 13;
	k *= UINT32_C(0xc2b2ae35);
	k ^= k >> 16;
	return k;
}

EJ_ALWAYS_INLINE v4u32 murmur3_32x4_final_mix(v4u32 k) noexcept {
	k ^= shift_right<16>(k);
	k *= make1_v4u32(UINT32_C(0x85ebca6b));
	k ^= shift_right<13>(k);
	k *= make1_v4u32(UINT32_C(0xc2b2ae35));
	k ^= shift_right<16>(k);
	return k;
}

uint32_t murmur3_32(const void *s, size_t n, uint32_t seed = 0) noexcept;
EJ_ALWAYS_INLINE uint32_t murmur3_32(uint32_t value, uint32_t seed = 0) noexcept {
	uint32_t hash;

	hash = seed;
	hash = murmur3_32_round(hash, value);

	hash ^= sizeof(value);
	hash = murmur3_32_final_mix(hash);

	return hash;
}
uint32_t murmur3_32(uint64_t value, uint32_t seed = 0) noexcept;
uint32_t murmur3_32(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t seed = 0) noexcept;
EJ_ALWAYS_INLINE uint32_t murmur3_32(int32_t value, uint32_t seed = 0) noexcept {
	return murmur3_32(static_cast<uint32_t>(value), seed);
}
EJ_ALWAYS_INLINE uint32_t murmur3_32(int64_t value, uint32_t seed = 0) noexcept {
	return murmur3_32(static_cast<uint64_t>(value), seed);
}
v4u32 murmur3_32x4(v4u32 value, uint32_t seed = 0) noexcept;
EJ_ALWAYS_INLINE v4u32 murmur3_32x4(v4u32 v0, v4u32 v1, v4u32 v2, uint32_t seed = 0) noexcept {
	v4u32 hash;

	hash.set1(seed);
	hash = murmur3_32x4_round(hash, v0);
	hash = murmur3_32x4_round(hash, v1);
	hash = murmur3_32x4_round(hash, v2);

	hash ^= make1_v4u32(sizeof(uint32_t) * 3);
	hash = murmur3_32x4_final_mix(hash);

	return hash;
}
EJ_ALWAYS_INLINE v4u32 murmur3_32x4(v4i32 value, uint32_t seed = 0) noexcept {
	return murmur3_32x4(make_v4u32(value), seed);
}
EJ_ALWAYS_INLINE v4u32 murmur3_32x4(v4i32 v0, v4i32 v1, v4i32 v2, uint32_t seed = 0) noexcept {
	return murmur3_32x4(make_v4u32(v0), make_v4u32(v1), make_v4u32(v2), seed);
}

duint64 murmur3_128(const void *s, size_t n, uint64_t seed = 0) noexcept;
duint64 murmur3_128(uint32_t value, uint64_t seed = 0) noexcept;
duint64 murmur3_128(uint64_t value, uint64_t seed = 0) noexcept;
EJ_ALWAYS_INLINE duint64 murmur3_128(int32_t value, uint64_t seed = 0) noexcept {
	return murmur3_128(static_cast<uint32_t>(value), seed);
}
EJ_ALWAYS_INLINE duint64 murmur3_128(int64_t value, uint64_t seed = 0) noexcept {
	return murmur3_128(static_cast<uint64_t>(value), seed);
}

//! Helper class for computing the hash of a string
template <typename T>
class Murmur3Hash;

template <>
class Murmur3Hash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const void *s, size_t n, value_type seed = 0) noexcept {
		return murmur3_32(s, n, seed);
	}
};

template <>
class Murmur3Hash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const void *s, size_t n, value_type seed = 0) noexcept {
		return duint64_get_low(murmur3_128(s, n, seed));
	}

	static value_type eval(uint32_t value, value_type seed = 0) noexcept {
		return duint64_get_low(murmur3_128(value, seed));
	}
};

//! Helper class for computing the hash of a string using a specific seed
template <typename T>
class Murmur3HashSeed;

template <>
class Murmur3HashSeed<uint32_t> {
public:
	typedef uint32_t value_type;

private:
	value_type Seed;

public:
	constexpr explicit Murmur3HashSeed(value_type seed = 0) noexcept : Seed(seed) {
	}

	value_type eval(const void *s, size_t n) const noexcept {
		return murmur3_32(s, n, Seed);
	}

	template <typename K>
	value_type eval(const K &k) const noexcept {
		return eval(&k, sizeof(k), Seed);
	}
};

template <>
class Murmur3HashSeed<uint64_t> {
public:
	typedef uint64_t value_type;

private:
	value_type Seed;

public:
	constexpr explicit Murmur3HashSeed(value_type seed = 0) noexcept : Seed(seed) {
	}

	value_type eval(const void *s, size_t n) const noexcept {
		return duint64_get_low(murmur3_128(s, n, Seed));
	}

	template <typename K>
	value_type eval(const K &k) const noexcept {
		return eval(&k, sizeof(k), Seed);
	}
};

}
