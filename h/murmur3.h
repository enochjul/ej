//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "Common.h"
#include "duint.h"

namespace ej {

uint32_t murmur3_32(const void *s, size_t n, uint32_t seed = 0) noexcept;
uint32_t murmur3_32(uint32_t value, uint32_t seed = 0) noexcept;
uint32_t murmur3_32(uint64_t value, uint32_t seed = 0) noexcept;
EJ_ALWAYS_INLINE uint32_t murmur3_32(int32_t value, uint32_t seed = 0) noexcept {
	return murmur3_32(static_cast<uint32_t>(value), seed);
}
EJ_ALWAYS_INLINE uint32_t murmur3_32(int64_t value, uint32_t seed = 0) noexcept {
	return murmur3_32(static_cast<uint64_t>(value), seed);
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
