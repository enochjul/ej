//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>

#include <type_traits>

#include "Common.h"

namespace ej {

template <typename H>
class CStringTraits final {
public:
	typedef char value_type;
	typedef size_t size_type;
	typedef H hasher_type;
	typedef typename hasher_type::value_type hash_type;
	typedef char make_value_type;

	constexpr static bool HasHash = false;
	constexpr static bool HasSize = false;

	CStringTraits(const CStringTraits &) = delete;
	CStringTraits &operator =(const CStringTraits &) = delete;

	template <typename A, bool null_terminated>
	EJ_ALWAYS_INLINE static std::enable_if_t<A::Global, make_value_type *> make(size_type n, const value_type *s) noexcept {
		auto *new_s = A::template alloc_array<value_type, 1>(n);
		if constexpr (null_terminated) {
			memcpy(new_s, s, n + 1);
		} else {
			memcpy(new_s, s, n);
			new_s[n] = '\0';
		}
		return new_s;
	}

	template <typename A, bool null_terminated>
	EJ_ALWAYS_INLINE static std::enable_if_t<!A::Global, make_value_type *> make(A *alloc, size_type n, const value_type *s) noexcept {
		auto *new_s = alloc->template alloc_array<value_type, 1>(n);
		if constexpr (null_terminated) {
			memcpy(new_s, s, n + 1);
		} else {
			memcpy(new_s, s, n);
			new_s[n] = '\0';
		}
		return new_s;
	}

	template <typename A>
	EJ_ALWAYS_INLINE static std::enable_if_t<A::Global, void> destroy(make_value_type *s) noexcept {
		A::template dealloc_array<make_value_type, 1>(s, strlen(s));
	}

	template <typename A>
	EJ_ALWAYS_INLINE static std::enable_if_t<!A::Global, void> destroy(A *alloc, make_value_type *s) noexcept {
		A::template dealloc_array<make_value_type, 1>(s, strlen(s));
	}
};

}
