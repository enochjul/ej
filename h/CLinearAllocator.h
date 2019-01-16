//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "Common.h"
#include "MonotonicAllocatorAdapter.h"

namespace ej {

template <size_t MinimumAlignment, bool ZeroPadding>
class CLinearAllocatorBase {
	static_assert(MinimumAlignment > 0 && MinimumAlignment <= alignof(max_align_t) && (MinimumAlignment & (MinimumAlignment - 1)) == 0);

	void *Start;
	void *End;
	size_t FreeSize;

	__attribute__((returns_nonnull)) static void *mark_nonnull(void *ptr) noexcept {
		assert(ptr != nullptr);
		return ptr;
	}

protected:
	template <size_t Alignment, bool may_fail>
	__attribute__((malloc, assume_aligned(Alignment), alloc_size(2), warn_unused_result)) void *allocate(size_t n) noexcept {
		static_assert(Alignment > 0 && Alignment <= alignof(max_align_t) && (Alignment & (Alignment - 1)) == 0);
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
			}
		} else {
			if (EJ_LIKELY(n <= free_size)) {
				if (Alignment == MinimumAlignment) {
					auto new_block = static_cast<char *>(End);
					End = new_block + n;
					FreeSize = free_size - n;
					return mark_nonnull(new_block);
				} else {
					auto end = static_cast<char *>(End);
					auto new_block = end;
					auto aligned_n = ((n + MinimumAlignment - 1) / MinimumAlignment) * MinimumAlignment;
					auto aligned_end = new_block + aligned_n;
					End = aligned_end;
					FreeSize = free_size - aligned_n;
					if (ZeroPadding) {
						end += n;
						for (auto pad_i = end; pad_i < aligned_end; ++pad_i) {
							*pad_i = 0;
						}
					}
					return mark_nonnull(new_block);
				}
			}
		}

		if (may_fail) {
			return nullptr;
		} else {
			abort();
		}
	}

	void deallocate(void *, size_t) noexcept {
	}

public:
#ifndef NDEBUG
	constexpr CLinearAllocatorBase() noexcept : Start(nullptr), End(nullptr), FreeSize(0) {
	}
	~CLinearAllocatorBase() noexcept {
		assert(Start == nullptr);
	}
#else
	constexpr CLinearAllocatorBase() noexcept = default;
#endif

	CLinearAllocatorBase(const CLinearAllocatorBase &) = delete;
	CLinearAllocatorBase &operator =(const CLinearAllocatorBase &) = delete;

	static constexpr bool always_zero() noexcept {
		return false;
	}

	static constexpr size_t min_size() noexcept {
		return 1;
	}

	static constexpr size_t max_size() noexcept {
		return SIZE_MAX;
	}

	void *getEnd() const noexcept {
		return End;
	}

	size_t size() const noexcept {
		return reinterpret_cast<uintptr_t>(End) - reinterpret_cast<uintptr_t>(Start);
	}

	bool initialize(size_t n) noexcept {
		assert(Start == nullptr);
		if (n < max_size() - alignof(max_align_t) + 1) {
			n = ((n + alignof(max_align_t) - 1) / alignof(max_align_t)) * alignof(max_align_t);
			auto new_start = malloc(n);
			if (new_start != nullptr) {
				Start = new_start;
				End = new_start;
				FreeSize = n;
				return true;
			}
		}
		return false;
	}

	void destroy() noexcept {
		assert(Start != nullptr);
		free(Start);
#ifndef NDEBUG
		Start = nullptr;
		End = nullptr;
		FreeSize = 0;
#endif
	}

	void pop_frame(void *frame_start) noexcept {
		assert(Start != nullptr);
		auto frame_end = End;
		End = frame_start;
		FreeSize += reinterpret_cast<uintptr_t>(frame_end) - reinterpret_cast<uintptr_t>(frame_start);
	}
};

template <size_t MinimumAlignment = 1, bool ZeroPadding = false>
using CLinearAllocator = MonotonicAllocatorAdapter<CLinearAllocatorBase<MinimumAlignment, ZeroPadding> >;

}
