//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

namespace ej {

inline uint32_t multiply_high(uint32_t a, uint32_t b) noexcept {
	return static_cast<uint32_t>((static_cast<uint64_t>(a) * b) >> 32);
}

#ifdef __LP64__

inline uint64_t multiply_high(uint64_t a, uint64_t b) noexcept {
	return static_cast<uint64_t>((static_cast<unsigned __int128>(a) * b) >> 64);
}

#endif

}
