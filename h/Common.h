//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_COMMON_H
#define EJ_COMMON_H

#include <stddef.h>

#define EJ_ALWAYS_INLINE	inline __attribute__((always_inline))
#define EJ_LIKELY(cond)		__builtin_expect(static_cast<bool>(cond), true)
#define EJ_UNLIKELY(cond)	__builtin_expect(static_cast<bool>(cond), false)

namespace ej {

template <typename T>
struct duint {
	T Low;
	T High;
};

template <typename T>
EJ_ALWAYS_INLINE bool operator ==(const duint<T> &a, const duint<T> &b) {
	return a.Low == b.Low && a.High == b.High;
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

}

#endif
