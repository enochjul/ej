//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>

#include <pthread.h>

namespace ej {

//! Represents a mutex that allows only one thread at a time to access shared data.
class Mutex {
	pthread_mutex_t myMutex;

public:
	enum : bool {
		Exclusive = true
	};

	constexpr Mutex() noexcept : myMutex(PTHREAD_MUTEX_INITIALIZER) {
	}
	~Mutex() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_mutex_destroy(&myMutex);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}

	Mutex(const Mutex &) = delete;
	Mutex &operator =(const Mutex &) = delete;

	void lock() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_mutex_lock(&myMutex);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}

	void unlock() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_mutex_unlock(&myMutex);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}
};

}
