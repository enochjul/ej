//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_C_STRING_VIEW_H
#define EJ_C_STRING_VIEW_H

#include <stddef.h>
#include <string.h>

namespace ej {

template <typename T = const char>
class CStringView {
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef value_type *iterator;
	typedef const value_type *const_iterator;

private:
	value_type *Data;

public:
	constexpr CStringView(value_type *s) noexcept : Data(s) {
	}
	constexpr CStringView(const CStringView &) noexcept = default;

	constexpr CStringView &operator =(value_type *s) noexcept {
		Data = s;
	}
	constexpr CStringView &operator =(const CStringView &) noexcept = default;

	constexpr bool empty() const noexcept {
		return *Data == '\0';
	}
	constexpr size_type size() const noexcept {
		return strlen(Data);
	}
	value_type *data() noexcept {
		return Data;
	}
	constexpr value_type *data() const noexcept {
		return Data;
	}

	iterator begin() noexcept {
		return Data;
	}
	constexpr const_iterator begin() const noexcept {
		return Data;
	}
	constexpr const_iterator cbegin() const noexcept {
		return Data;
	}
	iterator end() noexcept {
		return Data + size();
	}
	const_iterator end() const noexcept {
		return Data + size();
	}
	const_iterator cend() const noexcept {
		return Data + size();
	}

	value_type &operator [](size_type index) noexcept {
		assert(index >= 0 && index < size());
		return Data[index];
	}
	constexpr const value_type &operator [](size_type index) const noexcept {
		return Data[index];
	}

	constexpr void remove_prefix(size_type n) noexcept {
		Data += n;
	}

	constexpr void swap(CStringView &a) noexcept {
		value_type *s = a.Data;
		a.Data = Data;
		Data = s;
	}

	value_type *find(value_type ch, size_type index = 0) noexcept {
		return strchr(Data + index, ch);
	}

	constexpr const value_type *find(value_type ch, size_type index = 0) const noexcept {
		return strchr(Data + index, ch);
	}

	value_type *find(const value_type *s, size_type index = 0) noexcept {
		return strstr(Data + index, s);
	}

	constexpr const value_type *find(const value_type *s, size_type index = 0) const noexcept {
		return strstr(Data + index, s);
	}
};

template <typename T>
inline constexpr CStringView<T> make_c_string_view(T *ptr) noexcept {
	return CStringView<T>(ptr);
}

template <typename T>
inline constexpr CStringView<T> make_const_c_string_view(const T *ptr) noexcept {
	return CStringView<T>(ptr);
}

}

#endif
