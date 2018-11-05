//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_FASTHASH_H
#define EJ_FASTHASH_H

#include <stddef.h>
#include <stdint.h>

#include "Common.h"

namespace ej {

constexpr uint64_t FASTHASH_M = UINT64_C(0x880355f21e6d1965);

EJ_ALWAYS_INLINE uint64_t fasthash_mix(uint64_t hash) noexcept {
	hash ^= hash >> 23;
	hash *= UINT64_C(0x2127599bf4325c37);
	hash ^= hash >> 47;

	return hash;
}

uint64_t fasthash64(const void *s, size_t n, uint64_t seed = 0) noexcept;
inline uint64_t fasthash64(uint32_t value, uint64_t seed = 0) noexcept {
	uint64_t hash;

	hash = seed ^ (sizeof(value) * FASTHASH_M);
	hash ^= fasthash_mix(value);
	hash *= FASTHASH_M;
	hash = fasthash_mix(hash);

	return hash;
}
inline uint64_t fasthash64(uint64_t value, uint64_t seed = 0) noexcept {
	uint64_t hash;

	hash = seed ^ (sizeof(value) * FASTHASH_M);
	hash ^= fasthash_mix(value);
	hash *= FASTHASH_M;
	hash = fasthash_mix(hash);

	return hash;
}
inline uint64_t fasthash64(int32_t value, uint64_t seed = 0) noexcept {
	return fasthash64(static_cast<uint32_t>(value), seed);
}
inline uint64_t fasthash64(int64_t value, uint64_t seed = 0) noexcept {
	return fasthash64(static_cast<uint64_t>(value), seed);
}

inline uint32_t fasthash32(const void *s, size_t n, uint32_t seed = 0) noexcept {
	auto hash = fasthash64(s, n, seed);
	return static_cast<uint32_t>(hash - (hash >> 32));
}
inline uint32_t fasthash32(uint32_t value, uint32_t seed = 0) noexcept {
	auto hash = fasthash64(value, seed);
	return static_cast<uint32_t>(hash - (hash >> 32));
}
inline uint32_t fasthash32(uint64_t value, uint32_t seed = 0) noexcept {
	auto hash = fasthash64(value, seed);
	return static_cast<uint32_t>(hash - (hash >> 32));
}
inline uint32_t fasthash32(int32_t value, uint32_t seed = 0) noexcept {
	return fasthash32(static_cast<uint32_t>(value), seed);
}
inline uint32_t fasthash32(int64_t value, uint32_t seed = 0) noexcept {
	return fasthash32(static_cast<uint64_t>(value), seed);
}

//! Helper class for computing the hash of a string
template <typename T>
class FastHash;

template <>
class FastHash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const void *s, size_t n, value_type seed = 0) noexcept {
		return fasthash32(s, n, seed);
	}

	static value_type eval(uint32_t value, value_type seed = 0) noexcept {
		return fasthash32(value, seed);
	}

	static value_type eval(uint64_t value, value_type seed = 0) noexcept {
		return fasthash32(value, seed);
	}

	static value_type eval(int32_t value, value_type seed = 0) noexcept {
		return fasthash32(value, seed);
	}

	static value_type eval(int64_t value, value_type seed = 0) noexcept {
		return fasthash32(value, seed);
	}
};

template <>
class FastHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const void *s, size_t n, value_type seed = 0) noexcept {
		return fasthash64(s, n, seed);
	}

	static value_type eval(uint32_t value, value_type seed = 0) noexcept {
		return fasthash64(value, seed);
	}

	static value_type eval(uint64_t value, value_type seed = 0) noexcept {
		return fasthash64(value, seed);
	}

	static value_type eval(int32_t value, value_type seed = 0) noexcept {
		return fasthash64(value, seed);
	}

	static value_type eval(int64_t value, value_type seed = 0) noexcept {
		return fasthash64(value, seed);
	}
};

}

#endif
