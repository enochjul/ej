//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_QUEUE_H
#define EJ_QUEUE_H

#include <assert.h>
#include <limits.h>
#include <stddef.h>

#include <type_traits>

#include "CAlloc.h"
#include "CallType.h"
#include "Common.h"
#include "Construct.h"
#include "Destruct.h"

namespace ej {

template <typename T, size_t N, class Alloc = CAlloc>
class Queue {
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
	node_type *Last;
	void *Head;
	void *Tail;

public:
	Queue() noexcept;
	Queue(const Queue &) = delete;
	~Queue() noexcept;

	Queue &operator =(const Queue &) = delete;

	bool empty() noexcept {
		return Head == Tail;
	}

	inline void push(typename CallType<value_type>::param_type value) noexcept;
	template <typename Function>
	inline void pop(Function f) noexcept;
	void pop() noexcept {
		pop([](value_type *) EJ_ALWAYS_INLINE_ATTRIBUTE noexcept {});
	}
	void pop(value_type *value) noexcept {
		pop([value](value_type *front) EJ_ALWAYS_INLINE_ATTRIBUTE noexcept { *value = *front; });
	}
};

template <typename T, size_t N, class Alloc>
Queue<T, N, Alloc>::Queue() noexcept {
	auto *new_node = Alloc::template alloc<node_type>();
	new_node->Next = nullptr;
	First = new_node;
	Last = new_node;
	Head = new_node;
	Tail = new_node;
}

template <typename T, size_t N, class Alloc>
Queue<T, N, Alloc>::~Queue() noexcept {
	static_assert(std::is_nothrow_destructible<T>::value);

	if constexpr (std::is_trivially_destructible<value_type>::value) {
		auto *first = First;
		do {
			auto *next = first->Next;
			Alloc::dealloc(first);
			first = next;
		} while (first != nullptr);
	} else {
		auto *first = First;
		auto *last = Last;
		if (first != last) {
			destruct_array(lea(Head, offsetof(node_type, Data)), first->Data + N);

			auto *next = first->Next;
			Alloc::dealloc(first);
			first = next;

			while (first != last) {
				destruct_array(first->Data, first->Data + N);
				next = first->Next;
				Alloc::dealloc(first);
				first = next;
			}

			destruct_array(last->Data, lea(Tail, offsetof(node_type, Data)));
			Alloc::dealloc(last);
		} else {
			destruct_array(lea(Head, offsetof(node_type, Data)), lea(Tail, offsetof(node_type, Data)));
			Alloc::dealloc(first);
		}
	}
}

template <typename T, size_t N, class Alloc>
inline void Queue<T, N, Alloc>::push(typename CallType<value_type>::param_type value) noexcept {
	static_assert(std::is_nothrow_copy_constructible<T>::value);

	auto last = Last;
	auto tail = Tail;
	if (EJ_LIKELY(tail < reinterpret_cast<value_type *>(last) + N)) {
		Tail = lea(tail, sizeof(value_type));
		copy_construct(static_cast<value_type *>(lea(tail, offsetof(node_type, Data))), value);
	} else {
		auto *new_node = Alloc::template alloc<node_type>();
		new_node->Next = nullptr;
		Last->Next = new_node;
		Last = new_node;
		Tail = reinterpret_cast<value_type *>(new_node) + 1;
		copy_construct(new_node->Data, value);
	}
}

template <typename T, size_t N, class Alloc>
template <typename Function>
inline void Queue<T, N, Alloc>::pop(Function f) noexcept {
	static_assert(std::is_nothrow_destructible<T>::value);

	assert(!empty());

	auto head = Head;
	auto front = static_cast<value_type *>(lea(head, offsetof(node_type, Data)));
	static_assert(noexcept(f(front)));
	f(front);
	if constexpr (!std::is_trivially_destructible<value_type>::value) {
		destruct(front);
	}

	auto first = First;
	head = static_cast<value_type *>(head) + 1;
	if (EJ_LIKELY(head < reinterpret_cast<value_type *>(first) + N)) {
		Head = head;
	} else {
		auto next = first->Next;
		if (next != nullptr) {
			First = next;
			Head = next;
			Alloc::dealloc(first);
		} else {
			Head = first;
			Tail = first;
		}
	}
}

}

#endif
