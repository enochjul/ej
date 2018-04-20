//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_LOG2_H
#define EJ_LOG2_H

#include "BitScan.h"

namespace ej {

template <typename T>
struct CeilLog2ReturnType {
	T Result;
	bool Valid;
};

//! Computes the ceiling of the log base 2 of the specified value
inline CeilLog2ReturnType<uint16_t> ceil_log2_16(uint16_t value) noexcept {
	auto rv = bsr16(value);
	if (!rv.ZeroValue) {
		auto count = rv.Count;
		if ((value & (value - 1)) != 0) {
			count += 1;
		}
		return { count, true };
	} else {
		return { 0, false };
	}
}

//! Computes the ceiling of the log base 2 of the specified value
inline CeilLog2ReturnType<uint32_t> ceil_log2_32(uint32_t value) noexcept {
	auto rv = bsr32(value);
	if (!rv.ZeroValue) {
		auto count = rv.Count;
		if ((value & (value - 1)) != 0) {
			count += 1;
		}
		return { count, true };
	} else {
		return { 0, false };
	}
}

//! Computes the ceiling of the log base 2 of the specified value
inline CeilLog2ReturnType<uint64_t> ceil_log2_64(uint64_t value) noexcept {
	auto rv = bsr64(value);
	if (!rv.ZeroValue) {
		auto count = rv.Count;
		if ((value & (value - 1)) != 0) {
			count += 1;
		}
		return { count, true };
	} else {
		return { 0, false };
	}
}

//! Computes the ceiling of the log base 2 of the specified value
template <typename T>
inline CeilLog2ReturnType<T> ceil_log2(T value) noexcept;

template <>
inline CeilLog2ReturnType<uint16_t> ceil_log2(uint16_t value) noexcept {
	return ceil_log2_16(value);
}

template <>
inline CeilLog2ReturnType<uint32_t> ceil_log2(uint32_t value) noexcept {
	return ceil_log2_32(value);
}

template <>
inline CeilLog2ReturnType<uint64_t> ceil_log2(uint64_t value) noexcept {
	return ceil_log2_64(value);
}

//! Computes the ceiling of the log base 2 of the specified value, suitable for use in a constant expression but may be slower
template <typename T>
constexpr inline CeilLog2ReturnType<T> ce_ceil_log2(T value) noexcept {
	T count = 0;
	bool valid = value != 0;

	while ((value >>= 1) != 0) {
		count++;
	}
	return { count, valid };
}

}

#endif
