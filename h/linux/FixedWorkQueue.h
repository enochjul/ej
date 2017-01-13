//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_FIXED_WORK_QUEUE_H
#define EJ_FIXED_WORK_QUEUE_H

#include <assert.h>
#include <stdint.h>

#include "FastMutex.h"
#include "futex.h"

namespace ej {

//! A fixed size queue that allows multiple threads to add/remove values.
//! Values should be simple trivially copyable types as they are passed/returned by value.
template <typename T, uint32_t N = 64>
class FixedWorkQueue {
	static_assert(N > 0);

public:
	typedef T value_type;

private:
	FastMutex myMutex;
	//! Number of unused elements, and used as a futex by readers to signal writers
	uint32_t Unused;
	//! Number of filled elements, and used as a futex by writers to signal readers
	uint32_t Filled;
	//! Index to the first element
	uint32_t Head;
	value_type Data[N];

public:
	constexpr FixedWorkQueue() noexcept : myMutex(), Unused(N), Filled(0), Head(0) {
	}

	FixedWorkQueue(const FixedWorkQueue<T> &) = delete;
	FixedWorkQueue<T> &operator =(const FixedWorkQueue<T> &) = delete;

	void add(T value) noexcept;
	T remove() noexcept;
};

template <typename T, uint32_t N>
void FixedWorkQueue<T, N>::add(T value) noexcept {
	uint32_t unused, filled, tail;

	myMutex.lock();

	//Wait until there is at least one unused element
	while ((unused = __atomic_load_n(&Unused, __ATOMIC_RELAXED)) == 0) {
		myMutex.unlock();
		futex_wait(&Unused, 0);
		myMutex.lock();
	}

	//Find the first unused element and put the value
	filled = __atomic_load_n(&Filled, __ATOMIC_RELAXED);
	assert(unused + filled == N);

	if ((N & (N - 1)) == 0) {
		tail = (__atomic_load_n(&Head, __ATOMIC_RELAXED) + filled) % N;
	} else {
		tail = __atomic_load_n(&Head, __ATOMIC_RELAXED) + filled;
		if (tail >= N) {
			tail -= N;
		}
	}
	Data[tail] = value;

	//Adjust the number of unused/filled elements
	__atomic_store_n(&Unused, unused - 1, __ATOMIC_RELAXED);
	__atomic_store_n(&Filled, filled + 1, __ATOMIC_RELAXED);

	//Unlock and wake any waiting reader(s)
	myMutex.unlock();
	futex_wake(&Filled);
}

template <typename T, uint32_t N>
T FixedWorkQueue<T, N>::remove() noexcept {
	uint32_t unused, filled, head;

	myMutex.lock();

	//Wait until there is at least one filled element
	while ((filled = __atomic_load_n(&Filled, __ATOMIC_RELAXED)) == 0) {
		myMutex.unlock();
		futex_wait(&Filled, 0);
		myMutex.lock();
	}

	//Copy the head element and adjust head to point to the next element
	head = __atomic_load_n(&Head, __ATOMIC_RELAXED);
	T value = Data[head];

	if ((N & (N - 1)) == 0) {
		head = (head + 1) % N;
	} else {
		head = head + 1;
		if (head >= N) {
			head -= N;
		}
	}
	__atomic_store_n(&Head, head, __ATOMIC_RELAXED);

	//Adjust the number of unused/filled elements
	unused = __atomic_load_n(&Unused, __ATOMIC_RELAXED);
	assert(filled + unused == N);
	__atomic_store_n(&Unused, unused + 1, __ATOMIC_RELAXED);
	__atomic_store_n(&Filled, filled - 1, __ATOMIC_RELAXED);

	//Unlock and wake any waiting writer(s)
	myMutex.unlock();
	futex_wake(&Unused);

	return value;
}

}

#endif
