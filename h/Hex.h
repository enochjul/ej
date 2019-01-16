//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

namespace ej {

template <typename T>
struct Hex {
	T Value;
};

EJ_ALWAYS_INLINE auto hex(char value) noexcept {
	return Hex<uint32_t>{static_cast<unsigned char>(value)};
}

EJ_ALWAYS_INLINE auto hex(signed char value) noexcept {
	return Hex<uint32_t>{static_cast<unsigned char>(value)};
}

EJ_ALWAYS_INLINE auto hex(unsigned char value) noexcept {
	return Hex<uint32_t>{value};
}

EJ_ALWAYS_INLINE auto hex(int16_t value) noexcept {
	return Hex<uint32_t>{static_cast<uint16_t>(value)};
}

EJ_ALWAYS_INLINE auto hex(uint16_t value) noexcept {
	return Hex<uint32_t>{value};
}

EJ_ALWAYS_INLINE auto hex(int32_t value) noexcept {
	return Hex<uint32_t>{static_cast<uint32_t>(value)};
}

EJ_ALWAYS_INLINE auto hex(uint32_t value) noexcept {
	return Hex<uint32_t>{value};
}

EJ_ALWAYS_INLINE auto hex(int64_t value) noexcept {
	return Hex<uint64_t>{static_cast<uint64_t>(value)};
}

EJ_ALWAYS_INLINE auto hex(uint64_t value) noexcept {
	return Hex<uint64_t>{value};
}

}
