//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_KEY_TO_VALUE_H
#define EJ_KEY_TO_VALUE_H

#include <stdint.h>
#include <string.h>

namespace ej {

template <typename O, size_t S, typename V>
struct OffsetValue {
	typedef O offset_type;
	typedef V value_type;
	static constexpr size_t Scale = S;

	offset_type Offset;
	value_type Value;
};

template <typename EntryType, size_t N>
const EntryType *lsearch_c_string_to_value(const char data[], const EntryType entries[], const char *key) noexcept {
	for (const auto *i = entries, e = entries + N; i < e; i++) {
		if (strcmp(data + i->Offset * EntryType::Scale, key) == 0) {
			return i;
		}
	}
	return nullptr;
}

template <typename OffsetType, size_t Scale, typename ValueType, size_t N>
const ValueType *lsearch_c_string_to_value(const char data[], const OffsetType offsets[], const ValueType values[], const char *key) noexcept {
	const OffsetType *offsets_i;
	const OffsetType *offsets_end;
	const ValueType *value;
	for (offsets_i = offsets, offsets_end = offsets + N, value = values; offsets_i < offsets_end; offsets_i++, value++) {
		if (strcmp(data + *offsets_i * Scale, key) == 0) {
			return value;
		}
	}
	return nullptr;
}

template <typename EntryType, size_t N>
const EntryType *bsearch_c_string_to_value(const char data[], const EntryType entries[], const char *key) noexcept {
	size_t left = 0, n = N;
	while (n > 0) {
		size_t half = n / 2;
		size_t middle = left + half;
		const auto *entry = entries + middle;
		auto status = strcmp(data + entry->Offset * EntryType::Scale, key);
		if (status < 0) {
			left = middle + 1;
			n -= half + 1;
		} else if (status > 0) {
			n = half;
		} else {
			return entry;
		}
	}
	return nullptr;
}

template <typename OffsetType, size_t Scale, typename ValueType, size_t N>
const ValueType *bsearch_c_string_to_value(const char data[], const OffsetType offsets[], const ValueType values[], const char *key) noexcept {
	size_t left = 0, n = N;
	while (n > 0) {
		size_t half = n / 2;
		size_t middle = left + half;
		auto status = strcmp(data + offsets[middle] * Scale, key);
		if (status < 0) {
			left = middle + 1;
			n -= half + 1;
		} else if (status > 0) {
			n = half;
		} else {
			return values + middle;
		}
	}
	return nullptr;
}

template <typename EntryType, size_t N>
const EntryType *bsearch_eytzinger_c_string_to_value(const char data[], const EntryType entries[], const char *key) noexcept {
	size_t index = 0;
	while (index < N) {
		const auto *entry = entries + index;
		auto status = strcmp(data + entry->Offset * EntryType::Scale, key);
		if (status == 0) {
			return entry;
		}
		size_t bias = status > 0 ? 0 : 1;
		index = index * 2 + 1 + bias;
	}
	return nullptr;
}

template <typename OffsetType, size_t Scale, typename ValueType, size_t N>
const ValueType *bsearch_eytzinger_c_string_to_value(const char data[], const OffsetType offsets[], const ValueType values[], const char *key) noexcept {
	size_t index = 0, new_index;
	while (index < N) {
		auto status = strcmp(data + offsets[index] * Scale, key);
		if (status == 0) {
			return values + index;
		}
		size_t bias = status > 0 ? 0 : 1;
		index = index * 2 + 1 + bias;
	}
	return nullptr;
}

template <typename EntryType, size_t Mask, typename Hasher>
const EntryType *linear_hash_c_string_to_value(const char data[], const EntryType entries[], const char *key) noexcept {
	auto h = Hasher()(key);
	for (const auto *entry = entries + (h & Mask); entry->Offset > 0; entry++) {
		if (strcmp(data + entry->Offset * EntryType::Scale, key) == 0) {
			return entry;
		}
	}
	return nullptr;
}

template <typename OffsetType, size_t Scale, typename ValueType, size_t Mask, typename Hasher>
const ValueType *linear_hash_c_string_to_value(const char data[], const OffsetType offsets[], const ValueType values[], const char *key) noexcept {
	auto h = Hasher()(key);
	OffsetType offset;
	for (auto index = h & Mask; (offset = offsets[index]) > 0; index++) {
		if (strcmp(data + offset * Scale, key) == 0) {
			return values + index;
		}
	}
	return nullptr;
}

template <typename EntryType, size_t Mask, typename Hasher>
const EntryType *perfect_hash_c_string_to_value(const char data[], const EntryType entries[], const char *key) noexcept {
	auto h = Hasher()(key);
	auto *entry = entries + (h & Mask);
	if (entry->Offset > 0) {
		if (strcmp(data + entry->Offset * EntryType::Scale, key) == 0) {
			return entry;
		}
	}
	return nullptr;
}

template <typename OffsetType, size_t Scale, typename ValueType, size_t Mask, typename Hasher>
const ValueType *perfect_hash_c_string_to_value(const char data[], const OffsetType offsets[], const ValueType values[], const char *key) noexcept {
	auto h = Hasher()(key);
	auto index = h & Mask;
	auto offset = offsets[index];
	if (offset > 0) {
		if (strcmp(data + offset * Scale, key) == 0) {
			return values + index;
		}
	}
	return nullptr;
}

}

#endif
