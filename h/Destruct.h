//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_DESTRUCT_H
#define EJ_DESTRUCT_H

#include <stddef.h>

#include <type_traits>
#include <utility>

namespace ej {

//! Helper class to call the destructor when deallocating memory
template <class T, bool reverse = false, bool trivial = std::is_trivially_destructible<T>::value>
class Destruct;

template <class T>
class Destruct<T, false, false> {
public:
	static void run(T *ptr) noexcept(std::is_nothrow_destructible<T>::value) {
		ptr->~T();
	}

	static void run_array(T *first, T *last) noexcept(std::is_nothrow_destructible<T>::value) {
		for (auto ptr = first; ptr != last; ++ptr) {
			ptr->~T();
		}
	}

	static void run_array_n(T *first, size_t n) noexcept(std::is_nothrow_destructible<T>::value) {
		for (auto ptr = first, last = first + n; ptr != last; ++ptr) {
			ptr->~T();
		}
	}
};

template <class T>
class Destruct<T, true, false> {
public:
	static void run(T *ptr) noexcept(std::is_nothrow_destructible<T>::value) {
		ptr->~T();
	}

	static void run_array(T *first, T *last) noexcept(std::is_nothrow_destructible<T>::value) {
		for (auto ptr = last; ptr != first; --ptr) {
			(ptr - 1)->~T();
		}
	}

	static void run_array_n(T *first, size_t n) noexcept(std::is_nothrow_destructible<T>::value) {
		for (auto ptr = first + n; ptr != first; --ptr) {
			(ptr - 1)->~T();
		}
	}
};

template <class T, bool reverse>
class Destruct<T, reverse, true> {
public:
	static void run(T *) noexcept {
	}

	static void run_array(T *, T *) noexcept {
	}

	static void reverse_run_array(T *, T *) noexcept {
	}

	static void run_array_n(T *, size_t) noexcept {
	}

	static void reverse_run_array(T *, size_t) noexcept {
	}
};

//! Calls the destructor for the specified object.
template <class T>
inline void destruct(T *ptr) noexcept(std::is_nothrow_destructible<T>::value) {
	ptr->~T();
}

//! Calls the destructor for objects in the range of [first, last)
template <class T>
inline void destruct_array(T *first, T *last) noexcept(std::is_nothrow_destructible<T>::value) {
	Destruct<T, false>::run_array(first, last);
}

//! Calls the destructor for objects in the range of [first, last) in the reverse order
template <class T>
inline void reverse_destruct_array(T *first, T *last) noexcept(std::is_nothrow_destructible<T>::value) {
	Destruct<T, true>::reverse_run_array(first, last);
}

//! Calls the destructor for objects in the range of [first, first + n)
template <class T>
inline void destruct_array_n(T *first, size_t n) noexcept(std::is_nothrow_destructible<T>::value) {
	Destruct<T, false>::run_array_n(first, n);
}

//! Calls the destructor for objects in the range of [first, first + n) in the reverse order
template <class T>
inline void reverse_destruct_array_n(T *first, size_t n) noexcept(std::is_nothrow_destructible<T>::value) {
	Destruct<T, true>::run_array_n(first, n);
}

}

#endif
