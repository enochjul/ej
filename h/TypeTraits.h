//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

#include <limits>
#include <type_traits>

namespace ej {

template <typename T>
struct is_reallocatable final {
	enum : bool {
		value = std::is_trivially_move_constructible<T>::value && std::is_trivially_destructible<T>::value
	};
};

template <typename T>
struct is_zero_default_constructible final {
	enum : bool {
		value = std::is_trivially_constructible<T>::value
	};
};

template <typename T>
struct is_any_char final {
	enum : bool {
		value = false
	};
};

template <>
struct is_any_char<char> final {
	enum : bool {
		value = true
	};
};

template <>
struct is_any_char<signed char> final {
	enum : bool {
		value = true
	};
};

template <>
struct is_any_char<unsigned char> final {
	enum : bool {
		value = true
	};
};

template <uint64_t M>
struct unsigned_value_t {
	typedef typename std::conditional<
		M <= UINT8_MAX,
		uint8_t,
		typename std::conditional<
			M <= UINT16_MAX,
			uint16_t,
			typename std::conditional<
				M <= UINT32_MAX,
				uint32_t,
				uint64_t>::type>::type>::type type;

	static_assert(M <= std::numeric_limits<type>::max());
};

template <int64_t L, uint64_t R>
struct signed_range_t {
	typedef typename std::conditional<
		L >= INT8_MIN && R <= INT8_MAX,
		int8_t,
		typename std::conditional<
			L >= INT16_MIN && R <= INT16_MAX,
			int16_t,
			typename std::conditional<
				L >= INT32_MIN && R <= INT32_MAX,
				int32_t,
				int64_t>::type>::type>::type type;

	static_assert(L >= std::numeric_limits<type>::min() && R <= std::numeric_limits<type>::max());
};

}
