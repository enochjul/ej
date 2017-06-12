//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_BIT_SCAN_H
#define EJ_BIT_SCAN_H

#include <stdint.h>

#ifdef __GCC_ASM_FLAG_OUTPUTS__

template <typename T>
struct BitScanReturnType {
	T Count;
	bool ZeroValue;
};

//! x86 bit scan forward instruction
inline BitScanReturnType<uint16_t> bsf16(uint16_t value) noexcept {
	uint16_t count;
	bool zero_value;

	asm ("bsfw %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

//! x86 bit scan forward instruction
inline BitScanReturnType<uint32_t> bsf32(uint32_t value) noexcept {
	uint32_t count;
	bool zero_value;

	asm ("bsfl %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

//! x86 bit scan forward instruction
inline BitScanReturnType<uint64_t> bsf64(uint64_t value) noexcept {
	uint64_t count;
	bool zero_value;

	asm ("bsfq %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

//! x86 bit scan forward instruction
inline BitScanReturnType<uint16_t> bsr16(uint16_t value) noexcept {
	uint16_t count;
	bool zero_value;

	asm ("bsrw %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

//! x86 bit scan forward instruction
inline BitScanReturnType<uint32_t> bsr32(uint32_t value) noexcept {
	uint32_t count;
	bool zero_value;

	asm ("bsrl %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

//! x86 bit scan forward instruction
inline BitScanReturnType<uint64_t> bsr64(uint64_t value) noexcept {
	uint64_t count;
	bool zero_value;

	asm ("bsrq %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

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

#else
#error Needs GCC with support for flag outputs in inline assembly
#endif

#endif
