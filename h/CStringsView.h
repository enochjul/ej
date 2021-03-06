//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>
#include <string.h>

#include "Common.h"

namespace ej {

//! Represents a view on a list of C strings packed in an array
template <typename T = const char, size_t ALIGNMENT = alignof(T)>
class CStringsView {
public:
	typedef T value_type;
	typedef size_t size_type;

	static_assert((ALIGNMENT & (ALIGNMENT - 1)) == 0 && ALIGNMENT >= alignof(value_type));

private:
	value_type *Start;
	value_type *Finish;

public:
	constexpr CStringsView() noexcept : Start(nullptr), Finish(nullptr) {
	}
	constexpr CStringsView(value_type *start, value_type *finish) noexcept : Start(start), Finish(finish) {
	}
	constexpr CStringsView(value_type *start, size_type n) noexcept : Start(start), Finish(start + n) {
	}
	template <size_t N>
	constexpr CStringsView(value_type (&a)[N]) noexcept : Start(a), Finish(a + N) {
	}

	constexpr void assign(value_type *start, value_type *finish) noexcept {
		Start = start;
		Finish = finish;
	}
	constexpr void assign(value_type *start, size_t n) noexcept {
		Start = start;
		Finish = start + n;
	}
	template <size_t N>
	constexpr void assign(value_type (&a)[N]) noexcept {
		assign(a, N);
	}

	constexpr size_type size() const noexcept {
		size_type n = 0;
		for (const value_type *iter = Start, *end = Finish; iter < end;) {
			auto length = strlen(iter);

			iter += length + 1;
			if constexpr (ALIGNMENT != alignof(value_type)) {
				iter = reinterpret_cast<const value_type *>(((reinterpret_cast<uintptr_t>(iter) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);
			}
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
			auto n = strlen(iter);

			static_assert(noexcept(f(iter, n)));
			f(iter, n);

			iter += n + 1;
			if constexpr (ALIGNMENT != alignof(value_type)) {
				iter = reinterpret_cast<value_type *>(((reinterpret_cast<uintptr_t>(iter) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);
			}
		}
	}

	template <typename Function>
	EJ_ALWAYS_INLINE void cfor_each(Function f) const noexcept {
		for (const value_type *iter = Start, *end = Finish; iter < end;) {
			auto n = strlen(iter);

			static_assert(noexcept(f(iter, n)));
			f(iter, n);

			iter += n + 1;
			if constexpr (ALIGNMENT != alignof(value_type)) {
				iter = reinterpret_cast<const value_type *>(((reinterpret_cast<uintptr_t>(iter) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);
			}
		}
	}
};

}
