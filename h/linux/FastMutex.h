//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_FAST_MUTEX_H
#define EJ_FAST_MUTEX_H

#include <assert.h>
#include <stdint.h>

#include "../Common.h"
#include "futex_ops.h"

namespace ej {

//! Implements a simple mutex based on Linux futex, as described in Ulrich Drepper's "Futexes are Tricky" at
//! https://www.akkadia.org/drepper/futex.pdf
class FastMutex {
private:
	enum : uint32_t {
		Unlocked = 0,
		Locked = 1,
		LockedWithWaiters = 2,
	};

	uint32_t State;

public:
	enum : bool {
		Exclusive = true
	};

	constexpr FastMutex() noexcept : State(Unlocked) {
	}
#ifndef NDEBUG
	~FastMutex() noexcept {
		assert(State == Unlocked);
	}
#endif

	FastMutex(const FastMutex &) = delete;
	FastMutex &operator =(const FastMutex &) = delete;

	void lock() noexcept {
		uint32_t state;
		if ((state = compare_and_exchange_value_relaxed<uint32_t>(&State, Unlocked, Locked)) != Unlocked) {
			if (state != LockedWithWaiters) {
				state = __atomic_exchange_n(&State, LockedWithWaiters, __ATOMIC_RELAXED);
			}
			while (state != Unlocked) {
				futex_wait(&State, LockedWithWaiters);
				state = __atomic_exchange_n(&State, LockedWithWaiters, __ATOMIC_RELAXED);
			}
		}
		__atomic_thread_fence(__ATOMIC_ACQUIRE);
	}

	void unlock() noexcept {
		assert(State != Unlocked);
		__atomic_thread_fence(__ATOMIC_RELEASE);
		if (__atomic_fetch_sub(&State, 1, __ATOMIC_RELAXED) != Locked) {
			__atomic_store_n(&State, Unlocked, __ATOMIC_RELAXED);
			futex_wake(&State);
		}
	}

	template <class T>
	void unlock_and_wake_futex(T *value) noexcept {
		assert(State != Unlocked);
		__atomic_thread_fence(__ATOMIC_RELEASE);
		if (__atomic_fetch_sub(&State, 1, __ATOMIC_RELAXED) != Locked) {
			__atomic_store_n(&State, Unlocked, __ATOMIC_RELAXED);
			futex_requeue(value, 0, 1, &State);
			futex_wake(&State);
		} else {
			futex_wake(value);
		}
	}

	uint32_t *data() noexcept {
		return &State;
	}
};

}

#endif
