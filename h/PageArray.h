//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_PAGE_ARRAY_H
#define EJ_PAGE_ARRAY_H

#include <stddef.h>
#include <stdlib.h>

#include "CallType.h"
#include "Construct.h"
#include "Destruct.h"

namespace ej {

//! Represents an array using an array of arrays, and each individual array contains a fixed number of objects.
template <
	typename T,
	//! Number of objects per array
	size_t M = 16,
	//! Always call the default constructor if no parameters are supplied to functions such as push()/resize()
	bool always_default_construct = true>
class PageArray {
	static_assert(M > 0);

public:
	typedef T value_type;
	typedef size_t size_type;

private:
	value_type **PagesFirst;
	value_type **PagesReservedLast;
	size_type Size;
	size_type Capacity;

public:
	constexpr PageArray() noexcept : PagesFirst(nullptr), PagesReservedLast(nullptr), Size(0), Capacity(0) {
	}
	~PageArray() noexcept(std::is_nothrow_destructible<T>::value);

	PageArray(const PageArray &x) noexcept(std::is_nothrow_copy_constructible<T>::value);
	PageArray &operator =(const PageArray &x) noexcept(std::is_nothrow_copy_assignable<T>::value);

	size_type size() const noexcept {
		return Size;
	}

	size_type capacity() const noexcept {
		return Capacity;
	}

	T &operator [](size_type pos) noexcept {
		assert(pos < Size);
		return PagesFirst[pos / M][pos % M];
	}

	const T &operator [](size_type pos) const noexcept {
		assert(pos < Size);
		return PagesFirst[pos / M][pos % M];
	}

	void push(typename CallType<T>::param_type value) noexcept(std::is_nothrow_copy_constructible<T>::value);
	void push_back(typename CallType<T>::param_type value) noexcept(std::is_nothrow_copy_constructible<T>::value) {
		return push(value);
	}
	void append(typename CallType<T>::param_type value) noexcept(std::is_nothrow_copy_constructible<T>::value) {
		return push(value);
	}

	void pop() noexcept(std::is_nothrow_destructible<T>::value) {
		assert(Size > 0);
		auto n = Size;
		n--;
		Size = n;
		auto last_page = PagesFirst[n / M];
		auto index = n % M;
		destruct(last_page + index);
	}

	void pop_back() noexcept(std::is_nothrow_destructible<T>::value) {
		return pop();
	}

	//! Applies the given function to each stored object
	template <typename Function>
	void for_each(Function f) {
		value_type **page_begin;
		value_type **page_iter;
		value_type **page_end;

		auto cur_size = Size;
		page_begin = PagesFirst;
		for (page_iter = page_begin, page_end = page_begin + (cur_size / M); page_iter != page_end; ++page_iter) {
			auto page = *page_iter;
			for (auto obj_iter = page, obj_end = page + M; obj_iter != obj_end; ++obj_iter) {
				f(*obj_iter);
			}
		}

		auto last_page_size = cur_size % M;
		if (last_page_size > 0) {
			auto page = *page_iter;
			for (auto obj_iter = page, obj_end = page + last_page_size; obj_iter != obj_end; ++obj_iter) {
				f(*obj_iter);
			}
		}
	}
};

template <typename T, size_t M, bool always_default_construct>
PageArray<T, M, always_default_construct>::~PageArray() noexcept(std::is_nothrow_destructible<T>::value) {
	value_type **page_begin;
	value_type **page_iter;
	value_type **page_end;

	auto cur_size = Size;
	page_begin = PagesFirst;
	for (page_iter = page_begin, page_end = page_begin + (cur_size / M); page_iter != page_end; ++page_iter) {
		auto page = *page_iter;
		destruct_array(page, page + M);
		free(page);
	}

	size_type last_page_size = cur_size % M;
	if (last_page_size > 0) {
		auto page = *page_iter;
		destruct_array(page, page + last_page_size);
	}

	assert((Capacity % M) == 0);
	for (page_end = page_begin + (Capacity / M); page_iter != page_end; ++page_iter) {
		auto page = *page_iter;
		free(page);
	}

	free(PagesFirst);
}

template <typename T, size_t M, bool always_default_construct>
void PageArray<T, M, always_default_construct>::push(typename CallType<T>::param_type value) noexcept(std::is_nothrow_copy_constructible<T>::value) {
	//Checks if all pages are full
	auto cur_size = Size;
	size_type number_of_pages = cur_size / M;
	size_type index = cur_size % M;

	if (cur_size >= Capacity) {
		//Double the number of page entries if needed
		if ((PagesFirst + number_of_pages) == PagesReservedLast) {
			size_type new_number_of_pages = number_of_pages != 0 ? number_of_pages * 2 : 4;
			PagesFirst = static_cast<value_type **>(realloc(PagesFirst, sizeof(value_type *) * new_number_of_pages));
			PagesReservedLast = PagesFirst + new_number_of_pages;
		}
		//Allocate a new page
		PagesFirst[number_of_pages] = static_cast<value_type *>(malloc(sizeof(value_type) * M));
		Capacity += M;
	}
	auto page = PagesFirst[number_of_pages];
	copy_construct(page + index, value);
	Size++;
}

}

#endif
