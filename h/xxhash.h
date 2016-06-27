//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_XXHASH_H
#define EJ_XXHASH_H

#include <stddef.h>
#include <stdint.h>

namespace ej {

uint32_t xxhash_32(const void *s, size_t n, uint32_t seed = 0) noexcept;
uint64_t xxhash_64(const void *s, size_t n, uint64_t seed = 0) noexcept;

}

#endif
