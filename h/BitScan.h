//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <assert.h>
#include <stdint.h>

#include <x86intrin.h>

#ifdef __GCC_ASM_FLAG_OUTPUTS__

namespace ej {

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

//! x86 bit scan reverse instruction
inline BitScanReturnType<uint16_t> bsr16(uint16_t value) noexcept {
	uint16_t count;
	bool zero_value;

	asm ("bsrw %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

//! x86 bit scan reverse instruction
inline BitScanReturnType<uint32_t> bsr32(uint32_t value) noexcept {
	uint32_t count;
	bool zero_value;

	asm ("bsrl %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

//! x86 bit scan reverse instruction
inline BitScanReturnType<uint64_t> bsr64(uint64_t value) noexcept {
	uint64_t count;
	bool zero_value;

	asm ("bsrq %2, %1"
		: "=@ccz" (zero_value), "=r" (count)
		: "rm" (value)
		: "cc");
	return { count, zero_value };
}

}

#else
#error Needs GCC with support for flag outputs in inline assembly
#endif

//! x86 bit scan forward instruction, value must not be zero
inline unsigned bsf32_nz(uint32_t value) noexcept {
	assert(value != 0);
	return _bit_scan_forward(value);
}

//! x86 bit scan forward instruction, value must not be zero
inline unsigned bsf64_nz(uint64_t value) noexcept {
	assert(value != 0);
	return __bsfq(value);
}

//! x86 bit scan reverse instruction, value must not be zero
inline unsigned bsr32_nz(uint32_t value) noexcept {
	assert(value != 0);
	return _bit_scan_reverse(value);
}

//! x86 bit scan reverse instruction, value must not be zero
inline unsigned bsr64_nz(uint64_t value) noexcept {
	assert(value != 0);
	return __bsrq(value);
}
