//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_IDENTITITY_HASH_H
#define EJ_IDENTITITY_HASH_H

namespace ej {

template <typename T>
class IdentityHash {
public:
	typedef T value_type;

	static value_type eval(typename CallType<T>::param_type value) noexcept {
		return value;
	}
};

}

#endif
