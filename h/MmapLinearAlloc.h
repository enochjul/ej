//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_MMAP_LINEAR_ALLOC_H
#define EJ_MMAP_LINEAR_ALLOC_H

#include <assert.h>
#include <stddef.h>

#include <sys/mman.h>

#include "Common.h"
#include "MonotonicAllocAdapter.h"

namespace ej {

template <
	size_t MinimumAlignment = 1,
	bool ZeroPadding = false,
	size_t CommitGranularity = 4194304,
	size_t Identifier = 0>
class MmapLinearAllocBase {
	static_assert(MinimumAlignment > 0 && MinimumAlignment <= PAGE_SIZE && (MinimumAlignment & (MinimumAlignment - 1)) == 0);
	static_assert(CommitGranularity > 0 && CommitGranularity % PAGE_SIZE == 0);

	static void *Start;
	static void *Reserved;
	static void *End;
	static size_t FreeSize;

	__attribute__((malloc, alloc_align(1), alloc_size(2))) static void *allocate_raw(size_t alignment, size_t n) noexcept;

protected:
	template <typename T>
	static T *allocate(size_t n) noexcept {
		return static_cast<T *>(allocate_raw(MinimumAlignment > 1 && alignof(T) < MinimumAlignment ? alignof(T) : alignof(T), n));
	}

	static void deallocate(void *, size_t) noexcept {
		//Does nothing as monotonic allocators do not support arbitrary deallocations
	}

public:
	MmapLinearAllocBase(const MmapLinearAllocBase &) = delete;
	MmapLinearAllocBase &operator =(const MmapLinearAllocBase &) = delete;

	static void *getEnd() noexcept {
		return End;
	}

	static size_t size() noexcept {
		return reinterpret_cast<uintptr_t>(End) - reinterpret_cast<uintptr_t>(Start);
	}

	static bool initialize(size_t n) noexcept {
		assert(Start == nullptr);
		if (n <= SIZE_MAX - (CommitGranularity - 1)) {
			n = (n + (CommitGranularity - 1)) & ~(CommitGranularity - 1);
			auto new_start = mmap(nullptr, n, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
			if (new_start != MAP_FAILED) {
				Start = new_start;
				Reserved = static_cast<char *>(new_start) + n;
				End = new_start;
				FreeSize = 0;
				return true;
			}
		}
		return false;
	}

	static void destroy() noexcept {
		assert(Start != nullptr);
		munmap(Start, reinterpret_cast<uintptr_t>(Reserved) - reinterpret_cast<uintptr_t>(Start));

#ifndef NDEBUG
		Start = nullptr;
		Reserved = nullptr;
		End = nullptr;
		FreeSize = 0;
#endif
	}

	static void pop_frame(void *frame_start) noexcept {
		auto frame_end = End;
		End = frame_start;
		FreeSize += reinterpret_cast<uintptr_t>(frame_end) - reinterpret_cast<uintptr_t>(frame_start);
	}
};

template <size_t MinimumAlignment, bool ZeroPadding, size_t CommitGranularity, size_t Identifier> void *MmapLinearAllocBase<MinimumAlignment, ZeroPadding, CommitGranularity, Identifier>::Start = nullptr;
template <size_t MinimumAlignment, bool ZeroPadding, size_t CommitGranularity, size_t Identifier> void *MmapLinearAllocBase<MinimumAlignment, ZeroPadding, CommitGranularity, Identifier>::Reserved = nullptr;
template <size_t MinimumAlignment, bool ZeroPadding, size_t CommitGranularity, size_t Identifier> void *MmapLinearAllocBase<MinimumAlignment, ZeroPadding, CommitGranularity, Identifier>::End = nullptr;
template <size_t MinimumAlignment, bool ZeroPadding, size_t CommitGranularity, size_t Identifier> size_t MmapLinearAllocBase<MinimumAlignment, ZeroPadding, CommitGranularity, Identifier>::FreeSize = 0;

template <size_t MinimumAlignment, bool ZeroPadding, size_t CommitGranularity, size_t Identifier> 
void *MmapLinearAllocBase<MinimumAlignment, ZeroPadding, CommitGranularity, Identifier>::allocate_raw(size_t alignment, size_t n) noexcept {
	assert(alignment > 0 && alignment <= PAGE_SIZE && (alignment & (alignment - 1)) == 0);

	auto end = static_cast<char *>(End);
	auto free_size = FreeSize;
	auto align_mask = alignment - 1;
	auto new_block = static_cast<char *>(end) + (free_size & align_mask);
	auto aligned_free_size = free_size & ~align_mask;
	if (EJ_UNLIKELY(n > aligned_free_size)) {
		auto max_aligned_free_size = reinterpret_cast<uintptr_t>(Reserved) - reinterpret_cast<uintptr_t>(new_block);
		if (EJ_LIKELY(n <= max_aligned_free_size)) {
			auto allocated_end = end + free_size;
			auto additional_allocated_size = reinterpret_cast<uintptr_t>(new_block) + n - reinterpret_cast<uintptr_t>(allocated_end);
			additional_allocated_size = ((additional_allocated_size + CommitGranularity - 1) / CommitGranularity) * CommitGranularity;
			aligned_free_size += additional_allocated_size;
			if (EJ_UNLIKELY(mmap(allocated_end, additional_allocated_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0) == MAP_FAILED)) {
				return nullptr;
			}
		} else {
			return nullptr;
		}
	}

	if (ZeroPadding) {
		for (auto pad_i = end; pad_i < new_block; ++pad_i) {
			*pad_i = 0;
		}
	}
	End = new_block + n;
	FreeSize = aligned_free_size - n;
	return new_block;
}

template <
	size_t MinimumAlignment = 1,
	bool ZeroPadding = false,
	size_t CommitGranularity = 4194304,
	size_t Identifier = 0>
using MmapLinearAlloc = MonotonicAllocAdapter<MmapLinearAllocBase<MinimumAlignment, ZeroPadding, CommitGranularity, Identifier> >;

}

#endif
