//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_MURMUR3_H
#define EJ_MURMUR3_H

#include <stddef.h>
#include <stdint.h>

#include "Common.h"

namespace ej {

uint32_t murmur3_32(const void *s, size_t n, uint32_t seed = 0) noexcept;
duint<uint64_t> murmur3_128(const void *s, size_t n, uint64_t seed = 0) noexcept;

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
		return murmur3_128(s, n, seed).Low;
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
	constexpr explicit Murmur3HashSeed(value_type seed = 0) : Seed(seed) {
	}

	value_type eval(const void *s, size_t n) const noexcept {
		return murmur3_32(s, n, Seed);
	}
};

template <>
class Murmur3HashSeed<uint64_t> {
public:
	typedef uint64_t value_type;

private:
	value_type Seed;

public:
	constexpr explicit Murmur3HashSeed(value_type seed = 0) : Seed(seed) {
	}

	value_type eval(const void *s, size_t n) const noexcept {
		return murmur3_128(s, n, Seed).Low;
	}
};

}

#endif
