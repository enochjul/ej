//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <type_traits>

namespace ej {

//! Checks if the given type is a simple type that is better to pass by value to a function rather than by a const reference.
template <typename T, bool scalar = std::is_scalar<T>::value>
struct CallType;

template <typename T>
struct CallType<T, false> {
	typedef T value_type;

	typedef const value_type &param_type;
	typedef const value_type &return_type;
};

template <typename T>
struct CallType<T, true> {
	typedef T value_type;

	typedef value_type param_type;
	typedef value_type return_type;
};

}
