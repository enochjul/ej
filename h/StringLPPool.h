//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdlib.h>
#include <string.h>

#include <type_traits>

#include "CAlloc.h"
#include "Common.h"
#include "CStringView.h"
#include "fill.h"
#include "log2.h"

namespace ej {

template <typename T, typename ST = typename T::hash_type, class EA = CAlloc, class SA = CAlloc>
class StringLPPool final {
	typedef EA EntriesAlloc;
	typedef SA StringAlloc;

	static_assert(EntriesAlloc::HasDealloc);

public:
	typedef T traits_type;
	typedef typename traits_type::make_value_type value_type;
	typedef typename traits_type::value_type char_type;
	typedef typename traits_type::size_type string_size_type;
	typedef typename traits_type::hasher_type hasher_type;
	typedef typename traits_type::hash_type hash_type;
	typedef ST size_type;

	static_assert(sizeof(size_type) <= sizeof(hash_type));

private:
	value_type **Entries;
	hash_type Mask;
	size_type Size;

	EJ_ALWAYS_INLINE static void insert(value_type **entries, value_type **entries_e, hash_type mask, hash_type hash, value_type *value) noexcept {
		auto initial_entries_i = entries + (hash & mask);
		for (auto entries_i = initial_entries_i; entries_i < entries_e; entries_i++) {
			if (*entries_i == nullptr) {
				*entries_i = value;
				return;
			}
		}
		for (auto entries_i = entries; entries_i < initial_entries_i; entries_i++) {
			if (*entries_i == nullptr) {
				*entries_i = value;
				return;
			}
		}

		abort();
	}

	template <bool null_terminated>
	EJ_ALWAYS_INLINE static bool match(hash_type hash, size_type n, const char_type *s, const value_type *value) noexcept {
		if constexpr (std::is_scalar_v<value_type>) {
			static_assert(!traits_type::HasHash && !traits_type::HasSize && std::is_same_v<char_type, value_type>);
			if constexpr (!null_terminated) {
				return n == strlen(value) && memcmp(s, value, n) == 0;
			} else {
				return strcmp(s, value) == 0;
			}
		} else {
			if constexpr (traits_type::HasHash) {
				if constexpr (traits_type::HasSize || !null_terminated) {
					return hash == value->hash() && n == value->size() && memcmp(s, value->data(), n) == 0;
				} else {
					return hash == value->hash() && strcmp(s, value->c_str()) == 0;
				}
			} else {
				if constexpr (traits_type::HasSize || !null_terminated) {
					return n == value->size() && memcmp(s, value->data(), n) == 0;
				} else {
					return strcmp(s, value->c_str()) == 0;
				}
			}
		}
	}

public:
	StringLPPool() noexcept;
	~StringLPPool() noexcept;

	StringLPPool(const StringLPPool &) = delete;
	StringLPPool &operator =(const StringLPPool &) = delete;

	bool empty() const noexcept {
		return Size == 0;
	}

	size_type size() const noexcept {
		return Size;
	}

	void clear() noexcept;

	struct InsertReturnType {
		const value_type *Value;
		bool Inserted;
	};

	template <bool null_terminated = false>
	InsertReturnType insert(const char_type *s, string_size_type n) noexcept;
	InsertReturnType insert(const char_type *s) noexcept {
		return insert<true>(s, strlen(s));
	}

	template <bool null_terminated = false>
	const value_type *find(const char_type *s, string_size_type n) const noexcept;
	const value_type *find(const char_type *s) const noexcept {
		return find<true>(s, strlen(s));
	}

	template <bool null_terminated = false>
	bool exists(const char_type *s, string_size_type n) const noexcept {
		return find<null_terminated>(s, n) != nullptr;
	}
	bool exists(const char_type *s) const noexcept {
		return exists<true>(s, strlen(s));
	}

	template <bool null_terminated = false>
	bool contains(const char_type *s, string_size_type n) const noexcept {
		return exists<null_terminated>(s, n);
	}
	bool contains(const char_type *s) const noexcept {
		return exists(s);
	}

	template <typename Function>
	EJ_ALWAYS_INLINE void for_each(Function f) const noexcept {
		auto entries = Entries;
		auto mask = Mask;
		auto entries_e = entries + (static_cast<size_t>(mask) + 1u);
		for (auto entries_i = entries; entries_i < entries_e; entries_i++) {
			const value_type *value = *entries_i;
			if (value != nullptr) {
				static_assert(noexcept(f(value)));
				f(value);
			}
		}
	}
};

template <typename T, typename ST, class EA, class SA>
StringLPPool<T, ST, EA, SA>::StringLPPool() noexcept {
	constexpr auto alloc_min_number_of_entries = ce_ceil_log2(((EntriesAlloc::min_size() + sizeof(value_type *) - 1u) / sizeof(value_type *)) * sizeof(value_type *)).Result;
	static_assert(alloc_min_number_of_entries <= std::numeric_limits<size_type>::max());
	constexpr auto min_number_of_entries = alloc_min_number_of_entries <= 16 ? 16 : alloc_min_number_of_entries;
	static_assert((min_number_of_entries & (min_number_of_entries - 1)) == 0);

	auto new_entries = EntriesAlloc::template alloc_array<value_type *>(min_number_of_entries);
	if constexpr (!EntriesAlloc::AlwaysZero) {
		ej::fill_zero_n(new_entries, min_number_of_entries);
	}
	Entries = new_entries;
	Mask = min_number_of_entries - 1;
	Size = 0;
}

template <typename T, typename ST, class EA, class SA>
StringLPPool<T, ST, EA, SA>::~StringLPPool() noexcept {
	auto entries = Entries;
	auto mask = Mask;
	auto entries_e = entries + (static_cast<size_t>(mask) + 1u);
	if constexpr (StringAlloc::HasDealloc) {
		for (auto entries_i = entries; entries_i < entries_e; entries_i++) {
			auto value = *entries_i;
			if (value != nullptr) {
				traits_type::template destroy<StringAlloc>(value);
			}
		}
	}
	EntriesAlloc::dealloc_array_range(entries, entries, entries_e);
}

template <typename T, typename ST, class EA, class SA>
void StringLPPool<T, ST, EA, SA>::clear() noexcept {
	auto entries = Entries;
	auto mask = Mask;
	auto entries_e = entries + (static_cast<size_t>(mask) + 1u);
	if constexpr (StringAlloc::HasDealloc) {
		for (auto entries_i = entries; entries_i < entries_e; entries_i++) {
			auto value = *entries_i;
			if (value != nullptr) {
				traits_type::template destroy<StringAlloc>(value);
				*entries_i = nullptr;
			}
		}
	} else {
		ej::fill_zero(entries, entries_e);
	}
	Size = 0;
}

template <typename T, typename ST, class EA, class SA>
template <bool null_terminated>
auto StringLPPool<T, ST, EA, SA>::insert(const char_type *s, string_size_type n) noexcept -> InsertReturnType {
	auto hash = hasher_type::eval(s, n);

	auto entries = Entries;
	auto mask = Mask;
	auto entries_e = entries + (static_cast<size_t>(mask) + 1u);

	auto initial_entries_i = entries + (hash & mask);
	value_type **entries_i;
	for (entries_i = initial_entries_i; entries_i < entries_e; entries_i++) {
		auto value = *entries_i;
		if (value != nullptr) {
			if (match<null_terminated>(hash, n, s, value)) {
				return { value, false };
			}
		} else {
			goto insert_new_value;
		}
	}
	for (entries_i = entries; entries_i < initial_entries_i; entries_i++) {
		auto value = *entries_i;
		if (value != nullptr) {
			if (match<null_terminated>(hash, n, s, value)) {
				return { value, false };
			}
		} else {
			goto insert_new_value;
		}
	}
	abort();

insert_new_value:
	value_type *new_value;
	if constexpr (traits_type::HasHash) {
		new_value = traits_type::template make<StringAlloc, null_terminated>(hash, n, s);
	} else {
		new_value = traits_type::template make<StringAlloc, null_terminated>(n, s);
	}
	*entries_i = new_value;

	auto number_of_entries = Size;
	Size = number_of_entries + 1;

	if (EJ_UNLIKELY(number_of_entries > (mask / 2u))) {
		if ((sizeof(size_type) >= sizeof(hash_type) || number_of_entries < std::numeric_limits<size_type>::max()) && mask <= (std::numeric_limits<hash_type>::max() - 1u) / 2u) {
			auto new_mask = mask * 2u + 1u;
			auto new_number_of_entries = static_cast<size_t>(new_mask) + 1u;
			auto new_entries = EntriesAlloc::template alloc_array<value_type *>(new_number_of_entries);
			if constexpr (!EntriesAlloc::AlwaysZero) {
				ej::fill_zero_n(new_entries, new_number_of_entries);
			}
			auto new_entries_e = new_entries + new_number_of_entries;

			for (entries_i = entries; entries_i < entries_e; entries_i++) {
				auto value = *entries_i;
				if (value != nullptr) {
					if constexpr (std::is_scalar_v<value_type>) {
						insert(new_entries, new_entries_e, new_mask, hasher_type::eval(value, strlen(value)), value);
					} else {
						insert(new_entries, new_entries_e, new_mask, value->hash(), value);
					}
				}
			}

			EntriesAlloc::dealloc_array_range(entries, entries, entries_e);
			Entries = new_entries;
			Mask = new_mask;
		} else {
			abort();
		}
	}

	return { new_value, true };
}

template <typename T, typename ST, class EA, class SA>
template <bool null_terminated>
auto StringLPPool<T, ST, EA, SA>::find(const char_type *s, string_size_type n) const noexcept -> const value_type * {
	auto hash = hasher_type::eval(s, n);

	auto entries = Entries;
	auto mask = Mask;
	auto entries_e = entries + (static_cast<size_t>(mask) + 1u);

	auto initial_entries_i = entries + (hash & mask);
	value_type **entries_i;
	for (entries_i = initial_entries_i; entries_i < entries_e; entries_i++) {
		const value_type *value = *entries_i;
		if (value != nullptr) {
			if (match<null_terminated>(hash, n, s, value)) {
				return value;
			}
		} else {
			return nullptr;
		}
	}
	for (entries_i = entries; entries_i < initial_entries_i; entries_i++) {
		const value_type *value = *entries_i;
		if (value != nullptr) {
			if (match<null_terminated>(hash, n, s, value)) {
				return value;
			}
		} else {
			return nullptr;
		}
	}

	return nullptr;
}

}
