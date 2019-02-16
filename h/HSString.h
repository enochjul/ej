//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>

#include <type_traits>

#include "Common.h"

namespace ej {

//! Represents a statically initializable null-terminated string that stores a pre-computed hash and a size
template <class H, typename CharType = char, typename S = size_t>
struct HSString {
	typedef HSString this_type;
	typedef CharType value_type;
	typedef H hasher_type;
	typedef typename hasher_type::value_type hash_type;
	typedef S size_type;
	typedef const value_type *const_iterator;
	typedef this_type make_value_type;
	typedef const this_type *const_make_type;

	constexpr static bool HasHash = true;
	constexpr static bool HasSize = true;

	hash_type Hash;
	size_type Size;
	value_type Data[];

	static constexpr size_t sizeofHeader() noexcept {
		return offsetof(this_type, Data);
	}

	static constexpr size_t byte_size(size_type n) noexcept {
		return ((sizeofHeader() + n * sizeof(value_type) + alignof(this_type)) / alignof(this_type)) * alignof(this_type);
	}

	constexpr hash_type hash() const noexcept {
		return Hash;
	}

	constexpr size_type size() const noexcept {
		return Size;
	}

	constexpr size_type length() const noexcept {
		return Size;
	}

	constexpr bool empty() const noexcept {
		return Size == 0;
	}

	constexpr const value_type *data() const noexcept {
		return Data;
	}

	constexpr const value_type *c_str() const noexcept {
		return Data;
	}

	constexpr const_iterator begin() const noexcept {
		return Data;
	}

	constexpr const_iterator end() const noexcept {
		return Data + Size;
	}

	constexpr const_iterator cbegin() const noexcept {
		return Data;
	}

	constexpr const_iterator cend() const noexcept {
		return Data + Size;
	}

	constexpr const value_type &operator [](size_type pos) const noexcept {
		return Data[pos];
	}

	constexpr const value_type &front() const noexcept {
		return Data[0];
	}

	constexpr const value_type &back() const noexcept {
		return Data[Size - 1];
	}

	template <typename A, bool null_terminated>
	EJ_ALWAYS_INLINE static std::enable_if_t<A::Global, make_value_type *> make(hash_type hash, size_type n, const value_type *s) noexcept {
		auto *new_s = A::template alloc_flexible_array<this_type, 1>(n);
		new_s->Hash = hash;
		new_s->Size = n;
		if constexpr (null_terminated) {
			memcpy(new_s->Data, s, n + 1);
		} else {
			memcpy(new_s->Data, s, n);
			new_s->Data[n] = '\0';
		}
		return new_s;
	}

	template <typename A, bool null_terminated>
	EJ_ALWAYS_INLINE static std::enable_if_t<!A::Global, make_value_type *> make(A *alloc, hash_type hash, size_type n, const value_type *s) noexcept {
		auto *new_s = alloc->template alloc_flexible_array<this_type, 1>(n);
		new_s->Hash = hash;
		new_s->Size = n;
		if constexpr (null_terminated) {
			memcpy(new_s->Data, s, n + 1);
		} else {
			memcpy(new_s->Data, s, n);
			new_s->Data[n] = '\0';
		}
		return new_s;
	}

	template <typename A>
	EJ_ALWAYS_INLINE static std::enable_if_t<A::Global, void> destroy(make_value_type *s) noexcept {
		A::template dealloc_flexible_array<make_value_type, 1>(s, s->size());
	}

	template <typename A>
	EJ_ALWAYS_INLINE static std::enable_if_t<!A::Global, void> destroy(A *alloc, make_value_type *s) noexcept {
		A::template dealloc_array<make_value_type, 1>(s, s->size());
	}
};

}
