//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_XXHASH_H
#define EJ_XXHASH_H

#include <stddef.h>
#include <stdint.h>

#include "Common.h"

namespace ej {

uint32_t xxhash_32(const void *s, size_t n, uint32_t seed = 0) noexcept;
uint32_t xxhash_32(uint32_t value, uint32_t seed = 0) noexcept;
uint32_t xxhash_32(uint64_t value, uint32_t seed = 0) noexcept;
EJ_ALWAYS_INLINE uint32_t xxhash_32(int32_t value, uint32_t seed = 0) noexcept {
	return xxhash_32(static_cast<uint32_t>(value), seed);
}
EJ_ALWAYS_INLINE uint32_t xxhash_32(int64_t value, uint32_t seed = 0) noexcept {
	return xxhash_32(static_cast<uint64_t>(value), seed);
}

uint64_t xxhash_64(const void *s, size_t n, uint64_t seed = 0) noexcept;
uint64_t xxhash_64(uint32_t value, uint64_t seed = 0) noexcept;
uint64_t xxhash_64(uint64_t value, uint64_t seed = 0) noexcept;
EJ_ALWAYS_INLINE uint64_t xxhash_64(int32_t value, uint64_t seed = 0) noexcept {
	return xxhash_64(static_cast<uint32_t>(value), seed);
}
EJ_ALWAYS_INLINE uint64_t xxhash_64(int64_t value, uint64_t seed = 0) noexcept {
	return xxhash_64(static_cast<uint64_t>(value), seed);
}

//! Helper class for computing the hash of a string
template <typename T>
class xxHash;

template <>
class xxHash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const void *s, size_t n, value_type seed = 0) noexcept {
		return xxhash_32(s, n, seed);
	}

	static value_type eval(uint32_t value, value_type seed = 0) noexcept {
		return xxhash_32(value, seed);
	}

	static value_type eval(int32_t value, value_type seed = 0) noexcept {
		return xxhash_32(value, seed);
	}

	static value_type eval(uint64_t value, value_type seed = 0) noexcept {
		return xxhash_32(value, seed);
	}

	static value_type eval(int64_t value, value_type seed = 0) noexcept {
		return xxhash_32(value, seed);
	}

	template <typename K>
	static value_type eval(const K &k, value_type seed = 0) noexcept {
		return eval(&k, sizeof(K), seed);
	}
};

template <>
class xxHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const void *s, size_t n, value_type seed = 0) noexcept {
		return xxhash_64(s, n, seed);
	}

	static value_type eval(uint32_t value, value_type seed = 0) noexcept {
		return xxhash_64(value, seed);
	}

	static value_type eval(int32_t value, value_type seed = 0) noexcept {
		return xxhash_64(value, seed);
	}

	static value_type eval(uint64_t value, value_type seed = 0) noexcept {
		return xxhash_64(value, seed);
	}

	static value_type eval(int64_t value, value_type seed = 0) noexcept {
		return xxhash_64(value, seed);
	}

	template <typename K>
	static value_type eval(const K &k, value_type seed = 0) noexcept {
		return eval(&k, sizeof(K), seed);
	}
};

//! Helper class for computing the hash of a string using a specific seed
template <typename T>
class xxHashSeed;

template <>
class xxHashSeed<uint32_t> {
public:
	typedef uint32_t value_type;

private:
	value_type Seed;

public:
	constexpr explicit xxHashSeed(value_type seed = 0) noexcept : Seed(seed) {
	}

	value_type eval(const void *s, size_t n) const noexcept {
		return xxhash_32(s, n, Seed);
	}

	template <typename K>
	value_type eval(const K &k) const noexcept {
		return eval(&k, sizeof(K), Seed);
	}
};

template <>
class xxHashSeed<uint64_t> {
public:
	typedef uint64_t value_type;

private:
	value_type Seed;

public:
	constexpr explicit xxHashSeed(value_type seed = 0) noexcept : Seed(seed) {
	}

	value_type eval(const void *s, size_t n) const noexcept {
		return xxhash_64(s, n, Seed);
	}

	template <typename K>
	value_type eval(const K &k) const noexcept {
		return eval(&k, sizeof(K), Seed);
	}
};

}

#endif
