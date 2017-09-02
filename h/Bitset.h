//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_BITSET_H
#define EJ_BITSET_H

#include <assert.h>
#include <limits.h>

#include <limits>

#include "CAlloc.h"
#include "fill.h"

namespace ej {

template <typename T = unsigned, class Alloc = CAlloc>
class Bitset {
public:
	typedef T block_type;
	typedef size_t size_type;

private:
	enum : size_type {
		NUMBER_OF_BLOCK_TYPE_BITS = sizeof(block_type) * CHAR_BIT
	};

	T *Start;
	size_type Size;

public:
	Bitset(size_type n, bool value = false) noexcept;

	Bitset(const Bitset &) = delete;
	Bitset &operator =(const Bitset &) = delete;

	bool get(size_type pos) const noexcept {
		assert(pos < Size);
		return get(Start, pos);
	}

	void set(size_type pos) noexcept {
		assert(pos < Size);

		block_type *block = Start + (pos / NUMBER_OF_BLOCK_TYPE_BITS);
		*block |= static_cast<block_type>(1) << (pos % NUMBER_OF_BLOCK_TYPE_BITS);
	}

	void set(size_type pos, bool value) noexcept {
		assert(pos < Size);
		set(Start, pos, value);
	}

	block_type *data() noexcept {
		return Start;
	}

	const block_type *data() const noexcept {
		return Start;
	}

	static bool get(block_type *data, size_type pos) noexcept {
		return (data[pos / NUMBER_OF_BLOCK_TYPE_BITS] & (static_cast<block_type>(1) << (pos % NUMBER_OF_BLOCK_TYPE_BITS))) != 0;
	}

	static void set(block_type *data, size_type pos, bool value) noexcept {
		auto *block = data + (pos / NUMBER_OF_BLOCK_TYPE_BITS);
		size_type mask = static_cast<block_type>(1) << (pos % NUMBER_OF_BLOCK_TYPE_BITS);
		if (value) {
			*block |= mask;
		} else {
			*block &= ~mask;
		}
	}
};

template <typename T, class Alloc>
Bitset<T, Alloc>::Bitset(size_type n, bool value) noexcept {
	if (n > 0) {
		//Allocate memory for the bitset
		if (n <= std::numeric_limits<size_type>::max() - NUMBER_OF_BLOCK_TYPE_BITS + 1) {
			auto number_of_blocks = (n + NUMBER_OF_BLOCK_TYPE_BITS - 1) / NUMBER_OF_BLOCK_TYPE_BITS;
			auto *new_start = Alloc::template alloc_array<block_type>(number_of_blocks);
			Start = new_start;
			Size = n;

			if (Alloc::always_zero()) {
				if (value) {
					fill(new_start, new_start + number_of_blocks, ~static_cast<block_type>(0));
				}
			} else {
				fill(new_start, new_start + number_of_blocks, value ? ~static_cast<block_type>(0) : static_cast<block_type>(0));
			}
		} else {
			abort();
		}
	} else {
		Start = nullptr, Size = 0;
	}
}

}

#endif
