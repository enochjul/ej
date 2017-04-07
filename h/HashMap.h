//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_HASH_MAP_H
#define EJ_HASH_MAP_H

#include <stddef.h>
#include <string.h>

#include "CAlloc.h"
#include "CallType.h"
#include "xxhash.h"

namespace ej {

template <typename K, typename T>
struct HashMapKeyValue {
	typedef K key_type;
	typedef T value_type;
	typedef typename CallType<key_type>::param_type key_param_type;
	typedef typename CallType<value_type>::param_type value_param_type;

	K Key;
	T Value;

	HashMapKeyValue(key_param_type key, value_param_type value) : Key(key), Value(value) {
	}
};

template <typename K, typename T, typename S, bool CacheHash>
struct HashMapNode;

template <typename K, typename T, typename S>
struct HashMapNode<K, T, S, false> {
	typedef K key_type;
	typedef T value_type;
	typedef HashMapKeyValue<key_type, value_type> key_value_type;
	typedef S size_type;
	typedef typename CallType<key_type>::param_type key_param_type;
	typedef typename CallType<value_type>::param_type value_param_type;

	HashMapNode *Next;
	HashMapKeyValue<key_type, value_type> KeyValue;

	HashMapNode(HashMapNode *next, size_type, key_param_type key, value_param_type value) noexcept : Next(next), KeyValue(key, value) {
	}

	template <class HasherType>
	size_type findHash(const HasherType *hasher) const noexcept {
		return hasher->eval(KeyValue.Key);
	}
};

template <typename K, typename T, typename S>
struct HashMapNode<K, T, S, true> {
	typedef K key_type;
	typedef T value_type;
	typedef HashMapKeyValue<key_type, value_type> key_value_type;
	typedef S size_type;
	typedef typename CallType<key_type>::param_type key_param_type;
	typedef typename CallType<value_type>::param_type value_param_type;

	HashMapNode *Next;
	size_type Hash;
	HashMapKeyValue<key_type, value_type> KeyValue;

	HashMapNode(HashMapNode *next, size_type hash, key_param_type key, value_param_type value) noexcept : Next(next), Hash(hash), KeyValue(key, value) {
	}

	template <class HasherType>
	size_type findHash(const HasherType *) const noexcept {
		return Hash;
	}
};

template <typename K, typename T, class HasherType = xxHash<size_t>, bool CacheHash = false, class Alloc = CAlloc>
class HashMap : public HasherType {
public:
	typedef K key_type;
	typedef T value_type;
	typedef HashMapKeyValue<key_type, value_type> key_value_type;
	typedef size_t size_type;
	typedef HashMapNode<key_type, value_type, size_type, CacheHash> node_type;

private:
	typedef typename CallType<key_type>::param_type key_param_type;
	typedef typename CallType<value_type>::param_type value_param_type;

	enum : size_type {
		INITIAL_NUMBER_OF_BITS = 2,
		INITIAL_MASK = (1 << INITIAL_NUMBER_OF_BITS) - 1,
	};

	node_type **Nodes;
	size_type Mask;
	size_type Size;

public:
	HashMap() noexcept;
	~HashMap() noexcept;

	HashMap(const HashMap &) = delete;
	HashMap &operator =(const HashMap &) = delete;

	bool empty() const noexcept {
		return Size == 0;
	}

	size_type size() const noexcept {
		return Size;
	}

	size_type bucket_count() const noexcept {
		return Mask + 1;
	}

	void clear() noexcept;
	void reserve(size_type n) noexcept;

	struct InsertReturnType {
		key_value_type *Value;
		bool Status;
	};
	InsertReturnType emplace(key_param_type key, value_param_type value) noexcept;

private:
	key_value_type *lookup(key_param_type key) const noexcept;
public:
	key_value_type *find(value_param_type key) noexcept {
		return lookup(key);
	}
	const key_value_type *find(key_param_type key) const noexcept {
		return lookup(key);
	}
};

template <typename K, typename T, class HasherType, bool CacheHash, class Alloc>
HashMap<K, T, HasherType, CacheHash, Alloc>::HashMap() noexcept {
	Nodes = Alloc::template alloc_array<node_type *>(INITIAL_MASK + 1);
	Mask = INITIAL_MASK;
	Size = 0;
	memset(Nodes, 0, sizeof(node_type *) * (INITIAL_MASK + 1));
}

template <typename K, typename T, class HasherType, bool CacheHash, class Alloc>
HashMap<K, T, HasherType, CacheHash, Alloc>::~HashMap() noexcept {
	for (auto nodes_i = Nodes, nodes_e = nodes_i + Mask + 1; nodes_i != nodes_e; ++nodes_i) {
		auto node = *nodes_i;
		if (node != nullptr) {
			do {
				auto next_node = node->Next;
				Alloc::dealloc(node);
				node = next_node;
			} while (node != nullptr);
		}
	}
	Alloc::dealloc_array(Nodes, Mask + 1);
}

template <typename K, typename T, class HasherType, bool CacheHash, class Alloc>
void HashMap<K, T, HasherType, CacheHash, Alloc>::clear() noexcept {
	for (auto nodes_i = Nodes, nodes_e = nodes_i + Mask + 1; nodes_i != nodes_e; ++nodes_i) {
		auto node = *nodes_i;
		if (node != nullptr) {
			do {
				auto next_node = node->Next;
				Alloc::dealloc(node);
				node = next_node;
			} while (node != nullptr);
			*nodes_i = nullptr;
		}
	}
	Size = 0;
}

template <typename K, typename T, class HasherType, bool CacheHash, class Alloc>
void HashMap<K, T, HasherType, CacheHash, Alloc>::reserve(size_type n) noexcept {
	static_assert(sizeof(size_type) == 4 || sizeof(size_type) == 8);

	auto mask = Mask;
	if (n > mask + 1) {
		size_type new_mask;

		new_mask = n - 1;
		new_mask |= new_mask >> 1;
		new_mask |= new_mask >> 2;
		new_mask |= new_mask >> 4;
		new_mask |= new_mask >> 8;
		new_mask |= new_mask >> 16;
		if (sizeof(size_type) == 8) {
			new_mask |= new_mask >> 32;
		}

		auto new_nodes = Alloc::template try_alloc_array<node_type *>(new_mask + 1);
		if (new_nodes != nullptr) {
			memset(new_nodes, 0, sizeof(node_type *) * (new_mask + 1));
			auto nodes = Nodes;
			for (auto nodes_i = nodes, nodes_e = nodes_i + mask + 1; nodes_i != nodes_e; ++nodes_i) {
				auto node = *nodes_i;
				if (node != nullptr) {
					do {
						auto new_hash_nodes_i = new_nodes + (HasherType::eval(node->KeyValue.Key) & new_mask);
						auto next_node = node->Next;
						node->Next = *new_hash_nodes_i;
						*new_hash_nodes_i = node;
						node = next_node;
					} while (node != nullptr);
				}
			}

			Alloc::dealloc_array(nodes, mask + 1);
			Nodes = new_nodes;
			Mask = new_mask;
		}
	}
}

template <typename K, typename T, class HasherType, bool CacheHash, class Alloc>
auto HashMap<K, T, HasherType, CacheHash, Alloc>::emplace(key_param_type key, value_param_type value) noexcept -> InsertReturnType {
	auto hash = HasherType::eval(key);
	auto nodes = Nodes;
	auto mask = Mask;
	auto hash_nodes_i = nodes + (hash & mask);
	for (auto node = *hash_nodes_i; node != nullptr; node = node->Next) {
		if (node->KeyValue.Key == key) {
			return { &(node->KeyValue), false };
		}
	}

	auto n = Size;
	if (n > mask) {
		auto new_mask = mask * 2 + 1;
		auto new_nodes = Alloc::template try_alloc_array<node_type *>(new_mask + 1);
		if (new_nodes != nullptr) {
			for (auto nodes_i = nodes, new_nodes_i = new_nodes, nodes_e = nodes_i + mask + 1; nodes_i != nodes_e; ++nodes_i, ++new_nodes_i) {
				auto node = *nodes_i;
				*new_nodes_i = nullptr;
				*(new_nodes_i + mask + 1) = nullptr;
				if (node != nullptr) {
					do {
						auto new_hash_nodes_i = new_nodes + (node->findHash(this) & new_mask);
						auto next_node = node->Next;
						node->Next = *new_hash_nodes_i;
						*new_hash_nodes_i = node;
						node = next_node;
					} while (node != nullptr);
				}
			}

			Alloc::dealloc_array(nodes, mask + 1);
			Nodes = new_nodes;
			Mask = new_mask;
			hash_nodes_i = new_nodes + (hash & new_mask);
		}
	}

	auto new_node = Alloc::template alloc<node_type>();
	new(new_node, 0) node_type(*hash_nodes_i, hash, key, value);
	*hash_nodes_i = new_node;
	Size = n + 1;
	return { &(new_node->KeyValue), true };
}

template <typename K, typename T, class HasherType, bool CacheHash, class Alloc>
auto HashMap<K, T, HasherType, CacheHash, Alloc>::lookup(key_param_type key) const noexcept -> key_value_type * {
	auto hash = HasherType::eval(key);
	for (auto node = Nodes[hash & Mask]; node != nullptr; node = node->Next) {
		if (node->KeyValue.Key == key) {
			return &(node->KeyValue);
		}
	}
	return nullptr;
}

}

#endif
