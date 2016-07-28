//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_PAGE_ARRAY_H
#define EJ_PAGE_ARRAY_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "CAlloc.h"
#include "CallType.h"
#include "Construct.h"
#include "Destruct.h"

namespace ej {

template <typename T, size_t M, bool always_default_construct, class Alloc>
class PageArrayBase {
	static_assert(M > 0);

public:
	typedef T value_type;
	typedef size_t size_type;

protected:
	value_type **PagesFirst;
	value_type **PagesReservedLast;
	size_type Size;
	size_type Capacity;

	value_type *alloc() noexcept;

public:
	constexpr PageArrayBase() noexcept : PagesFirst(nullptr), PagesReservedLast(nullptr), Size(0), Capacity(0) {
	}

	PageArrayBase(const PageArrayBase &x) noexcept(std::is_nothrow_copy_constructible<value_type>::value) = delete;
	PageArrayBase &operator =(const PageArrayBase &x) noexcept(std::is_nothrow_copy_assignable<value_type>::value) = delete;

	bool empty() const noexcept {
		return Size == 0;
	}

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

	T &operator ()(size_type pos) noexcept {
		return operator [](pos);
	}

	const T &operator ()(size_type pos) const noexcept {
		return operator [](pos);
	}

	T &front() noexcept {
		assert(Size > 0);
		return operator [](0);
	}

	const T &cfront() const noexcept {
		assert(Size > 0);
		return operator [](0);
	}

	const T &front() const noexcept {
		return cfront();
	}

	T &back() noexcept {
		assert(Size > 0);
		return operator [](Size - 1);
	}

	const T &cback() const noexcept {
		assert(Size > 0);
		return operator [](Size - 1);
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
		return cback();
	}

	value_type &push() noexcept(std::is_nothrow_default_constructible<value_type>::value) {
		auto new_object = alloc();
		default_construct<value_type, always_default_construct>(new_object);
		return *new_object;
	}
	value_type &push_back() noexcept(std::is_nothrow_default_constructible<value_type>::value) {
		return push();
	}
	value_type &append() noexcept(std::is_nothrow_default_constructible<value_type>::value) {
		return push();
	}

	value_type &push(typename CallType<value_type>::param_type value) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		auto new_object = alloc();
		copy_construct(new_object, value);
		return *new_object;
	}
	value_type &push_back(typename CallType<value_type>::param_type value) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		return push(value);
	}
	value_type &append(typename CallType<value_type>::param_type value) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		return push(value);
	}

	template <typename ... Args>
	value_type &emplace_back(Args && ... a) noexcept(std::is_nothrow_constructible<T, Args ...>::value) {
		auto new_object = alloc();
		forward_construct(new_object, std::forward<Args>(a) ...);
		return *new_object;
	}

	void pop() noexcept(std::is_nothrow_destructible<value_type>::value) {
		assert(Size > 0);
		auto n = Size;
		n--;
		Size = n;
		auto *obj = PagesFirst[n / M] + (n % M);
		destruct(obj);
	}
	void pop_back() noexcept(std::is_nothrow_destructible<value_type>::value) {
		pop();
	}

	void pop(T *x) noexcept(std::is_nothrow_copy_assignable<value_type>::value && std::is_nothrow_destructible<value_type>::value) {
		assert(Size > 0);

		auto n = Size;
		n--;
		auto *obj = PagesFirst[n / M] + (n % M);
		*x = *obj;
		Size = n;
		destruct(obj);
	}
	void pop_back(T *x) noexcept(std::is_nothrow_copy_assignable<value_type>::value && std::is_nothrow_destructible<value_type>::value) {
		pop(x);
	}

	//! Applies the given function to each object
	template <typename Function>
	void for_each(Function f) {
		value_type *const *page_begin;
		value_type *const *page_iter;
		value_type *const *page_end;

		//Iterate through the objects in full pages
		auto n = Size;
		page_begin = PagesFirst;
		for (page_iter = page_begin, page_end = page_begin + (n / M); page_iter != page_end; ++page_iter) {
			auto page = *page_iter;
			for (auto obj_iter = page, obj_end = page + M; obj_iter != obj_end; ++obj_iter) {
				f(*obj_iter);
			}
		}

		//Check and iterate through the objects in the partial page
		auto last_page_size = n % M;
		if (last_page_size > 0) {
			auto page = *page_iter;
			for (auto obj_iter = page, obj_end = page + last_page_size; obj_iter != obj_end; ++obj_iter) {
				f(*obj_iter);
			}
		}
	}

	//! Applies the given function to each object
	template <typename Function>
	void cfor_each(Function f) const {
		const value_type *const *page_begin;
		const value_type *const *page_iter;
		const value_type *const *page_end;

		//Iterate through the objects in full pages
		auto n = Size;
		page_begin = PagesFirst;
		for (page_iter = page_begin, page_end = page_begin + (n / M); page_iter != page_end; ++page_iter) {
			auto page = *page_iter;
			for (auto obj_iter = page, obj_end = page + M; obj_iter != obj_end; ++obj_iter) {
				f(*obj_iter);
			}
		}

		//Check and iterate through the objects in the partial page
		auto last_page_size = n % M;
		if (last_page_size > 0) {
			auto page = *page_iter;
			for (auto obj_iter = page, obj_end = page + last_page_size; obj_iter != obj_end; ++obj_iter) {
				f(*obj_iter);
			}
		}
	}

	//! Applies the given function to each object
	template <typename Function>
	void for_each(Function f) const {
		cfor_each(f);
	}

	//! Applies the given function to each object, starting from the last object
	template <typename Function>
	void rfor_each(Function f) {
		auto n = this->Size;
		auto page_begin = this->PagesFirst;
		auto full_page_end = page_begin + (n / M);

		//Check and iterate through the objects in the partial page
		auto last_page_size = n % M;
		if (last_page_size > 0) {
			auto page = *full_page_end;
			for (auto obj_riter = page + last_page_size, obj_rend = page; obj_riter != obj_rend; --obj_riter) {
				f(*(obj_riter - 1));
			}
		}

		//Iterate through the objects in full pages
		for (auto page_riter = full_page_end, page_rend = page_begin; page_riter != page_rend; --page_riter) {
			auto page = *(page_riter - 1);
			for (auto obj_riter = page + M, obj_rend = page; obj_riter != obj_rend; --obj_riter) {
				f(*(obj_riter - 1));
			}
		}
	}

	//! Applies the given function to each object, starting from the last object
	template <typename Function>
	void crfor_each(Function f) const {
		auto n = this->Size;
		const auto *const *page_begin = this->PagesFirst;
		auto full_page_end = page_begin + (n / M);

		//Check and iterate through the objects in the partial page
		auto last_page_size = n % M;
		if (last_page_size > 0) {
			auto page = *full_page_end;
			for (auto obj_riter = page + last_page_size, obj_rend = page; obj_riter != obj_rend; --obj_riter) {
				f(*(obj_riter - 1));
			}
		}

		//Iterate through the objects in full pages
		for (auto page_riter = full_page_end, page_rend = page_begin; page_riter != page_rend; --page_riter) {
			auto page = *(page_riter - 1);
			for (auto obj_riter = page + M, obj_rend = page; obj_riter != obj_rend; --obj_riter) {
				f(*(obj_riter - 1));
			}
		}
	}

	//! Applies the given function to each object, starting from the last object
	template <typename Function>
	void rfor_each(Function f) const {
		crfor_each(f);
	}
};

template <typename T, size_t M, bool always_default_construct, class Alloc>
auto PageArrayBase<T, M, always_default_construct, Alloc>::alloc() -> value_type * {
	//Checks if all pages are full
	auto n = Size;
	auto number_of_full_pages = n / M;

	assert(Size <= Capacity);

	auto page_begin = PagesFirst;
	if (n >= Capacity) {
		//Double the number of page entries if needed
		if ((page_begin + number_of_full_pages) == PagesReservedLast) {
			size_type new_number_of_pages = number_of_full_pages > 0 ? number_of_full_pages * 2 : 4;
			page_begin = Alloc::template realloc_array<value_type *>(page_begin, new_number_of_pages);
			PagesFirst = page_begin;
			PagesReservedLast = page_begin + new_number_of_pages;
		}
		//Allocate a new page
		page_begin[number_of_full_pages] = Alloc::template alloc_array<value_type>(M);
		Capacity += M;
	}

	Size = n + 1;
	return page_begin[number_of_full_pages] + (n % M);
}

//! Represents an array using an array of arrays, and each individual array contains a fixed number of objects.
template <
	typename T,
	//! Number of objects per array
	size_t M = 16,
	//! Calls the destructor for each object in reverse order
	bool reverse_destruct = false,
	//! Always call the default constructor if no parameters are supplied to functions such as push()/resize()
	bool always_default_construct = true,
	class Alloc = CAlloc,
	bool trivially_destructible = std::is_trivially_destructible<T>::value>
class PageArray;

template <typename T, size_t M, bool always_default_construct, class Alloc>
class PageArray<T, M, false, always_default_construct, Alloc, false> : public PageArrayBase<T, M, always_default_construct, Alloc> {
	typedef PageArrayBase<T, M, always_default_construct, Alloc> super;

public:
	using typename super::value_type;

	~PageArray() noexcept(std::is_nothrow_destructible<value_type>::value);

	void clear() noexcept(std::is_nothrow_destructible<value_type>::value);
};

template <typename T, size_t M, bool always_default_construct, class Alloc>
PageArray<T, M, false, always_default_construct, Alloc, false>::~PageArray() noexcept(std::is_nothrow_destructible<value_type>::value) {
	value_type **page_begin;
	value_type **page_iter;
	value_type **page_end;

	//Destroy and deallocate the objects in full pages
	auto n = this->Size;
	page_begin = this->PagesFirst;
	for (page_iter = page_begin, page_end = page_begin + (n / M); page_iter != page_end; ++page_iter) {
		auto page = *page_iter;
		destruct_array_n(page, M);
		Alloc::dealloc_array(page, M);
	}

	//Check and destroy the objects in the partial page
	auto last_page_size = n % M;
	if (last_page_size > 0) {
		auto page = *page_iter;
		destruct_array_n(page, last_page_size);
	}

	//Destroy the partial page (if present) and reserved but unused pages
	assert((this->Capacity % M) == 0);
	for (page_end = page_begin + (this->Capacity / M); page_iter != page_end; ++page_iter) {
		auto page = *page_iter;
		Alloc::dealloc_array(page, M);
	}

	Alloc::dealloc_array(page_begin, this->PagesReservedLast - page_begin);
}

template <typename T, size_t M, bool always_default_construct, class Alloc>
void PageArray<T, M, false, always_default_construct, Alloc, false>::clear() noexcept(std::is_nothrow_destructible<value_type>::value) {
	value_type **page_begin;
	value_type **page_iter;
	value_type **page_end;

	//Destroy the objects in full pages
	auto n = this->Size;
	page_begin = this->PagesFirst;
	for (page_iter = page_begin, page_end = page_begin + (n / M); page_iter != page_end; ++page_iter) {
		auto page = *page_iter;
		destruct_array_n(page, M);
	}

	//Check and destroy the objects in the partial page
	auto last_page_size = n % M;
	if (last_page_size > 0) {
		auto page = *page_iter;
		destruct_array_n(page, last_page_size);
	}

	this->Size = 0;
}

template <typename T, size_t M, bool always_default_construct, class Alloc>
class PageArray<T, M, true, always_default_construct, Alloc, false> : public PageArrayBase<T, M, always_default_construct, Alloc> {
	typedef PageArrayBase<T, M, always_default_construct, Alloc> super;

public:
	using typename super::value_type;

	~PageArray() noexcept(std::is_nothrow_destructible<value_type>::value);

	void clear() noexcept(std::is_nothrow_destructible<value_type>::value);
};

template <typename T, size_t M, bool always_default_construct, class Alloc>
PageArray<T, M, true, always_default_construct, Alloc, false>::~PageArray() noexcept(std::is_nothrow_destructible<value_type>::value) {
	value_type **page_begin;
	value_type **full_page_end;
	value_type **reserved_page_end;
	value_type **page_riter;
	value_type **page_rend;

	assert((this->Capacity % M) == 0);

	auto n = this->Size;
	page_begin = this->PagesFirst;
	full_page_end = page_begin + (n / M);
	reserved_page_end = page_begin + (this->Capacity / M);

	//Check if there are objects in a partial page
	auto last_page_size = n % M;
	if (last_page_size > 0) {
		//Deallocates reserved but unused pages
		for (page_riter = reserved_page_end, page_rend = full_page_end + 1; page_riter != page_rend; --page_riter) {
			auto page = *(page_riter - 1);
			Alloc::dealloc_array(page, M);
		}

		//Destroy and deallocate the objects in the partial page
		auto page = *(page_riter - 1);
		reverse_destruct_array_n(page, last_page_size);
		Alloc::dealloc_array(page, M);
	} else {
		//Deallocates reserved but unused pages
		for (page_riter = reserved_page_end, page_rend = full_page_end; page_riter != page_rend; --page_riter) {
			auto page = *(page_riter - 1);
			Alloc::dealloc_array(page, M);
		}
	}

	//Destroy and deallocate objects in the full pages
	for (page_riter = full_page_end, page_rend = page_begin; page_riter != page_rend; --page_riter) {
		auto page = *(page_riter - 1);
		reverse_destruct_array_n(page, M);
		Alloc::dealloc_array(page, M);
	}

	Alloc::dealloc_array(page_begin, this->PagesReservedLast - page_begin);
}

template <typename T, size_t M, bool always_default_construct, class Alloc>
void PageArray<T, M, true, always_default_construct, Alloc, false>::clear() noexcept(std::is_nothrow_destructible<value_type>::value) {
	auto n = this->Size;
	auto page_begin = this->PagesFirst;
	auto full_page_end = page_begin + (n / M);

	//Check and destroy the elements on a partial page
	auto last_page_size = n % M;
	if (last_page_size > 0) {
		auto page = *full_page_end;
		reverse_destruct_array_n(page, last_page_size);
	}

	//Destroy the elements on full pages
	for (auto page_riter = full_page_end, page_rend = page_begin; page_riter != page_rend; --page_riter) {
		auto page = *(page_riter - 1);
		reverse_destruct_array_n(page, M);
	}

	this->Size = 0;
}

template <typename T, size_t M, bool reverse_destruct, bool always_default_construct, class Alloc>
class PageArray<T, M, reverse_destruct, always_default_construct, Alloc, true> : public PageArrayBase<T, M, always_default_construct, Alloc> {
	typedef PageArrayBase<T, M, always_default_construct, Alloc> super;

public:
	using typename super::value_type;

	~PageArray() noexcept;

	void clear() noexcept {
		this->Size = 0;
	}
};

template <typename T, size_t M, bool reverse_destruct, bool always_default_construct, class Alloc>
PageArray<T, M, reverse_destruct, always_default_construct, Alloc, true>::~PageArray() noexcept {
	assert((this->Capacity % M) == 0);

	auto page_begin = this->PagesFirst;
	for (auto page_iter = page_begin, page_end = page_begin + (this->Capacity / M); page_iter != page_end; ++page_iter) {
		auto page = *page_iter;
		Alloc::dealloc_array(page, M);
	}
	Alloc::dealloc_array(page_begin, this->PagesReservedLast - page_begin);
}

}

#endif
