//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_DYN_ARRAY_H
#define EJ_DYN_ARRAY_H

#include <stddef.h>
#include <string.h>

#include <type_traits>

#include "Common.h"
#include "CAlloc.h"
#include "CallType.h"
#include "Construct.h"
#include "Destruct.h"
#include "TypeTraits.h"

namespace ej {

template <
	typename T,
	//! Calls the destructor for each object in reverse order
	bool reverse_destruct = false,
	//! Always call the default constructor if no parameters are supplied to functions such as push()/resize()
	bool always_default_construct = true,
	class Alloc = CAlloc>
class DynArray {
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef T *iterator;
	typedef const T *const_iterator;

private:
	T *Start;
	T *Finish;
	T *AllocFinish;

	value_type *expand_one_alloc() noexcept;
	value_type *expand_one() noexcept {
		//Check if there is enough space available
		if (Finish < AllocFinish) {
			auto ptr = Finish;
			Finish = ptr + 1;
			return ptr;
		} else {
			return expand_one_alloc();
		}
	}

public:
	constexpr DynArray() noexcept : Start(nullptr), Finish(nullptr), AllocFinish(nullptr) {
	}
	~DynArray() noexcept;

	DynArray(const DynArray &a) noexcept;
	DynArray(DynArray &&a) noexcept {
		Start = a.Start;
		Finish = a.Finish;
		AllocFinish = a.AllocFinish;
		a.Start = nullptr;
		a.Finish = nullptr;
		a.AllocFinish = nullptr;
	}

	DynArray &operator =(const DynArray &a) noexcept;
	DynArray &operator =(DynArray &&a) noexcept;

	value_type &operator [](size_type i) noexcept {
		return Start[i];
	}

	const value_type &operator [](size_type i) const noexcept {
		return Start[i];
	}

	void clear() noexcept {
		Finish = Start;
	}

	void resize(size_type n) noexcept;
	void reserve(size_type n) noexcept;
	void push_back(typename CallType<value_type>::param_type value) noexcept {
		append(value);
	}
	value_type &append(typename CallType<value_type>::param_type value) noexcept {
		assert(&value < Start || &value >= AllocFinish);

		auto ptr = expand_one();
		copy_construct(ptr, value);
		return *ptr;
	}
	value_type &append_in_place(typename CallType<value_type>::param_type value) noexcept {
		assert(Finish < AllocFinish);
		auto ptr = Finish;
		Finish = ptr + 1;
		copy_construct(ptr, value);
		return *ptr;
	}

	iterator begin() noexcept {
		return Start;
	}

	const_iterator begin() const noexcept {
		return Start;
	}

	const_iterator cbegin() const noexcept {
		return Start;
	}

	iterator end() noexcept {
		return Finish;
	}

	const_iterator end() const noexcept {
		return Finish;
	}

	const_iterator cend() const noexcept {
		return Finish;
	}

	bool empty() const noexcept {
		return Finish == Start;
	}

	size_type size() const noexcept {
		return (reinterpret_cast<uintptr_t>(Finish) - reinterpret_cast<uintptr_t>(Start)) / sizeof(value_type);
	}

	size_type capacity() const noexcept {
		return (reinterpret_cast<uintptr_t>(AllocFinish) - reinterpret_cast<uintptr_t>(Start)) / sizeof(value_type);
	}

	T *data() noexcept {
		return Start;
	}

	const T *data() const noexcept {
		return Start;
	}
};

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
DynArray<T, reverse_destruct, always_default_construct, Alloc>::~DynArray() noexcept {
	Destruct<value_type, reverse_destruct>::run_array(Start, Finish);
	Alloc::dealloc_n(Start, reinterpret_cast<uintptr_t>(AllocFinish) - reinterpret_cast<uintptr_t>(Start));
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
DynArray<T, reverse_destruct, always_default_construct, Alloc>::DynArray(const DynArray &a) noexcept {
	auto new_byte_size = reinterpret_cast<uintptr_t>(a.Finish) - reinterpret_cast<uintptr_t>(a.Start);
	if (new_byte_size > 0) {
		auto new_start = Alloc::template alloc_n<value_type>(new_byte_size);
		copy_construct_array(new_start, a.Start, a.Finish);
		Start = new_start;
		auto new_finish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + new_byte_size);
		Finish = new_finish;
		AllocFinish = new_finish;
	} else {
		Start = nullptr;
		Finish = nullptr;
		AllocFinish = nullptr;
	}
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
auto DynArray<T, reverse_destruct, always_default_construct, Alloc>::expand_one_alloc() noexcept -> value_type * {
	T *ptr;
	T *new_start;

	static_assert(sizeof(value_type) < Alloc::max_size() / 2);

	//Try allocating twice the current space
	auto cur_byte_size = reinterpret_cast<uintptr_t>(Finish) - reinterpret_cast<uintptr_t>(Start);
	if (cur_byte_size > 0) {
		if (EJ_LIKELY(cur_byte_size <= Alloc::max_size() / 2)) {
			auto new_byte_size = cur_byte_size * 2;
			if (is_reallocatable<value_type>::value) {
				new_start = Alloc::try_realloc_n(Start, cur_byte_size, new_byte_size);
				if (new_start != nullptr) {
					ptr = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size);
					Start = new_start;
					Finish = ptr + 1;
					AllocFinish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + new_byte_size);
					return ptr;
				} else {
					//Try allocating the exact size
					new_start = Alloc::realloc_n(Start, cur_byte_size, cur_byte_size + sizeof(value_type));
				}
			} else {
				new_start = Alloc::template try_alloc_n<value_type>(new_byte_size);
				if (new_start != nullptr) {
					move_construct_array(new_start, reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size), Start);
					Alloc::dealloc_n(Start, cur_byte_size);
					ptr = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size);
					Start = new_start;
					Finish = ptr + 1;
					AllocFinish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + new_byte_size);
					return ptr;
				} else {
					//Try allocating the exact size
					new_start = Alloc::template alloc_n<value_type>(cur_byte_size + sizeof(value_type));
					move_construct_array(new_start, reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size), Start);
					Alloc::dealloc_n(Start, cur_byte_size);
				}
			}
		} else if (cur_byte_size <= Alloc::max_size() - sizeof(value_type)) {
			//Try allocating the exact size
			if (ej::is_reallocatable<value_type>::value) {
				new_start = Alloc::realloc_n(Start, cur_byte_size, cur_byte_size + sizeof(value_type));
			} else {
				new_start = Alloc::template alloc_n<value_type>(cur_byte_size + sizeof(value_type));
				move_construct_array(new_start, reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size), Start);
				Alloc::dealloc_n(Start, cur_byte_size);
			}
		} else {
			abort();
		}

		ptr = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size);
		Start = new_start;
		auto new_finish = ptr + 1;
		Finish = new_finish;
		AllocFinish = new_finish;
		return ptr;
	} else {
		constexpr size_t min_size = 4 * sizeof(uintptr_t) <= sizeof(value_type) ? 1 : 4 * sizeof(uintptr_t) / sizeof(value_type);
		ptr = Alloc::template alloc_n<value_type>(min_size * sizeof(value_type));
		Start = ptr;
		Finish = ptr + 1;
		AllocFinish = ptr + min_size;
		return ptr;
	}
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
DynArray<T, reverse_destruct, always_default_construct, Alloc> &DynArray<T, reverse_destruct, always_default_construct, Alloc>::operator =(const DynArray &a) noexcept {
	if (this != &a) {
		Destruct<value_type, reverse_destruct>::run_array(Start, Finish);
		Alloc::dealloc_n(Start, reinterpret_cast<uintptr_t>(AllocFinish) - reinterpret_cast<uintptr_t>(Start));

		auto new_byte_size = reinterpret_cast<uintptr_t>(a.Finish) - reinterpret_cast<uintptr_t>(a.Start);
		if (new_byte_size > 0) {
			auto new_start = Alloc::template alloc_n<value_type>(new_byte_size);
			copy_construct_array(new_start, a.Start, a.Finish);
			Start = new_start;
			auto new_finish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + new_byte_size);
			Finish = new_finish;
			AllocFinish = new_finish;
		} else {
			Start = nullptr;
			Finish = nullptr;
			AllocFinish = nullptr;
		}
	}
	return *this;
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
DynArray<T, reverse_destruct, always_default_construct, Alloc> &DynArray<T, reverse_destruct, always_default_construct, Alloc>::operator =(DynArray &&a) noexcept {
	auto start = Start, finish = Finish, alloc_finish = AllocFinish;
	Start = a.Start;
	Finish = a.Finish;
	AllocFinish = a.AllocFinish;
	a.Start = start;
	a.Finish = finish;
	a.AllocFinish = alloc_finish;
	return *this;
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
void DynArray<T, reverse_destruct, always_default_construct, Alloc>::resize(size_type n) noexcept {
	if (n <= capacity()) {
		auto cur_size = size();
		auto finish = Finish;
		auto new_finish = Start + n;
		Finish = new_finish;
		if (n > cur_size) {
			default_construct_array<value_type, always_default_construct>(finish, new_finish);
		} else {
			destruct_array(new_finish, finish);
		}
	} else {
		if (n <= Alloc::max_size() / sizeof(value_type)) {
			auto new_byte_size = n * sizeof(value_type);
			auto start = Start;
			if (is_reallocatable<value_type>::value) {
				auto new_start = Alloc::realloc_n(start, reinterpret_cast<uintptr_t>(AllocFinish) - reinterpret_cast<uintptr_t>(start), new_byte_size);
				auto cur_byte_size = reinterpret_cast<uintptr_t>(Finish) - reinterpret_cast<uintptr_t>(start);
				auto finish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size);
				auto new_finish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + new_byte_size);

				Start = new_start;
				Finish = new_finish;
				AllocFinish = new_finish;

				default_construct_array<value_type, always_default_construct>(finish, new_finish);
			} else {
				auto new_start = Alloc::template alloc_n<value_type>(new_byte_size);
				auto new_finish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + new_byte_size);

				auto finish = Finish;
				auto byte_capacity = reinterpret_cast<uintptr_t>(AllocFinish) - reinterpret_cast<uintptr_t>(start);
				Start = new_start;
				Finish = new_finish;
				AllocFinish = new_finish;

				move_construct_array(new_start, start, finish);
				Alloc::dealloc_n(start, byte_capacity);
				auto cur_byte_size = reinterpret_cast<uintptr_t>(finish) - reinterpret_cast<uintptr_t>(start);
				default_construct_array<value_type, always_default_construct>(reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size), new_finish);
			}
		} else {
			abort();
		}
	}
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
void DynArray<T, reverse_destruct, always_default_construct, Alloc>::reserve(size_type n) noexcept {
	if (n > capacity()) {
		if (n <= Alloc::max_size() / sizeof(value_type)) {
			auto new_byte_size = n * sizeof(value_type);
			auto start = Start;
			if (is_reallocatable<value_type>::value) {
				auto new_start = Alloc::realloc_n(start, reinterpret_cast<uintptr_t>(AllocFinish) - reinterpret_cast<uintptr_t>(start), new_byte_size);
				auto cur_byte_size = reinterpret_cast<uintptr_t>(Finish) - reinterpret_cast<uintptr_t>(start);
				auto new_finish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + cur_byte_size);
				Start = new_start;
				Finish = new_finish;
				AllocFinish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + new_byte_size);
			} else {
				auto new_start = Alloc::template alloc_n<value_type>(n * sizeof(value_type));

				auto finish = Finish;
				auto byte_size = reinterpret_cast<uintptr_t>(finish) - reinterpret_cast<uintptr_t>(start);
				auto new_finish = reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(new_start) + byte_size);
				auto byte_capacity = reinterpret_cast<uintptr_t>(AllocFinish) - reinterpret_cast<uintptr_t>(start);
				Start = new_start;
				Finish = new_finish;
				AllocFinish = new_start + n;

				move_construct_array(new_start, start, finish);
				Alloc::dealloc_n(start, byte_capacity);
			}
		} else {
			abort();
		}
	}
}

}

#endif
