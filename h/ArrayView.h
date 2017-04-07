//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_ARRAY_VIEW_H
#define EJ_ARRAY_VIEW_H

namespace ej {

//! A non-owning view of an array represented with two pointers.
template <typename T, typename S = size_t>
class ArrayView {
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef T *iterator;
	typedef const T *const_iterator;

private:
	T *First;
	size_type Size;

public:
	constexpr ArrayView() noexcept : First(nullptr), Size(0) {
	}
	constexpr ArrayView(value_type *first, value_type *last) noexcept : First(first), Size(last - first) {
	}
	constexpr ArrayView(value_type *ptr, size_type n) noexcept : First(ptr), Size(n) {
	}
	template <size_t N>
	explicit constexpr ArrayView(value_type (&x)[N]) noexcept : ArrayView(x, N) {
	}
	constexpr ArrayView(const ArrayView &) noexcept = default;
	constexpr ArrayView &operator =(const ArrayView &) noexcept = default;

	constexpr bool empty() const noexcept {
		return Size == 0;
	}

	constexpr size_type size() const noexcept {
		return Size;
	}

	constexpr T &operator [](size_type pos) const noexcept {
		return First[pos];
	}

	constexpr T &operator ()(size_type pos) const noexcept {
		return First[pos];
	}

	constexpr iterator begin() const noexcept {
		return First;
	}

	constexpr const_iterator cbegin() const noexcept {
		return First;
	}

	constexpr iterator end() const noexcept {
		return First + Size;
	}

	constexpr const_iterator cend() const noexcept {
		return First + Size;
	}

	constexpr value_type *data() const noexcept {
		return First;
	}

	//! Applies the given function to each object
	template <typename Function>
	void for_each(Function f) const {
		for (auto &obj : *this) {
			f(obj);
		}
	}

	//! Applies the given function to each object
	template <typename Function>
	void cfor_each(Function f) const {
		for (auto &obj : *this) {
			f(obj);
		}
	}
};

//! A non-owning view of an array represented with two pointers.
template <typename T>
class ArrayView<T, void> {
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef T *iterator;
	typedef const T *const_iterator;

private:
	T *First;
	T *Last;

public:
	constexpr ArrayView() noexcept : First(nullptr), Last(nullptr) {
	}
	constexpr ArrayView(value_type *first, value_type *last) noexcept : First(first), Last(last) {
	}
	constexpr ArrayView(value_type *ptr, size_type n) noexcept : First(ptr), Last(ptr + n) {
	}
	template <size_t N>
	explicit constexpr ArrayView(value_type (&x)[N]) noexcept : ArrayView(x, N) {
	}
	constexpr ArrayView(const ArrayView &) noexcept = default;

	constexpr bool empty() const noexcept {
		return Last == First;
	}

	constexpr size_type size() const noexcept {
		return Last - First;
	}

	constexpr T &operator [](size_type pos) const noexcept {
		return First[pos];
	}

	constexpr T &operator ()(size_type pos) const noexcept {
		return First[pos];
	}

	constexpr iterator begin() const noexcept {
		return First;
	}

	constexpr const_iterator cbegin() const noexcept {
		return First;
	}

	constexpr iterator end() const noexcept {
		return Last;
	}

	constexpr const_iterator cend() const noexcept {
		return Last;
	}

	constexpr value_type *data() const noexcept {
		return First;
	}

	//! Applies the given function to each object
	template <typename Function>
	void for_each(Function f) const {
		for (auto &obj : *this) {
			f(obj);
		}
	}

	//! Applies the given function to each object
	template <typename Function>
	void cfor_each(Function f) const {
		for (auto &obj : *this) {
			f(obj);
		}
	}
};

}

#endif
