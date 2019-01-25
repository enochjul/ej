//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

#include "duint.h"

namespace ej {

inline uint32_t multiply_high(uint32_t a, uint32_t b) noexcept {
	return duint32_get_high(duint32_mul(a, b));
}

inline uint64_t multiply_high(uint64_t a, uint64_t b) noexcept {
	return duint64_get_high(duint64_mul(a, b));
}

}
