//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>
#include <stdint.h>

#define EJ_ALWAYS_INLINE_ATTRIBUTE	__attribute__((always_inline))
#define EJ_ALWAYS_INLINE	inline EJ_ALWAYS_INLINE_ATTRIBUTE
#define EJ_LIKELY(cond)		__builtin_expect(static_cast<bool>(cond), true)
#define EJ_UNLIKELY(cond)	__builtin_expect(static_cast<bool>(cond), false)

namespace ej {

template <typename T, size_t N>
EJ_ALWAYS_INLINE constexpr size_t get_size(const T (&a)[N]) noexcept {
	return N;
}

template <typename T>
EJ_ALWAYS_INLINE constexpr T *lea(T *ptr, uintptr_t offset) noexcept {
	return reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(ptr) + offset);
}

enum : size_t {
	PAGE_SIZE = 4096,
};

//Unfortunately gcc currently does not optimize __atomic_compare_exchange_n() well, and does an extra memory write
//to expected instead of putting it in a register.
//On x86 use the sync builtins to avoid the issue, but the extra memory barrier implied by sync may not work well for
//other architectures.

template <typename T, bool weak = false>
inline bool compare_and_exchange_bool_relaxed(T *ptr, T expected, T desired) noexcept {
#if defined(__i386__) || defined(__amd64__)
	return __sync_bool_compare_and_swap(ptr, expected, desired);
#else
	return __atomic_compare_exchange_n(ptr, &expected, desired, weak, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
#endif
}

template <typename T, bool weak = false>
inline T compare_and_exchange_value_relaxed(T *ptr, T expected, T desired) noexcept {
#if defined(__i386__) || defined(__amd64__)
	return __sync_val_compare_and_swap(ptr, expected, desired);
#else
	__atomic_compare_exchange_n(ptr, &expected, desired, weak, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return expected;
#endif
}

#ifndef EJ_WORD_SIZE
#ifdef __LP64__
#define EJ_WORD_SIZE 64
#define EJ_POINTER_SIZE 64
#else
#define EJ_WORD_SIZE 32
#define EJ_POINTER_SIZE 32
#endif
#endif

}
