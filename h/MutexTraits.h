//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

namespace ej {

//! Checks if the specified T is a mutex
template <class T>
struct is_mutex final {
	enum : bool {
		value = true
	};
};

}
