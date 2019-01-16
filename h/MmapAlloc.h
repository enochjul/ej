//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>

#include <sys/mman.h>

#include "AllocAdapter.h"
#include "Common.h"

namespace ej {

//! Allocates memory using mmap, which is useful for large allocations such as dynamic array
class MmapAllocBase {
protected:
	template <size_t Alignment, bool may_fail>
	static void *allocate(size_t n) noexcept {
		static_assert((Alignment & (Alignment - 1)) == 0 && Alignment <= PAGE_SIZE);
		auto addr = mmap(nullptr, n, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (addr != MAP_FAILED) {
			return addr;
		} else if (may_fail) {
			return nullptr;
		} else {
			abort();
		}
	}

	static void deallocate(void *ptr, size_t n) noexcept {
		munmap(ptr, n);
	}

	template <size_t Alignment, bool may_fail>
	static void *reallocate(void *ptr, size_t old_n, size_t new_n) noexcept {
		void *addr;
		if (EJ_LIKELY(ptr != nullptr)) {
			addr = mremap(ptr, old_n, new_n, MREMAP_MAYMOVE);
		} else {
			addr = mmap(nullptr, new_n, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		}
		if (addr != MAP_FAILED) {
			return addr;
		} else if (may_fail) {
			return nullptr;
		} else {
			abort();
		}
	}

public:
	MmapAllocBase(const MmapAllocBase &) = delete;
	MmapAllocBase &operator =(const MmapAllocBase &) = delete;

	static constexpr bool always_zero() noexcept {
		return true;
	}

	static constexpr size_t min_size() noexcept {
		return PAGE_SIZE;
	}

	static constexpr size_t max_size() noexcept {
		return SIZE_MAX;
	}
};

using MmapAlloc = ReallocAdapter<MmapAllocBase>;

}
