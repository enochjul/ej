//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_STRING_VIEW_H
#define EJ_STRING_VIEW_H

#include <stddef.h>

namespace ej {

template <typename T = const char, typename S = size_t>
class StringView {
public:
	typedef T value_type;
	typedef S size_type;
	typedef value_type *iterator;
	typedef const value_type *const_iterator;

private:
	value_type *First;
	size_type Size;

public:
	constexpr StringView() noexcept : First(nullptr), Size(0) {
	}
	constexpr StringView(value_type *first, value_type *last) noexcept : First(first), Size(last - first) {
	}
	constexpr StringView(value_type *ptr, size_type n) noexcept : First(ptr), Size(n) {
	}
	constexpr StringView(value_type data[]) noexcept : First(data), Size(strlen(data)) {
	}
	constexpr StringView(const StringView &) noexcept = default;
	constexpr StringView &operator =(const StringView &) noexcept = default;

	constexpr bool empty() const noexcept {
		return Size == 0;
	}
	constexpr size_type size() const noexcept {
		return Size;
	}

	iterator begin() noexcept {
		return First;
	}
	constexpr const_iterator begin() const noexcept {
		return First;
	}
	constexpr const_iterator cbegin() const noexcept {
		return First;
	}
	iterator end() noexcept {
		return First + Size;
	}
	constexpr const_iterator end() const noexcept {
		return First + Size;
	}
	constexpr const_iterator cend() const noexcept {
		return First + Size;
	}

	value_type &operator [](size_type index) noexcept {
		return First[index];
	}
	constexpr const value_type &operator [](size_type index) const noexcept {
		return First[index];
	}

	value_type *data() noexcept {
		return First;
	}
	constexpr const value_type *data() const noexcept {
		return First;
	}

	constexpr void remove_prefix(size_type n) noexcept {
		First += n;
		Size -= n;
	}
	constexpr void remove_suffix(size_type n) noexcept {
		Size -= n;
	}
};

template <typename T>
class StringView<T, void> {
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef value_type *iterator;
	typedef const value_type *const_iterator;

private:
	value_type *First;
	value_type *Last;

public:
	constexpr StringView() noexcept : First(nullptr), Last(nullptr) {
	}
	constexpr StringView(value_type *first, value_type *last) noexcept : First(first), Last(last) {
	}
	constexpr StringView(value_type *ptr, size_type n) noexcept : First(ptr), Last(ptr + n) {
	}
	constexpr StringView(value_type data[]) noexcept : First(data), Last(data + strlen(data)) {
	}
	constexpr StringView(const StringView &) noexcept = default;
	constexpr StringView &operator =(const StringView &) noexcept = default;

	constexpr bool empty() const noexcept {
		return Last == First;
	}
	constexpr size_type size() const noexcept {
		return Last - First;
	}

	iterator begin() noexcept {
		return First;
	}
	constexpr const_iterator begin() const noexcept {
		return First;
	}
	constexpr const_iterator cbegin() const noexcept {
		return First;
	}
	iterator end() noexcept {
		return Last;
	}
	constexpr const_iterator end() const noexcept {
		return Last;
	}
	constexpr const_iterator cend() const noexcept {
		return Last;
	}

	value_type &operator [](size_type index) noexcept {
		return First[index];
	}
	constexpr const value_type &operator [](size_type index) const noexcept {
		return First[index];
	}

	value_type *data() noexcept {
		return First;
	}
	constexpr const value_type *data() const noexcept {
		return First;
	}

	constexpr void remove_prefix(size_type n) noexcept {
		First += n;
	}
	constexpr void remove_suffix(size_type n) noexcept {
		Last -= n;
	}
};

template <typename T>
inline constexpr StringView<T, void> make_string_view(T *first, T *last) noexcept {
	return StringView<T, void>(first, last);
}

template <typename T>
inline constexpr StringView<T, void> make_string_view(T *ptr, typename StringView<T, void>::size_type n) noexcept {
	return StringView<T, void>(ptr, n);
}

template <typename T>
inline constexpr StringView<T, void> make_string_view(T data[]) noexcept {
	return StringView<T, void>(data);
}

template <typename T, typename S>
inline constexpr StringView<T, void> make_string_view(StringView<T, S> a) noexcept {
	return StringView<T, void>(a.begin(), a.end());
}

template <typename T, typename S>
inline constexpr StringView<const T, void> make_const_string_view(StringView<T, S> a) noexcept {
	return StringView<const T, void>(a.begin(), a.end());
}

template <typename T>
inline constexpr StringView<T> make_string_view_n(T *first, T *last) noexcept {
	return StringView<T>(first, last);
}

template <typename T>
inline constexpr StringView<T> make_string_view_n(T *ptr, typename StringView<T>::size_type n) noexcept {
	return StringView<T>(ptr, n);
}

template <typename T>
inline constexpr StringView<T> make_string_view_n(T data[]) noexcept {
	return StringView<T>(data);
}

template <typename T, typename S>
inline constexpr StringView<T> make_string_view_n(StringView<T, S> a) noexcept {
	return StringView<T>(a.data(), a.size());
}

template <typename T, typename S>
inline constexpr StringView<const T> make_const_string_view_n(StringView<T, S> a) noexcept {
	return StringView<const T>(a.data(), a.size());
}

}

#endif
