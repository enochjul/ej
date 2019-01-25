//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>
#include <string.h>

#include "Common.h"
#include "CAlloc.h"
#include "CallType.h"
#include "Construct.h"
#include "DoubleWideMultiply.h"
#include "IdentityHash.h"

namespace ej {

#if EJ_POINTER_SIZE == 64
extern const uint64_t PrimeHashMapPrimes1_5x[];
extern const uint64_t PrimeHashMapPrimes2x[];
#else
extern const uint32_t PrimeHashMapPrimes1_5x[];
extern const uint32_t PrimeHashMapPrimes2x[];
#endif

template <typename K, typename T, class HasherType = IdentityHash<K>, bool Resize2X = true, class Alloc = CAlloc>
class PrimeHashMap {
public:
	typedef K key_type;
	typedef T value_type;
	typedef size_t size_type;

private:
	typedef typename CallType<key_type>::param_type key_param_type;
	typedef typename CallType<value_type>::param_type value_param_type;

	struct KeyValue {
		const key_type Key;
		value_type Value;

		KeyValue(key_param_type key, value_param_type value) noexcept : Key(key), Value(value) {
		}
	};

	struct Entry {
		Entry *Next;
		KeyValue myKeyValue;

		Entry(Entry *next, key_param_type key, value_param_type value) noexcept : Next(next), myKeyValue(key, value) {
		}
	};

	Entry **Entries;
	const size_type *EntriesSizeReciprocal;
	size_type Size;

	static size_type mod(size_type hash, size_type prime, size_type reciprocal) noexcept {
		size_type d = multiply_high(hash, reciprocal);
		size_type remainder = hash - d * prime;
		if (remainder >= prime) {
			remainder -= prime;
		}
		return remainder;
	}

public:
	PrimeHashMap() noexcept;
	~PrimeHashMap() noexcept;

	PrimeHashMap(const PrimeHashMap &) = delete;
	PrimeHashMap &operator =(const PrimeHashMap &) = delete;

	bool empty() const noexcept {
		return Size == 0;
	}

	size_type size() const noexcept {
		return Size;
	}

	void clear() noexcept;
	void reserve(size_type n) noexcept;

	struct InsertReturnType {
		KeyValue *Value;
		bool Status;
	};
	InsertReturnType emplace(key_param_type key, value_param_type value) noexcept;

private:
	KeyValue *lookup(key_param_type key) const noexcept;
public:
	KeyValue *find(value_param_type key) noexcept {
		return lookup(key);
	}
	const KeyValue *find(key_param_type key) const noexcept {
		return lookup(key);
	}
};

template <typename K, typename T, class HasherType, bool Resize2X, class Alloc>
PrimeHashMap<K, T, HasherType, Resize2X, Alloc>::PrimeHashMap() noexcept {
	size_type initial_entries_size = Resize2X ? PrimeHashMapPrimes2x[0] : PrimeHashMapPrimes1_5x[0];
	Entries = Alloc::template alloc_array<Entry *>(initial_entries_size);
	EntriesSizeReciprocal = Resize2X ? PrimeHashMapPrimes2x : PrimeHashMapPrimes1_5x;
	Size = 0;
	memset(Entries, 0, sizeof(Entry *) * EntriesSizeReciprocal[0]);
}

template <typename K, typename T, class HasherType, bool Resize2X, class Alloc>
PrimeHashMap<K, T, HasherType, Resize2X, Alloc>::~PrimeHashMap() noexcept {
	auto prime = EntriesSizeReciprocal[0];
	for (auto entries_i = Entries, entries_e = entries_i + prime; entries_i != entries_e; ++entries_i) {
		auto entry = *entries_i;
		if (entry != nullptr) {
			do {
				auto next_entry = entry->Next;
				Alloc::dealloc(entry);
				entry = next_entry;
			} while (entry != nullptr);
		}
	}
	Alloc::dealloc_array(Entries, prime);
}

template <typename K, typename T, class HasherType, bool Resize2X, class Alloc>
void PrimeHashMap<K, T, HasherType, Resize2X, Alloc>::clear() noexcept {
	for (auto entries_i = Entries, entries_e = entries_i + EntriesSizeReciprocal[0]; entries_i != entries_e; ++entries_i) {
		auto entry = *entries_i;
		if (entry != nullptr) {
			do {
				auto next_entry = entry->Next;
				Alloc::dealloc(entry);
				entry = next_entry;
			} while (entry != nullptr);
			*entries_i = nullptr;
		}
	}
	Size = 0;
}

template <typename K, typename T, class HasherType, bool Resize2X, class Alloc>
void PrimeHashMap<K, T, HasherType, Resize2X, Alloc>::reserve(size_type n) noexcept {
	auto new_entries_size_reciprocal = EntriesSizeReciprocal;
	while (new_entries_size_reciprocal[2] < n) {
		new_entries_size_reciprocal += 2;
	}
	new_entries_size_reciprocal += 2;

	auto new_prime = new_entries_size_reciprocal[0];
	auto new_entries = Alloc::template try_alloc_array<Entry *>(new_prime);
	if (new_entries != nullptr) {
		memset(new_entries, 0, sizeof(Entry *) * new_prime);
		auto new_reciprocal = new_entries_size_reciprocal[1];
		for (auto entries_i = Entries, entries_e = entries_i + EntriesSizeReciprocal[0]; entries_i != entries_e; ++entries_i) {
			auto entry = *entries_i;
			if (entry != nullptr) {
				do {
					auto new_hash_entries_i = new_entries + mod(HasherType::eval(entry->myKeyValue.Key), new_prime, new_reciprocal);
					auto next_entry = entry->Next;
					entry->Next = *new_hash_entries_i;
					*new_hash_entries_i = entry;
					entry = next_entry;
				} while (entry != nullptr);
			}
		}

		Alloc::dealloc_array(Entries, EntriesSizeReciprocal[0]);
		Entries = new_entries;
		EntriesSizeReciprocal = new_entries_size_reciprocal;
	}
}

template <typename K, typename T, class HasherType, bool Resize2X, class Alloc>
auto PrimeHashMap<K, T, HasherType, Resize2X, Alloc>::emplace(key_param_type key, value_param_type value) noexcept -> InsertReturnType {
	//Search for an existing entry
	auto prime = EntriesSizeReciprocal[0], reciprocal = EntriesSizeReciprocal[1];
	auto hash = HasherType::eval(key);
	auto hash_entries_i = Entries + mod(hash, prime, reciprocal);
	for (auto entry = *hash_entries_i; entry != nullptr; entry = entry->Next) {
		if (entry->myKeyValue.Key == key) {
			return { &(entry->myKeyValue), false };
		}
	}

	//Resize if necessary
	auto n = Size;
	if (n >= prime) {
		auto new_entries_size_reciprocal = EntriesSizeReciprocal + 2;
		auto new_prime = new_entries_size_reciprocal[0];
		auto new_entries = Alloc::template try_alloc_array<Entry *>(new_prime);
		if (new_entries != nullptr) {
			memset(new_entries, 0, sizeof(Entry *) * new_prime);
			auto new_reciprocal = new_entries_size_reciprocal[1];
			for (auto entries_i = Entries, entries_e = entries_i + prime; entries_i != entries_e; entries_i++) {
				auto entry = *entries_i;
				if (entry != nullptr) {
					do {
						auto new_hash_entries_i = new_entries + mod(HasherType::eval(entry->myKeyValue.Key), new_prime, new_reciprocal);
						auto next_entry = entry->Next;
						entry->Next = *new_hash_entries_i;
						*new_hash_entries_i = entry;
						entry = next_entry;
					} while (entry != nullptr);
				}
			}

			Alloc::dealloc_array(Entries, prime);
			Entries = new_entries;
			EntriesSizeReciprocal = new_entries_size_reciprocal;
			hash_entries_i = new_entries + mod(hash, new_prime, new_reciprocal);
		}
	}

	auto new_entry = Alloc::template alloc<Entry>();
	forward_construct(new_entry, *hash_entries_i, key, value);
	*hash_entries_i = new_entry;
	Size = n + 1;
	return { &(new_entry->myKeyValue), true };
}

template <typename K, typename T, class HasherType, bool Resize2X, class Alloc>
auto PrimeHashMap<K, T, HasherType, Resize2X, Alloc>::lookup(key_param_type key) const noexcept -> KeyValue * {
	auto entries_i = Entries + mod(HasherType::eval(key), EntriesSizeReciprocal[0], EntriesSizeReciprocal[1]);
	for (auto entry = *entries_i; entry != nullptr; entry = entry->Next) {
		if (key == entry->myKeyValue.Key) {
			return &(entry->myKeyValue);
		}
	}
	return nullptr;
}

}
