//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>
#include <limits.h>
#include <stddef.h>

#include "CAlloc.h"
#include "CallType.h"
#include "Common.h"
#include "Construct.h"
#include "Destruct.h"

namespace ej {

template <typename T, size_t N, class Alloc = CAlloc>
class Stack {
	static_assert(N > 0);

	struct Node {
		Node *Next;
		T Data[N];
	};

public:
	typedef T value_type;
	typedef Node node_type;

private:
	node_type *First;
	void *Top;

public:
	Stack() noexcept;
	Stack(const Stack &) = delete;
	~Stack() noexcept;

	Stack &operator =(const Stack &) = delete;

	bool empty() noexcept {
		return Top == First;
	}

	inline void push(typename CallType<value_type>::param_type value) noexcept;
	template <typename Function>
	inline void pop(Function f) noexcept;
	void pop() noexcept {
		pop([](value_type *) EJ_ALWAYS_INLINE_ATTRIBUTE noexcept {});
	}
	void pop(value_type *value) noexcept {
		pop([value](value_type *back) EJ_ALWAYS_INLINE_ATTRIBUTE noexcept { *value = *back; });
	}
};

template <typename T, size_t N, class Alloc>
Stack<T, N, Alloc>::Stack() noexcept {
	auto *new_node = Alloc::template alloc<node_type>();
	new_node->Next = nullptr;
	First = new_node;
	Top = new_node;
}

template <typename T, size_t N, class Alloc>
Stack<T, N, Alloc>::~Stack() noexcept {
	static_assert(std::is_nothrow_destructible<value_type>::value);

	if constexpr (std::is_trivially_destructible<value_type>::value) {
		auto *first = First;
		do {
			auto *next = first->Next;
			Alloc::dealloc(first);
			first = next;
		} while (first != nullptr);
	} else {
		auto *first = First;
		reverse_destruct_array(first->Data, lea(static_cast<value_type *>(Top), offsetof(node_type, Data)));
		auto *next = first->Next;
		Alloc::dealloc(first);
		first = next;
		while (first != nullptr) {
			reverse_destruct_array(first->Data, first->Data + N);
			next = first->Next;
			Alloc::dealloc(first);
			first = next;
		}
	}
}

template <typename T, size_t N, class Alloc>
inline void Stack<T, N, Alloc>::push(typename CallType<value_type>::param_type value) noexcept {
	static_assert(std::is_nothrow_copy_constructible<value_type>::value);

	auto first = First;
	auto top = Top;
	if (EJ_LIKELY(top < reinterpret_cast<value_type *>(first) + N)) {
		Top = static_cast<value_type *>(top) + 1;
		copy_construct(static_cast<value_type *>(lea(top, offsetof(node_type, Data))), value);
	} else {
		auto *new_node = Alloc::template alloc<node_type>();
		new_node->Next = First;
		First = new_node;
		Top = reinterpret_cast<value_type *>(new_node) + 1;
		copy_construct(new_node->Data, value);
	}
}

template <typename T, size_t N, class Alloc>
template <typename Function>
inline void Stack<T, N, Alloc>::pop(Function f) noexcept {
	static_assert(std::is_nothrow_destructible<T>::value);

	assert(!empty());

	auto top = Top;

	top = static_cast<value_type *>(top) - 1;
	auto back = static_cast<value_type *>(lea(top, offsetof(node_type, Data)));
	static_assert(noexcept(f(back)));
	f(back);
	if constexpr (!std::is_trivially_destructible<value_type>::value) {
		destruct(back);
	}

	auto first = First;
	if (EJ_LIKELY(top > reinterpret_cast<value_type *>(first))) {
		Top = top;
	} else {
		auto next = first->Next;
		if (next != nullptr) {
			First = next;
			Top = reinterpret_cast<value_type *>(next) + N;
			Alloc::dealloc(first);
		} else {
			Top = top;
		}
	}
}

}
