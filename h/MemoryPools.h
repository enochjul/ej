//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_MEMORY_POOLS_H
#define EJ_MEMORY_POOLS_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <sys/mman.h>

#include "Common.h"

namespace ej {

enum : size_t {
	MEMORY_POOLS_DEFAULT_POOL_SIZE = PAGE_SIZE < 65536 ? 65536 : PAGE_SIZE
};

//! Allocates memory in large chunks from the OS and splits it into separate blocks.
//! The blocks cannot be deallocated individually.
template <
	//! Size of memory to allocate from the OS
	size_t PoolSize = MEMORY_POOLS_DEFAULT_POOL_SIZE,
	//! Alignment of each block
	size_t Alignment = alignof(max_align_t),
	//! Maximum size allowed, which may be further reduced due to alignment
	size_t SizeMax = SIZE_MAX>
class MemoryPools {
	static_assert((PoolSize % PAGE_SIZE) == 0);

	struct Pool {
		//! Pointer to the next pool.
		Pool *Next;
		//! Number of bytes allocated for the pool, including the header.
		size_t Size;
		//! Start of the first block
		alignas(Alignment) uint8_t Data[];
	};

	//! Pointer to the current pool to allocate from.
	Pool *Pools;
	//! Number of free bytes in the current pool.
	size_t FreeSize;

	//! Allocates memory for a block of the given size (which must be a multiple of the alignment)
	__attribute__((malloc, assume_aligned(Alignment), warn_unused_result)) void *allocate(size_t n) noexcept;

public:
	constexpr MemoryPools() noexcept : Pools(nullptr), FreeSize(0) {
	}
	~MemoryPools() noexcept;

	MemoryPools(const MemoryPools &) = delete;
	MemoryPools &operator =(const MemoryPools &) = delete;

	void clear() noexcept;

	//! Allocates memory for an object of the specified type
	//! If there is not enough memory, returns nullptr if may_fail is true, otherwise aborts
	template <typename T, bool may_fail = false>
	inline T *alloc() noexcept {
		static_assert(alignof(T) <= Alignment);

		if (sizeof(T) <= (SizeMax - (offsetof(Pool, Data) + PAGE_SIZE - 1) - (Alignment - 1))) {
			auto *new_block = static_cast<T *>(allocate(((sizeof(T) + Alignment - 1) / Alignment) * Alignment));
			if (may_fail || new_block != nullptr) {
				return new_block;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Allocates memory for an object of the specified type, and returns nullptr if there is not enough memory
	template <typename T>
	inline T *try_alloc() noexcept {
		return alloc<T, true>();
	}

	//! Allocates memory for an array of the specified type
	//! If there is not enough memory, returns nullptr if may_fail is true, otherwise aborts
	template <typename T, size_t sentinel_n = 0, bool may_fail = false>
	inline T *alloc_array(size_t n) noexcept {
		static_assert(alignof(T) <= Alignment);
		static_assert(sentinel_n <= (SizeMax - (offsetof(Pool, Data) + PAGE_SIZE - 1) - (Alignment - 1)) / sizeof(T));

		size_t size;
		if (n <= ((SizeMax - (offsetof(Pool, Data) + PAGE_SIZE - 1) - (Alignment - 1)) / sizeof(T) - sentinel_n)) {
			size = ((n * sizeof(T) + sentinel_n * sizeof(T) + Alignment - 1) / Alignment) * Alignment;
			auto *new_block = static_cast<T *>(allocate(size));
			if (may_fail || new_block != nullptr) {
				return new_block;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Allocates memory for an array of the specified type, and returns nullptr if there is not enough memory
	template <typename T, size_t sentinel_n = 0>
	inline T *try_alloc_array(size_t n) noexcept {
		return alloc_array<T, sentinel_n, true>(n);
	}

	//! Allocates memory for a flexible array of the specified type
	//! If there is not enough memory, returns nullptr if may_fail is true, otherwise aborts
	template <typename T, size_t sentinel_n = 0, bool may_fail = false>
	inline T *alloc_flexible_array(size_t n) noexcept {
		static_assert(alignof(T) <= Alignment);
		static_assert(sentinel_n <= ((SizeMax - (offsetof(Pool, Data) + PAGE_SIZE - 1) - (T::sizeofHeader() + Alignment - 1)) / sizeof(typename T::value_type)));

		size_t size;
		if (n <= ((SizeMax - (offsetof(Pool, Data) + PAGE_SIZE - 1) - (T::sizeofHeader() + Alignment - 1)) / sizeof(typename T::value_type) - sentinel_n)) {
			size = ((n * sizeof(typename T::value_type) + sentinel_n * sizeof(typename T::value_type) + T::sizeofHeader() + Alignment - 1) / Alignment) * Alignment;
			auto *new_block = static_cast<T *>(allocate(size));
			if (may_fail || new_block != nullptr) {
				return new_block;
			}
		} else if (may_fail) {
			return nullptr;
		}
		abort();
	}

	//! Allocates memory for a flexible array of the specified type, and returns nullptr if there is not enough memory
	template <typename T, size_t sentinel_n = 0>
	inline T *try_alloc_flexible_array(size_t n) noexcept {
		return alloc_flexible_array<T, sentinel_n, true>(n);
	}
};

template <size_t PoolSize, size_t Alignment, size_t SizeMax>
MemoryPools<PoolSize, Alignment, SizeMax>::~MemoryPools() noexcept {
	auto pool = Pools;
	if (pool != nullptr) {
		do {
			auto next_pool = pool->Next;
			munmap(pool, pool->Size);
			pool = next_pool;
		} while (pool != nullptr);
	}
}

template <size_t PoolSize, size_t Alignment, size_t SizeMax>
void MemoryPools<PoolSize, Alignment, SizeMax>::clear() noexcept {
	auto pool = Pools;
	if (pool != nullptr) {
		do {
			auto next_pool = pool->Next;
			munmap(pool, pool->Size);
			pool = next_pool;
		} while (pool != nullptr);
	}
	Pools = nullptr;
	FreeSize = 0;
}

template <size_t PoolSize, size_t Alignment, size_t SizeMax>
void *MemoryPools<PoolSize, Alignment, SizeMax>::allocate(size_t n) noexcept {
	Pool *pool;
	void *new_block;
	size_t free_size, new_free_size, pool_size;

	assert((n % Alignment) == 0);
	assert(n < (SizeMax - (offsetof(Pool, Data) + PAGE_SIZE - 1)));

	//Check if there is enough free space in the current pool
	free_size = FreeSize;
	if (EJ_LIKELY(free_size >= n)) {
		pool = Pools;
		FreeSize = free_size - n;
		new_block = reinterpret_cast<uint8_t *>(pool) + (pool->Size - free_size);
		return new_block;
	} else {
		//Check if the allocation size + header is greater than the minimum pool size,
		//and round it to a multiple of the page size
		n += offsetof(Pool, Data);
		pool_size = PoolSize;
		if (EJ_UNLIKELY(n > pool_size)) {
			pool_size = ((n + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
		}

		pool = static_cast<Pool *>(mmap(
			nullptr,
			pool_size,
			PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS | MAP_PRIVATE,
			-1,
			0));
		if (pool != MAP_FAILED) {
			auto pools = Pools;
			pool->Size = pool_size;
			//Check if the new pool has more free space than the current pool, and set it as the new pool if it is
			//Otherwise insert the new pool after the current pool
			new_free_size = pool_size - n;
			if (new_free_size >= free_size) {
				pool->Next = pools;
				Pools = pool;
				FreeSize = new_free_size;
			} else {
				pool->Next = pools->Next;
				pools->Next = pool;
			}
			new_block = pool->Data;
			return new_block;
		} else {
			return nullptr;
		}
	}
}

}

#endif
