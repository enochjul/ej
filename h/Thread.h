//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>
#include <stdlib.h>

#include <pthread.h>

#include "Common.h"

namespace ej {

class Thread {
public:
	typedef pthread_t native_handle_type;
	typedef void *return_type;

private:
	native_handle_type Handle;

public:
	Thread(return_type (*start_routine)(void *), void *arg) noexcept {
		if (EJ_UNLIKELY(pthread_create(&Handle, nullptr, start_routine, arg) != 0)) {
			abort();
		}
	}
	explicit Thread(native_handle_type handle) noexcept : Handle(handle) {
	}

	Thread(const Thread &) = delete;
	Thread &operator =(const Thread &) = delete;

	void join(return_type *return_value = nullptr) const noexcept {
#ifdef NDEBUG
		auto error_code =
#endif
		pthread_join(Handle, return_value);
#ifdef NDEBUG
		if (EJ_UNLIKELY(error_code != 0)) {
			abort();
		}
#endif
	}

	void detach() const noexcept {
#ifdef NDEBUG
		auto error_code =
#endif
		pthread_detach(Handle);
#ifdef NDEBUG
		if (EJ_UNLIKELY(error_code != 0)) {
			abort();
		}
#endif
	}
};

}
