//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>

#include "Common.h"

namespace ej {

//! Represents a view on a list of flexible arrays packed in a single array
template <typename T, size_t ALIGNMENT = alignof(T)>
class FlexArraysView {
public:
	typedef T value_type;
	typedef size_t size_type;

	static_assert((ALIGNMENT & (ALIGNMENT - 1)) == 0 && ALIGNMENT >= alignof(value_type));

private:
	value_type *Start;
	value_type *Finish;

public:
	constexpr FlexArraysView() noexcept : Start(nullptr), Finish(nullptr) {
	}
	constexpr FlexArraysView(value_type *start, value_type *finish) noexcept : Start(start), Finish(finish) {
	}

	constexpr void assign(value_type *start, value_type *finish) noexcept {
		Start = start;
		Finish = finish;
	}

	constexpr size_type size() const noexcept {
		size_type n = 0;
		for (const value_type *iter = Start, *end = Finish; iter < end;) {
			iter = reinterpret_cast<const value_type *>(((reinterpret_cast<uintptr_t>(iter) + iter->byte_size() + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);
			n++;
		}
		return n;
	}

	constexpr bool empty() const noexcept {
		return Start == Finish;
	}

	template <typename Function>
	EJ_ALWAYS_INLINE void for_each(Function f) const noexcept {
		for (auto iter = Start, end = Finish; iter < end;) {
			auto next_iter = reinterpret_cast<value_type *>(((reinterpret_cast<uintptr_t>(iter) + iter->byte_size() + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);

			static_assert(noexcept(f(*iter)));
			f(*iter);

			iter = next_iter;
		}
	}

	template <typename Function>
	EJ_ALWAYS_INLINE void cfor_each(Function f) const noexcept {
		for (const value_type *iter = Start, *end = Finish; iter < end;) {
			auto next_iter = reinterpret_cast<const value_type *>(((reinterpret_cast<uintptr_t>(iter) + iter->byte_size() + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);

			static_assert(noexcept(f(*iter)));
			f(*iter);

			iter = next_iter;
		}
	}
};

}
