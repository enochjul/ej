//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_ROTATE_H
#define EJ_ROTATE_H

#include <assert.h>
#include <stdint.h>

#if defined(__i386__) || defined(__amd64__)
	#include <x86intrin.h>
#endif

namespace ej {

//! Rotates the specified 32-bit value towards the left
inline uint32_t rotate_left_32(
	//! Value to rotate
	uint32_t value,
	//! Count to rotate, which must be in the range of [1, 31]
	unsigned n) noexcept {
	assert(n > 0 && n < 32);
#if defined(__i386__) || defined(__amd64__)
	return __rold(value, n);
#else
	return (value << n) | (value >> (32 - n));
#endif
}

//! Rotates the specified 64-bit value towards the left
inline uint64_t rotate_left_64(
	//! Value to rotate
	uint64_t value,
	//! Count to rotate, which must be in the range of [1, 63]
	unsigned n) noexcept {
	assert(n > 0 && n < 64);
#if defined(__i386__) || defined(__amd64__)
	return __rolq(value, n);
#else
	return (value << n) | (value >> (64 - n));
#endif
}

template <typename T>
T rotate_left(T value, unsigned n) noexcept;

template <>
inline uint32_t rotate_left(uint32_t value, unsigned n) noexcept {
	return rotate_left_32(value, n);
}

template <>
inline uint64_t rotate_left(uint64_t value, unsigned n) noexcept {
	return rotate_left_64(value, n);
}

//! Rotates the specified 32-bit value towards the right
inline uint32_t rotate_right_32(
	//! Value to rotate
	uint32_t value,
	//! Count to rotate, which must be in the range of [1, 31]
	unsigned n) noexcept {
	assert(n > 0 && n < 32);
#if defined(__i386__) || defined(__amd64__)
	return __rord(value, n);
#else
	return (value >> n) | (value << (32 - n));
#endif
}

//! Rotates the specified 64-bit value towards the right
inline uint64_t rotate_right_64(
	//! Value to rotate
	uint64_t value,
	//! Count to rotate, which must be in the range of [1, 63]
	unsigned n) noexcept {
	assert(n > 0 && n < 64);
#if defined(__i386__) || defined(__amd64__)
	return __rorq(value, n);
#else
	return (value >> n) | (value << (64 - n));
#endif
}

template <typename T>
T rotate_right(T value, unsigned n) noexcept;

template <>
inline uint32_t rotate_right(uint32_t value, unsigned n) noexcept {
	return rotate_right_32(value, n);
}

template <>
inline uint64_t rotate_right(uint64_t value, unsigned n) noexcept {
	return rotate_right_64(value, n);
}

}

#endif
