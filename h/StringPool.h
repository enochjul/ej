//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_STRING_POOL_H
#define EJ_STRING_POOL_H

#include <string.h>

#include <atomic>
#include <limits>
#include <utility>

#include "MemoryPools.h"
#include "NoMutex.h"
#include "String.h"

namespace ej {

template <size_t NumberOfRegionBits, class MutexType, class StringType, typename SizeType, size_t PoolSize>
class StringPoolBase : public StringType::hasher_type {
	typedef typename StringType::hasher_type super;

public:
	typedef typename super::value_type hash_type;
	typedef SizeType size_type;

	struct InsertReturnType {
		const StringType *Value;
		bool Inserted;
	};

protected:
	//! Holds a mutable mutex and converts lock/unlock operations to const to allow const read operations on the data protected by the mutex
	template <typename T, bool real_mutex = is_mutex<T>::value, bool exclusive = T::Exclusive>
	struct RegionBase;

	template <typename T, bool real_mutex>
	struct RegionBase<T, real_mutex, true> {
		mutable T Mutex;

		void lock() const noexcept {
			Mutex.lock();
		}

		void unlock() const noexcept {
			Mutex.unlock();
		}
	};

	template <typename T, bool real_mutex>
	struct RegionBase<T, real_mutex, false> {
		mutable T Mutex;

		void rdlock() const noexcept {
			Mutex.rdlock();
		}

		void rdunlock() const noexcept {
			Mutex.rdunlock();
		}

		void wrlock() const noexcept {
			Mutex.wrlock();
		}

		void wrunlock() const noexcept {
			Mutex.wrunlock();
		}
	};

	template <typename T>
	struct RegionBase<T, false, true> {
		void lock() const noexcept {
		}

		void unlock() const noexcept {
		}
	};

	struct Region : public RegionBase<MutexType> {
		typedef RegionBase<MutexType> super;

		StringType **Buckets;
		hash_type Mask;
		size_type Size;
		MemoryPools<PoolSize, alignof(StringType), std::numeric_limits<size_type>::max()> Pools;

		Region() noexcept : super(), Buckets(nullptr), Mask(0), Size(0), Pools() {
		}
		~Region() noexcept {
			auto buckets = Buckets;
			if (buckets != nullptr) {
				auto mask = Mask;
				munmap(buckets, (mask + 1) * sizeof(StringType *));
			}
		}
	};

	enum : size_t {
		NUMBER_OF_REGIONS_BITS = NumberOfRegionBits,
		NUMBER_OF_REGIONS = 1 << NUMBER_OF_REGIONS_BITS,
		INITIAL_NUMBER_OF_BUCKETS = PAGE_SIZE / sizeof(StringType *),
	};

	Region Regions[NUMBER_OF_REGIONS];

	void initialize() noexcept;

public:
	template <typename ... Args>
	StringPoolBase(Args && ... args) noexcept : super(std::forward<Args>(args) ...) {
		initialize();

		if (is_mutex<MutexType>::value) {
			std::atomic_thread_fence(std::memory_order_release);
		}
	}
	~StringPoolBase() noexcept {
		if (is_mutex<MutexType>::value) {
			std::atomic_thread_fence(std::memory_order_acquire);
		}
	}

	StringPoolBase(const StringPoolBase &) = delete;
	StringPoolBase &operator =(const StringPoolBase &) = delete;
};

//! Represents a set of immutable strings
template <size_t NumberOfRegionBits = 8, class MutexType = NoMutex, class StringType = String<>, typename SizeType = typename StringType::hash_type, size_t PoolSize = MEMORY_POOLS_DEFAULT_POOL_SIZE, bool exclusive_mutex = MutexType::Exclusive>
class StringPool;

template <size_t NumberOfRegionBits, class MutexType, class StringType, typename SizeType, size_t PoolSize>
class StringPool<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize, true> final : public StringPoolBase<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize> {
	typedef StringPoolBase<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize> super;

public:
	template <typename ... Args>
	StringPool(Args && ... args) noexcept : super(std::forward<Args>(args) ...) {
	}

	//! Checks if the given string is in the pool and returns the pointer to the string, or return nullptr if there is no such string
	const StringType *find(const typename StringType::value_type *s, typename StringType::size_type n) const noexcept;
	const StringType *find(const typename StringType::value_type *start, const typename StringType::value_type *end) const noexcept {
		return find(start, end - start);
	}
	template <size_t N>
	const StringType *find(const typename StringType::value_type (&s)[N]) const noexcept {
		static_assert(N > 0);
		return find(s, N - 1);
	}

	//! Inserts the given string to the pool, and returns both the pointer to the string and a flag indicating if the string was not present in the pool
	typename super::InsertReturnType insert(const typename StringType::value_type *s, typename StringType::size_type n) noexcept;
	typename super::InsertReturnType insert(const typename StringType::value_type *start, const typename StringType::value_type *end) noexcept {
		return insert(start, end - start);
	}
	template <size_t N>
	typename super::InsertReturnType insert(const typename StringType::value_type (&s)[N]) noexcept {
		static_assert(N > 0);
		return insert(s, N - 1);
	}
};

template <size_t NumberOfRegionBits, class MutexType, class StringType, typename SizeType, size_t PoolSize>
class StringPool<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize, false> final : public StringPoolBase<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize> {
	typedef StringPoolBase<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize> super;

public:
	template <typename ... Args>
	StringPool(Args && ... args) noexcept : super(std::forward<Args>(args) ...) {
	}

	//! Checks if the given string is in the pool and returns the pointer to the string, or return nullptr if there is no such string
	const StringType *find(const typename StringType::value_type *s, typename StringType::size_type n) const noexcept;
	const StringType *find(const typename StringType::value_type *start, const typename StringType::value_type *end) const noexcept {
		return find(start, end - start);
	}
	template <size_t N>
	const StringType *find(const typename StringType::value_type (&s)[N]) const noexcept {
		static_assert(N > 0);
		return find(s, N - 1);
	}

	//! Inserts the given string to the pool, and returns both the pointer to the string and a flag indicating if the string was not present in the pool
	typename super::InsertReturnType insert(const typename StringType::value_type *s, typename StringType::size_type n) noexcept;
	typename super::InsertReturnType insert(const typename StringType::value_type *start, const typename StringType::value_type *end) noexcept {
		return insert(start, end - start);
	}
	template <size_t N>
	typename super::InsertReturnType insert(const typename StringType::value_type (&s)[N]) noexcept {
		static_assert(N > 0);
		return insert(s, N - 1);
	}
};

template <size_t NumberOfRegionBits, class MutexType, class StringType, typename SizeType, size_t PoolSize>
void StringPoolBase<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize>::initialize() noexcept {
	for (auto &region : Regions) {
		auto new_buckets = static_cast<StringType **>(mmap(
			nullptr,
			INITIAL_NUMBER_OF_BUCKETS * sizeof(StringType *),
			PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS | MAP_PRIVATE,
			-1,
			0));
		if (new_buckets != MAP_FAILED) {
			//Only initialize buckets/mask as the rest are already default initialized
			region.Buckets = new_buckets;
			region.Mask = INITIAL_NUMBER_OF_BUCKETS - 1;
		} else {
			abort();
		}
	}
}

template <size_t NumberOfRegionBits, class MutexType, class StringType, typename SizeType, size_t PoolSize>
const StringType *StringPool<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize, true>::find(const typename StringType::value_type *s, typename StringType::size_type n) const noexcept {
	StringType *str;

	auto hash = super::eval(s, n);
	auto &region = super::Regions[hash & (super::NUMBER_OF_REGIONS - 1)];

	region.lock();
	auto buckets = region.Buckets;
	auto mask = region.Mask;
	for (str = buckets[(hash >> super::NUMBER_OF_REGIONS_BITS) & mask]; str != nullptr; str = str->getNext()) {
		if (str->hash() == hash && str->size() == n && memcmp(s, str->data(), n) == 0) {
			break;
		}
	}
	region.unlock();
	return str;
}

template <size_t NumberOfRegionBits, class MutexType, class StringType, typename SizeType, size_t PoolSize>
auto StringPool<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize, true>::insert(const typename StringType::value_type *s, typename StringType::size_type n) noexcept -> typename super::InsertReturnType {
	StringType *str;

	//Check if there is an existing entry
	auto hash = super::eval(s, n);
	auto &region = super::Regions[hash & (super::NUMBER_OF_REGIONS - 1)];

	region.lock();
	auto buckets = region.Buckets;
	auto mask = region.Mask;
	for (str = buckets[(hash >> super::NUMBER_OF_REGIONS_BITS) & mask]; str != nullptr; str = str->getNext()) {
		if (str->hash() == hash && str->size() == n && memcmp(s, str->data(), n) == 0) {
			region.unlock();
			return { str, false };
		}
	}

	//Resize the buckets if necessary
	auto size = region.Size;
	++size;
	region.Size = size;
	if (EJ_UNLIKELY(size > mask)) {
		auto new_mask = mask;
		new_mask = new_mask * 2 + 1;

		auto new_buckets = static_cast<StringType **>(mremap(
			buckets,
			(mask + 1) * sizeof(StringType *),
			(new_mask + 1) * sizeof(StringType *),
			MREMAP_MAYMOVE));
		if (new_buckets != MAP_FAILED) {
			for (auto buckets_i = new_buckets, buckets_e_m1 = new_buckets + mask; buckets_i <= buckets_e_m1; ++buckets_i) {
				str = *buckets_i;
				if (str != nullptr) {
					*buckets_i = nullptr;
					do {
						auto next_str = str->getNext();
						auto &new_bucket = new_buckets[(str->hash() >> super::NUMBER_OF_REGIONS_BITS) & new_mask];
						str->setNext(new_bucket);
						new_bucket = str;
						str = next_str;
					} while (str != nullptr);
				}
			}

			buckets = new_buckets;
			mask = new_mask;
			region.Buckets = new_buckets;
			region.Mask = new_mask;
		}
	}

	//Allocate a new string
	str = region.Pools.template alloc_flexible_array<StringType, StringType::NullTerminated ? 1 : 0>(n);
	auto &bucket = buckets[(hash >> super::NUMBER_OF_REGIONS_BITS) & mask];
	str->setNext(bucket);
	str->setHash(hash);
	str->setSize(n);
	memcpy(str->data(), s, n);
	//Since the pool uses mmap and the memory is always zeroed, skip setting the null terminator at the end
	bucket = str;

	region.unlock();
	return { str, true };
}

template <size_t NumberOfRegionBits, class MutexType, class StringType, typename SizeType, size_t PoolSize>
const StringType *StringPool<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize, false>::find(const typename StringType::value_type *s, typename StringType::size_type n) const noexcept {
	StringType *str;

	auto hash = super::eval(s, n);
	auto &region = super::Regions[hash & (super::NUMBER_OF_REGIONS - 1)];

	region.rdlock();
	auto buckets = region.Buckets;
	auto mask = region.Mask;
	for (str = buckets[(hash >> super::NUMBER_OF_REGIONS_BITS) & mask]; str != nullptr; str = str->getNext()) {
		if (str->hash() == hash && str->size() == n && memcmp(s, str->data(), n) == 0) {
			break;
		}
	}
	region.rdunlock();
	return str;
}

template <size_t NumberOfRegionBits, class MutexType, class StringType, typename SizeType, size_t PoolSize>
auto StringPool<NumberOfRegionBits, MutexType, StringType, SizeType, PoolSize, false>::insert(const typename StringType::value_type *s, typename StringType::size_type n) noexcept -> typename super::InsertReturnType {
	StringType *str;

	//Check if there is an existing entry
	auto hash = super::eval(s, n);
	auto &region = super::Regions[hash & (super::NUMBER_OF_REGIONS - 1)];

	region.rdlock();
	auto buckets = region.Buckets;
	auto mask = region.Mask;
	for (str = buckets[(hash >> super::NUMBER_OF_REGIONS_BITS) & mask]; str != nullptr; str = str->getNext()) {
		if (str->hash() == hash && str->size() == n && memcmp(s, str->data(), n) == 0) {
			region.rdunlock();
			return { str, false };
		}
	}
	region.rdunlock();

	region.wrlock();
	//Retry the search in case another writer has added the string
	buckets = region.Buckets;
	mask = region.Mask;
	for (str = buckets[(hash >> super::NUMBER_OF_REGIONS_BITS) & mask]; str != nullptr; str = str->getNext()) {
		if (str->hash() == hash && str->size() == n && memcmp(s, str->data(), n) == 0) {
			region.wrunlock();
			return { str, false };
		}
	}

	//Resize the buckets if necessary
	auto size = region.Size;
	++size;
	region.Size = size;
	if (EJ_UNLIKELY(size > mask)) {
		auto new_mask = mask;
		new_mask = new_mask * 2 + 1;

		auto new_buckets = static_cast<StringType **>(mremap(
			buckets,
			(mask + 1) * sizeof(StringType *),
			(new_mask + 1) * sizeof(StringType *),
			MREMAP_MAYMOVE));
		if (new_buckets != MAP_FAILED) {
			for (auto buckets_i = new_buckets, buckets_e_m1 = new_buckets + mask; buckets_i <= buckets_e_m1; ++buckets_i) {
				str = *buckets_i;
				if (str != nullptr) {
					*buckets_i = nullptr;
					do {
						auto next_str = str->getNext();
						auto &new_bucket = new_buckets[(str->hash() >> super::NUMBER_OF_REGIONS_BITS) & new_mask];
						str->setNext(new_bucket);
						new_bucket = str;
						str = next_str;
					} while (str != nullptr);
				}
			}

			buckets = new_buckets;
			mask = new_mask;
			region.Buckets = new_buckets;
			region.Mask = new_mask;
		}
	}

	//Allocate a new string
	str = region.Pools.template alloc_flexible_array<StringType, StringType::NullTerminated ? 1 : 0>(n);
	auto &bucket = buckets[(hash >> super::NUMBER_OF_REGIONS_BITS) & mask];
	str->setNext(bucket);
	str->setHash(hash);
	str->setSize(n);
	memcpy(str->data(), s, n);
	//Since the pool uses mmap and the memory is always zeroed, skip setting the null terminator at the end
	bucket = str;

	region.wrunlock();
	return { str, true };
}

}

#endif
