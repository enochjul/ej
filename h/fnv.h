//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_FNV_H
#define EJ_FNV_H

#include <stddef.h>
#include <stdint.h>

namespace ej {

uint32_t fnv1_32(const char *s) noexcept;
uint64_t fnv1_64(const char *s) noexcept;

uint32_t fnv1a_32(const char *s) noexcept;
uint64_t fnv1a_64(const char *s) noexcept;

uint32_t fnv1_32(const void *s, size_t n) noexcept;
uint64_t fnv1_64(const void *s, size_t n) noexcept;

uint32_t fnv1a_32(const void *s, size_t n) noexcept;
uint64_t fnv1a_64(const void *s, size_t n) noexcept;

uint32_t fnv1_add_32(const void *s, size_t n) noexcept;
uint64_t fnv1_add_64(const void *s, size_t n) noexcept;

uint32_t fnv1a_add_32(const void *s, size_t n) noexcept;
uint64_t fnv1a_add_64(const void *s, size_t n) noexcept;

//! Helper class for computing the FNV1 hash of a string
template <typename T>
class FNV1Hash;

template <>
class FNV1Hash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1_32(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1_32(s, n);
	}
};

template <>
class FNV1Hash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1_64(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1_64(s, n);
	}
};

//! Helper class for computing the FNV1 hash of a string
template <typename T>
class FNV1aHash;

template <>
class FNV1aHash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1a_32(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1a_32(s, n);
	}
};

template <>
class FNV1aHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1a_64(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1a_64(s, n);
	}
};

//! Helper class for computing the FNV1 hash (with add instead of xor) of a string
template <typename T>
class FNV1AddHash;

template <>
class FNV1AddHash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1_add_32(s, n);
	}
};

template <>
class FNV1AddHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1_add_64(s, n);
	}
};

//! Helper class for computing the FNV1a hash (with add instead of xor) of a string
template <typename T>
class FNV1aAddHash;

template <>
class FNV1aAddHash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1a_add_32(s, n);
	}
};

template <>
class FNV1aAddHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1a_add_64(s, n);
	}
};

}

#endif
