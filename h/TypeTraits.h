//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_TYPE_TRAITS_H
#define EJ_TYPE_TRAITS_H

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

}

#endif
