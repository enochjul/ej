//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_FAST_RW_MUTEX_H
#define EJ_FAST_RW_MUTEX_H

#include <stdint.h>

#include "futex_ops.h"

namespace ej {

//! Implements a read/write mutex based on furlocks by Rusty Russell at http://www.kernel.org/pub/linux/kernel/people/rusty/futex-2.2.tar.gz
class FastRWMutex {
	enum : int32_t {
		MutexUnlocked = 0,
		MutexLocked = 1,
		MutexLockedWithWaiters = 2,
		WriteMutexUnlocked = 1,
		WriteMutexLocked = 0,
		WriteMutexLockedWithWaiters = -1,
	};

	int32_t MutexState;
	int32_t WriteMutexState;
	int32_t Count;

public:
	enum : bool {
		Exclusive = false
	};

	constexpr FastRWMutex() noexcept : MutexState(MutexUnlocked), WriteMutexState(WriteMutexLocked), Count(0) {
	}

	FastRWMutex(const FastRWMutex &) = delete;
	FastRWMutex &operator =(const FastRWMutex &) = delete;

	void rdlock() noexcept {
		int32_t mutex_state;

		if ((mutex_state = compare_and_exchange_value_relaxed<int32_t>(&MutexState, MutexUnlocked, MutexLocked)) != MutexUnlocked) {
			if (mutex_state != MutexLockedWithWaiters) {
				mutex_state = __atomic_exchange_n(&MutexState, MutexLockedWithWaiters, __ATOMIC_RELAXED);
			}
			while (mutex_state != MutexUnlocked) {
				futex_wait(&MutexState, MutexLockedWithWaiters);
				mutex_state = __atomic_exchange_n(&MutexState, MutexLockedWithWaiters, __ATOMIC_RELAXED);
			}
		}

		__atomic_fetch_add(&Count, 1, __ATOMIC_RELAXED);

		if (__atomic_fetch_sub(&MutexState, 1, __ATOMIC_RELAXED) != MutexLocked) {
			__atomic_store_n(&MutexState, MutexUnlocked, __ATOMIC_RELAXED);
			futex_wake(&MutexState);
		}

		__atomic_thread_fence(__ATOMIC_ACQUIRE);
	}

	void rdunlock() noexcept {
		__atomic_thread_fence(__ATOMIC_RELEASE);
		if (__atomic_sub_fetch(&Count, 1, __ATOMIC_RELAXED) < 0) {
			if (__atomic_add_fetch(&WriteMutexState, 1, __ATOMIC_RELAXED) == WriteMutexLocked) {
				__atomic_store_n(&WriteMutexState, WriteMutexUnlocked, __ATOMIC_RELAXED);
				futex_wake(&WriteMutexState);
			}
		}
	}

	void wrlock() noexcept {
		int32_t mutex_state, write_mutex_state;

		if ((mutex_state = compare_and_exchange_value_relaxed<int32_t>(&MutexState, MutexUnlocked, MutexLocked)) != MutexUnlocked) {
			if (mutex_state != MutexLockedWithWaiters) {
				mutex_state = __atomic_exchange_n(&MutexState, MutexLockedWithWaiters, __ATOMIC_RELAXED);
			}
			while (mutex_state != MutexUnlocked) {
				futex_wait(&MutexState, MutexLockedWithWaiters);
				mutex_state = __atomic_exchange_n(&MutexState, MutexLockedWithWaiters, __ATOMIC_RELAXED);
			}
		}

		if (__atomic_sub_fetch(&Count, 1, __ATOMIC_RELAXED) >= 0) {
			if ((write_mutex_state = compare_and_exchange_value_relaxed<int32_t>(&WriteMutexState, WriteMutexUnlocked, WriteMutexLocked)) != WriteMutexUnlocked) {
				if (write_mutex_state != WriteMutexLockedWithWaiters) {
					write_mutex_state = __atomic_exchange_n(&WriteMutexState, WriteMutexLockedWithWaiters, __ATOMIC_RELAXED);
				}
				while (write_mutex_state != WriteMutexUnlocked) {
					futex_wait(&WriteMutexState, WriteMutexLockedWithWaiters);
					write_mutex_state = __atomic_exchange_n(&WriteMutexState, WriteMutexLockedWithWaiters, __ATOMIC_RELAXED);
				}
			}
		}

		__atomic_thread_fence(__ATOMIC_ACQUIRE);
	}

	void wrunlock() noexcept {
		__atomic_thread_fence(__ATOMIC_RELEASE);
		__atomic_store_n(&Count, 0, __ATOMIC_RELAXED);
		if (__atomic_fetch_sub(&MutexState, 1, __ATOMIC_RELAXED) != MutexLocked) {
			__atomic_store_n(&MutexState, MutexUnlocked, __ATOMIC_RELAXED);
			futex_wake(&MutexState);
		}
	}
};

}

#endif
