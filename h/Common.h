//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_COMMON_H
#define EJ_COMMON_H

#include <stddef.h>

#define EJ_ALWAYS_INLINE	inline __attribute__((always_inline))
#define EJ_LIKELY(cond)		__builtin_expect(!!(cond), true)
#define EJ_UNLIKELY(cond)	__builtin_expect(!!(cond), false)

namespace ej {

enum : size_t {
	PAGE_SIZE = 4096,
};

}

#endif
