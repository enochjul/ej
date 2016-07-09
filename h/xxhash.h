//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_XXHASH_H
#define EJ_XXHASH_H

#include <stddef.h>
#include <stdint.h>

namespace ej {

uint32_t xxhash_32(const void *s, size_t n, uint32_t seed = 0) noexcept;
uint64_t xxhash_64(const void *s, size_t n, uint64_t seed = 0) noexcept;

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
};

template <>
class xxHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const void *s, size_t n, value_type seed = 0) noexcept {
		return xxhash_64(s, n, seed);
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
};

}

#endif
