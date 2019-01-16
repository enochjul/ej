//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>

#include <pthread.h>

namespace ej {

//! Represents a mutex that allows multiple readers but only a single writer to shared data.
class RWMutex {
	pthread_rwlock_t myRWMutex;

public:
	enum : bool {
		Exclusive = false
	};

	RWMutex() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_rwlock_init(&myRWMutex, nullptr);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}

	~RWMutex() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_rwlock_destroy(&myRWMutex);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}

	void rdlock() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_rwlock_rdlock(&myRWMutex);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}

	void rdunlock() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_rwlock_unlock(&myRWMutex);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}

	void wrlock() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_rwlock_wrlock(&myRWMutex);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}

	void wrunlock() noexcept {
#ifndef NDEBUG
		auto error_code =
#endif
		pthread_rwlock_unlock(&myRWMutex);
#ifndef NDEBUG
		assert(error_code == 0);
#endif
	}
};

}
