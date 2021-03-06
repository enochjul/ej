//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "AllocAdapter.h"

namespace ej {

//! Helper functions to access the C memory allocation functions such as malloc and realloc
class CAllocBase {
public:
	constexpr static bool HasDeallocate = true;
	constexpr static bool AlwaysZero = false;

protected:
	template <size_t Alignment, bool may_fail>
	__attribute__((malloc, assume_aligned(Alignment <= alignof(max_align_t) ? alignof(max_align_t) : Alignment), alloc_size(1), warn_unused_result)) static void *allocate(size_t n) noexcept {
		if constexpr (Alignment <= alignof(max_align_t)) {
			auto ptr = malloc(n);
			if (may_fail || ptr != nullptr) {
				return ptr;
			} else {
				abort();
			}
		} else {
			assert(n % Alignment == 0);
			auto ptr = aligned_alloc(Alignment, n);
			if (may_fail || ptr != nullptr) {
				return ptr;
			} else {
				abort();
			}
		}
	}

	static void deallocate(void *ptr, size_t) noexcept {
		free(ptr);
	}

	template <size_t Alignment, bool may_fail>
	static void *reallocate(void *ptr, size_t old_n, size_t new_n) noexcept {
		static_assert(Alignment <= alignof(max_align_t));
		ptr = ::realloc(ptr, new_n);
		if (may_fail || ptr != nullptr) {
			return ptr;
		} else {
			abort();
		}
	}

public:
	CAllocBase(const CAllocBase &) = delete;
	CAllocBase &operator =(const CAllocBase &) = delete;

	static constexpr size_t min_size() noexcept {
		return alignof(max_align_t);
	}

	static constexpr size_t max_size() noexcept {
		return PTRDIFF_MAX <= SIZE_MAX ? PTRDIFF_MAX : SIZE_MAX;
	}
};

using CAlloc = ReallocAdapter<CAllocBase>;

}
