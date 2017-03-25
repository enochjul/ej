//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_DJB2_H
#define EJ_DJB2_H

#include <stddef.h>
#include <stdint.h>

#include "Common.h"

namespace ej {

//! Computes 32-bit DJB2 hash function for a null terminated string
uint32_t djb2_32(const char *s) noexcept;
//! Computes 64-bit DJB2 hash function for a null terminated string
uint64_t djb2_64(const char *s) noexcept;

//! Computes 32-bit DJB2 hash function
uint32_t djb2_32(const void *s, size_t n) noexcept;
//! Computes 32-bit DJB2 hash function
uint32_t djb2_32(uint32_t value) noexcept;
//! Computes 32-bit DJB2 hash function
uint32_t djb2_32(uint64_t value) noexcept;
//! Computes 32-bit DJB2 hash function
EJ_ALWAYS_INLINE uint32_t djb2_32(int32_t value) noexcept {
	return djb2_32(static_cast<uint32_t>(value));
}
//! Computes 32-bit DJB2 hash function
EJ_ALWAYS_INLINE uint32_t djb2_32(int64_t value) noexcept {
	return djb2_32(static_cast<uint64_t>(value));
}

//! Computes 64-bit DJB2 hash function
uint64_t djb2_64(const void *s, size_t n) noexcept;
//! Computes 64-bit DJB2 hash function
uint64_t djb2_64(uint32_t value) noexcept;
//! Computes 64-bit DJB2 hash function
uint64_t djb2_64(uint64_t value) noexcept;
//! Computes 64-bit DJB2 hash function
EJ_ALWAYS_INLINE uint64_t djb2_64(int32_t value) noexcept {
	return djb2_64(static_cast<uint32_t>(value));
}
//! Computes 64-bit DJB2 hash function
EJ_ALWAYS_INLINE uint64_t djb2_64(int64_t value) noexcept {
	return djb2_64(static_cast<uint64_t>(value));
}

//! Helper class for computing the hash of a string
template <typename T>
class DJB2Hash;

template <>
class DJB2Hash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const char *s) noexcept {
		return djb2_32(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return djb2_32(s, n);
	}
};

template <>
class DJB2Hash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const char *s) noexcept {
		return djb2_64(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return djb2_64(s, n);
	}
};

}

#endif
