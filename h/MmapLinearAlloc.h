//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

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

	__attribute__((returns_nonnull)) static void *mark_nonnull(void *ptr) noexcept {
		assert(ptr != nullptr);
		return ptr;
	}

	template <bool may_fail>
	__attribute__((malloc, alloc_align(1), alloc_size(2), warn_unused_result)) static void *commit_and_allocate(size_t alignment, size_t n) noexcept;

public:
	constexpr static bool HasDeallocate = false;
	constexpr static bool AlwaysZero = false;

protected:
	template <size_t Alignment, bool may_fail>
	__attribute__((malloc, assume_aligned(Alignment), alloc_size(1), warn_unused_result)) static void *allocate(size_t n) noexcept {
		static_assert(Alignment > 0 && Alignment <= PAGE_SIZE && (Alignment & (Alignment - 1)) == 0);
		assert(Start != nullptr);
		assert(n % Alignment == 0);

		auto free_size = FreeSize;
		if (Alignment > MinimumAlignment) {
			auto aligned_free_size = free_size & ~(Alignment - 1);
			if (EJ_LIKELY(n <= aligned_free_size)) {
				auto end = static_cast<char *>(End);
				auto new_block = end + (free_size & (Alignment - 1));
				End = new_block + n;
				FreeSize = aligned_free_size - n;
				if (ZeroPadding) {
					for (auto pad_i = end; pad_i < new_block; ++pad_i) {
						*pad_i = 0;
					}
				}
				return mark_nonnull(new_block);
			} else {
				return commit_and_allocate<may_fail>(Alignment, n);
			}
		} else {
			auto new_block = static_cast<char *>(End);
			if (EJ_LIKELY(n <= free_size)) {
				if (Alignment == MinimumAlignment) {
					End = new_block + n;
					FreeSize = free_size - n;
					return mark_nonnull(new_block);
				} else {
					auto aligned_n = ((n + MinimumAlignment - 1) / MinimumAlignment) * MinimumAlignment;
					auto aligned_end = new_block + aligned_n;
					End = aligned_end;
					FreeSize = free_size - aligned_n;
					if (ZeroPadding) {
						for (auto pad_i = new_block + n; pad_i < aligned_end; ++pad_i) {
							*pad_i = 0;
						}
					}
					return mark_nonnull(new_block);
				}
			} else {
				return commit_and_allocate<may_fail>(Alignment, n);
			}
		}

		if (may_fail) {
			return nullptr;
		} else {
			abort();
		}
	}

	static void deallocate(void *, size_t) noexcept {
		//Does nothing as monotonic allocators do not support arbitrary deallocations
	}

public:
	MmapLinearAllocBase(const MmapLinearAllocBase &) = delete;
	MmapLinearAllocBase &operator =(const MmapLinearAllocBase &) = delete;

	static constexpr size_t min_size() noexcept {
		return 1;
	}

	static constexpr size_t max_size() noexcept {
		return SIZE_MAX;
	}

	static void *getStart() noexcept {
		return Start;
	}

	static void *getEnd() noexcept {
		return End;
	}

	static size_t size() noexcept {
		return reinterpret_cast<uintptr_t>(End) - reinterpret_cast<uintptr_t>(Start);
	}

	static bool initialize(size_t n) noexcept {
		assert(Start == nullptr);
		if (n <= max_size() - (CommitGranularity - 1)) {
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
		assert(Start != nullptr);
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
template <bool may_fail>
void *MmapLinearAllocBase<MinimumAlignment, ZeroPadding, CommitGranularity, Identifier>::commit_and_allocate(size_t alignment, size_t n) noexcept {
	assert(Start != nullptr);
	assert(alignment > 0 && alignment <= PAGE_SIZE && (alignment & (alignment - 1)) == 0);
	assert(n % alignment == 0);

	auto end = static_cast<char *>(End);
	auto free_size = FreeSize;
	auto alignment_mask = alignment - 1;
	auto new_block = end + (free_size & alignment_mask);
	auto aligned_free_size = free_size & ~alignment_mask;
	auto max_aligned_free_size = reinterpret_cast<uintptr_t>(Reserved) - reinterpret_cast<uintptr_t>(new_block);
	if (EJ_LIKELY(n <= max_aligned_free_size)) {
		auto allocated_end = end + free_size;
		auto additional_allocated_size = reinterpret_cast<uintptr_t>(new_block) + n - reinterpret_cast<uintptr_t>(allocated_end);
		additional_allocated_size = ((additional_allocated_size + CommitGranularity - 1) / CommitGranularity) * CommitGranularity;
		assert(additional_allocated_size > 0);
		if (EJ_LIKELY(mmap(allocated_end, additional_allocated_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0) != MAP_FAILED)) {
			if (ZeroPadding) {
				for (auto pad_i = static_cast<char *>(End); pad_i < new_block; ++pad_i) {
					*pad_i = 0;
				}
			}
			auto aligned_n = ((n + MinimumAlignment - 1) / MinimumAlignment) * MinimumAlignment;
			auto new_aligned_end = new_block + aligned_n;
			End = new_aligned_end;
			FreeSize = aligned_free_size + additional_allocated_size - aligned_n;
			if (ZeroPadding) {
				for (auto pad_i = new_block + n; pad_i < new_aligned_end; ++pad_i) {
					*pad_i = 0;
				}
			}
			return new_block;
		}
	}

	if (may_fail) {
		return nullptr;
	} else {
		abort();
	}
}

template <
	size_t MinimumAlignment = 1,
	bool ZeroPadding = false,
	size_t CommitGranularity = 4194304,
	size_t Identifier = 0>
using MmapLinearAlloc = MonotonicAllocAdapter<MmapLinearAllocBase<MinimumAlignment, ZeroPadding, CommitGranularity, Identifier> >;

}
