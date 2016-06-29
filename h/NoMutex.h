//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_NO_MUTEX_H
#define EJ_NO_MUTEX_H

#include "MutexTraits.h"

namespace ej {

//! Implements a mutex interface for single threaded code, useful for templates to switch between single thread or multiple threads
class NoMutex {
public:
	enum : bool {
		Exclusive = true
	};

	void lock() noexcept {
	}

	void unlock() noexcept {
	}
};

template <>
struct is_mutex<NoMutex> final {
	enum : bool {
		value = false
	};
};

}

#endif
