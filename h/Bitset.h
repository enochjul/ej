//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

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
	Bitset() noexcept : Start(nullptr), Size(0) {
	}
	Bitset(size_type n, bool value = false) noexcept;
	~Bitset() noexcept {
		Alloc::dealloc_array(Start, (Size + NUMBER_OF_BLOCK_TYPE_BITS - 1) / NUMBER_OF_BLOCK_TYPE_BITS);
	}

	Bitset(const Bitset &a) noexcept;
	Bitset &operator =(const Bitset &a) noexcept;

	block_type *data() noexcept {
		return Start;
	}

	const block_type *data() const noexcept {
		return Start;
	}

	size_type size() const noexcept {
		return Size;
	}

	bool empty() const noexcept {
		return Size == 0;
	}

	bool get(size_type pos) const noexcept {
		return get(Start, Size, pos);
	}

	void set(size_type pos) noexcept {
		set(Start, Size, pos);
	}

	void set(size_type pos, bool value) noexcept {
		set(Start, Size, pos, value);
	}

	static bool get(block_type *data, size_type n, size_type pos) noexcept {
		assert(pos >= 0 && pos < n);
		return (data[pos / NUMBER_OF_BLOCK_TYPE_BITS] & (static_cast<block_type>(1) << (pos % NUMBER_OF_BLOCK_TYPE_BITS))) != 0;
	}

	static void set(block_type *data, size_type n, size_type pos) noexcept {
		assert(pos >= 0 && pos < n);
		auto *block = data + (pos / NUMBER_OF_BLOCK_TYPE_BITS);
		*block |= static_cast<block_type>(1) << (pos % NUMBER_OF_BLOCK_TYPE_BITS);
	}

	static void set(block_type *data, size_type n, size_type pos, bool value) noexcept {
		assert(pos >= 0 && pos < n);
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

			if constexpr (Alloc::AlwaysZero) {
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

template <typename T, class Alloc>
Bitset<T, Alloc>::Bitset(const Bitset &a) noexcept {
	auto n = a.Size;
	if (n > 0) {
		auto number_of_blocks = (n + NUMBER_OF_BLOCK_TYPE_BITS - 1) / NUMBER_OF_BLOCK_TYPE_BITS;
		auto *new_start = Alloc::template alloc_array<block_type>(number_of_blocks);
		Start = new_start;
		Size = n;
		memcpy(new_start, a.Start, number_of_blocks * sizeof(block_type));
	} else {
		Start = nullptr, Size = 0;
	}
}

template <typename T, class Alloc>
auto Bitset<T, Alloc>::operator =(const Bitset &a) noexcept -> Bitset & {
	auto old_n = Size, new_n = a.Size;
	if (new_n != old_n) {
		Alloc::dealloc_array(Start, (old_n + NUMBER_OF_BLOCK_TYPE_BITS - 1) / NUMBER_OF_BLOCK_TYPE_BITS);
		if (new_n > 0) {
			auto number_of_blocks = (new_n + NUMBER_OF_BLOCK_TYPE_BITS - 1) / NUMBER_OF_BLOCK_TYPE_BITS;
			auto *new_start = Alloc::template alloc_array<block_type>(number_of_blocks);
			Start = new_start;
			Size = new_n;
			memcpy(new_start, a.Start, ((new_n + NUMBER_OF_BLOCK_TYPE_BITS - 1) / NUMBER_OF_BLOCK_TYPE_BITS) * sizeof(block_type));
		} else {
			Start = nullptr, Size = 0;
		}
	} else {
		memcpy(Start, a.Start, ((new_n + NUMBER_OF_BLOCK_TYPE_BITS - 1) / NUMBER_OF_BLOCK_TYPE_BITS) * sizeof(block_type));
	}
	return *this;
}

}
