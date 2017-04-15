//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_C_ALLOC_H
#define EJ_C_ALLOC_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "AllocAdapter.h"

namespace ej {

//! Helper functions to access the C memory allocation functions such as malloc and realloc
class CAllocBase {
protected:
	template <typename T>
	static T *allocate(size_t n) noexcept {
		if (alignof(T) <= alignof(max_align_t)) {
			return static_cast<T *>(malloc(n));
		} else {
			assert(n % alignof(T) == 0);
			return static_cast<T *>(aligned_alloc(alignof(T), n));
		}
	}

	static void deallocate(void *ptr, size_t) noexcept {
		free(ptr);
	}

	template <typename T>
	static T *reallocate(T *ptr, size_t n) noexcept {
		static_assert(alignof(T) <= alignof(max_align_t));
		return static_cast<T *>(::realloc(ptr, n));
	}

public:
	CAllocBase(const CAllocBase &) = delete;
	CAllocBase &operator =(const CAllocBase &) = delete;
};

using CAlloc = ReallocAdapter<CAllocBase>;

}

#endif
