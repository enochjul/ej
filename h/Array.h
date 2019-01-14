//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_ARRAY_H
#define EJ_ARRAY_H

#include <stddef.h>

#include <algorithm>
#include <initializer_list>

#include "ArrayView.h"
#include "CAlloc.h"
#include "CallType.h"
#include "Construct.h"
#include "Destruct.h"
#include "fill.h"
#include "TypeTraits.h"

namespace ej {

//! A heap allocated array of objects using only 2 pointers but requires allocation whenever one changes its size.
template <
	typename T,
	//! Calls the destructor for each object in reverse order
	bool reverse_destruct = false,
	//! Always call the default constructor if no parameters are supplied to functions such as push()/resize()
	bool always_default_construct = true,
	class Alloc = CAlloc>
class Array {
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef T *iterator;
	typedef const T *const_iterator;

private:
	T *First;
	T *Last;

	static value_type *find_last(value_type *ptr, const value_type *first, const value_type *last) noexcept {
		assert(reinterpret_cast<uintptr_t>(last) >= reinterpret_cast<uintptr_t>(first));
		return reinterpret_cast<value_type *>(reinterpret_cast<uintptr_t>(ptr) + (reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first)));
	}

public:
	constexpr Array() noexcept : First(nullptr), Last(nullptr) {
	}
	explicit Array(size_type n) noexcept(std::is_nothrow_default_constructible<value_type>::value);
	Array(size_type n, typename CallType<value_type>::param_type value) noexcept;
	Array(const value_type *first, const value_type *last) noexcept(std::is_nothrow_copy_constructible<value_type>::value);
	template <size_t N>
	explicit Array(const value_type (&x)[N]) noexcept(std::is_nothrow_copy_constructible<value_type>::value) : Array(x, x + N) {
	}
	Array(std::initializer_list<value_type> x) noexcept(std::is_nothrow_copy_constructible<value_type>::value) : Array(x.begin(), x.end()) {
	}
	Array(const Array &x) noexcept(std::is_nothrow_copy_constructible<value_type>::value) : Array(x.First, x.Last) {
	}
	Array(Array &&x) noexcept {
		First = x.First, Last = x.Last;
		x.First = nullptr, x.Last = nullptr;
	}

	~Array() noexcept(std::is_nothrow_destructible<value_type>::value);

	template <size_t N>
	Array &operator =(const value_type (&x)[N]) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		assign(x);
		return *this;
	}
	Array &operator =(const Array &x) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		assign(x);
		return *this;
	}
	Array &operator =(Array &&x) noexcept {
		swap(x);
		return *this;
	}

	void assign(size_type n) noexcept(std::is_nothrow_default_constructible<value_type>::value);
	void assign(size_type n, typename CallType<value_type>::param_type value) noexcept(std::is_nothrow_copy_constructible<value_type>::value);
	void assign(const value_type *first, const value_type *last) noexcept(std::is_nothrow_copy_constructible<value_type>::value);
	template <size_t N>
	void assign(const value_type (&x)[N]) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		assign(x, x + N);
	}
	void assign(std::initializer_list<value_type> x) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		assign(x.begin(), x.end());
	}
	void assign(const Array &x) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		assign(x.First, x.Last);
	}

	void fill(typename CallType<value_type>::param_type value) noexcept(std::is_nothrow_copy_assignable<value_type>::value) {
		fill(First, Last, value);
	}

	void zero() noexcept(std::is_nothrow_copy_assignable<value_type>::value) {
		fill(First, Last, 0);
	}

	void clear() noexcept(std::is_nothrow_destructible<value_type>::value);

	void swap(Array &x) noexcept {
		auto first = First, last = Last;
		First = x.First, Last = x.Last;
		x.First = first, x.Last = last;
	}

	bool empty() const noexcept {
		return Last == First;
	}

	size_type size() const noexcept {
		return (reinterpret_cast<uintptr_t>(Last) - reinterpret_cast<uintptr_t>(First)) / sizeof(value_type);
	}

	T &operator [](size_type pos) noexcept {
		assert(pos < size());
		return First[pos];
	}

	const T &operator [](size_type pos) const noexcept {
		assert(pos < size());
		return First[pos];
	}

	T &operator ()(size_type pos) noexcept {
		return operator [](pos);
	}

	const T &operator ()(size_type pos) const noexcept {
		return operator [](pos);
	}

	T &front() noexcept {
		assert(size() > 0);
		return *First;
	}

	const T &cfront() const noexcept {
		assert(size() > 0);
		return *First;
	}

	const T &front() const noexcept {
		return cfront();
	}

	T &back() noexcept {
		assert(size() > 0);
		return *(Last - 1);
	}

	const T &cback() const noexcept {
		assert(size() > 0);
		return *(Last - 1);
	}

	const T &back() const noexcept {
		return cback();
	}

	T &top() noexcept {
		return back();
	}

	const T &ctop() const noexcept {
		return cback();
	}

	const T &top() const noexcept {
		return back();
	}

	iterator begin() noexcept {
		return First;
	}

	const_iterator cbegin() const noexcept {
		return First;
	}

	const_iterator begin() const noexcept {
		return First;
	}

	iterator end() noexcept {
		return Last;
	}

	const_iterator cend() const noexcept {
		return Last;
	}

	const_iterator end() const noexcept {
		return Last;
	}

	value_type *data() noexcept {
		return First;
	}

	const value_type *data() const noexcept {
		return First;
	}

	ArrayView<value_type> view() noexcept {
		return ArrayView<value_type>(First, Last);
	}

	ArrayView<const value_type> cview() const noexcept {
		return ArrayView<const value_type>(First, Last);
	}

	ArrayView<const value_type> view() const noexcept {
		return ArrayView<const value_type>(First, Last);
	}

	//! Applies the given function to each object
	template <typename Function>
	void for_each(Function f) noexcept {
		for (auto &obj : *this) {
			static_assert(noexcept(f(obj)));
			f(obj);
		}
	}

	//! Applies the given function to each object
	template <typename Function>
	void cfor_each(Function f) const noexcept {
		for (const auto &obj : *this) {
			static_assert(noexcept(f(obj)));
			f(obj);
		}
	}

	//! Applies the given function to each object
	template <typename Function>
	void for_each(Function f) const noexcept {
		cfor_each(f);
	}
};

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
Array<T, reverse_destruct, always_default_construct, Alloc>::Array(size_type n) noexcept(std::is_nothrow_default_constructible<value_type>::value) {
	if (n > 0) {
		auto new_first = Alloc::template alloc_array<value_type>(n);
		auto new_last = new_first + n;

		First = new_first, Last = new_last;
		default_construct_array<T, always_default_construct>(new_first, new_last);
	} else {
		First = nullptr, Last = nullptr;
	}
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
Array<T, reverse_destruct, always_default_construct, Alloc>::Array(size_type n, typename CallType<value_type>::param_type value) noexcept {
	if (n > 0) {
		auto new_first = Alloc::template alloc_array<value_type>(n);
		auto new_last = new_first + n;

		First = new_first, Last = new_last;
		copy_construct_fill_array(new_first, new_last, value);
	} else {
		First = nullptr, Last = nullptr;
	}
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
Array<T, reverse_destruct, always_default_construct, Alloc>::Array(const value_type *first, const value_type *last) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
	if (first != last) {
		auto new_first = Alloc::alloc_array_range(first, last);
		First = new_first, Last = find_last(new_first, first, last);
		copy_construct_array(new_first, first, last);
	} else {
		First = nullptr, Last = nullptr;
	}
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
Array<T, reverse_destruct, always_default_construct, Alloc>::~Array() noexcept(std::is_nothrow_destructible<value_type>::value) {
	auto first = First, last = Last;
	Destruct<value_type, reverse_destruct>::run_array(first, last);
	Alloc::dealloc_array_range(first, first, last);
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
void Array<T, reverse_destruct, always_default_construct, Alloc>::assign(size_type n) noexcept(std::is_nothrow_default_constructible<value_type>::value) {
	auto first = First, last = Last;
	Destruct<T, reverse_destruct>::run_array(first, last);
	auto cur_size = (reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first)) / sizeof(value_type);
	if (cur_size != n) {
		Alloc::dealloc_array(first, cur_size);
		if (n > 0) {
			first = Alloc::template alloc_array<value_type>(n);
			last = first + n;
			First = first;
			Last = last;
			default_construct_array_n<T, true>(first, n);
		} else {
			First = nullptr, Last = nullptr;
		}
	} else {
		default_construct_array_n<T, true>(first, n);
	}
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
void Array<T, reverse_destruct, always_default_construct, Alloc>::assign(size_type n, typename CallType<value_type>::param_type value) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
	value_type *new_first;
	value_type *new_last;
	if (n > 0) {
		new_first = Alloc::template alloc_array<value_type>(n);
		new_last = new_first + n;
		copy_construct_fill_array(new_first, new_last, value);
	} else {
		new_first = nullptr, new_last = nullptr;
	}

	auto first = First, last = Last;
	Destruct<T, reverse_destruct>::run_array(first, last);
	Alloc::dealloc_array_range(first, first, last);
	First = new_first, Last = new_last;
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
void Array<T, reverse_destruct, always_default_construct, Alloc>::assign(const value_type *first, const value_type *last) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
	value_type *new_first;
	if (first != last) {
		new_first = Alloc::alloc_array_range(first, last);
		copy_construct_array(new_first, first, last);
	} else {
		new_first = nullptr;
	}

	auto my_first = First, my_last = Last;
	Destruct<T, reverse_destruct>::run_array(my_first, my_last);
	Alloc::dealloc_array_range(my_first, my_first, my_last);
	First = new_first, Last = find_last(new_first, first, last);
}

template <typename T, bool reverse_destruct, bool always_default_construct, class Alloc>
void Array<T, reverse_destruct, always_default_construct, Alloc>::clear() noexcept(std::is_nothrow_destructible<value_type>::value) {
	auto first = First, last = Last;
	First = nullptr, Last = nullptr;
	Destruct<value_type, reverse_destruct>::run_array(first, last);
	Alloc::dealloc_array_range(first, first, last);
}

}

#endif
