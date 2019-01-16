//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

#include "CAlloc.h"
#include "Hex.h"

namespace ej {

template <class Alloc = CAlloc>
class StringStream {
public:
	typedef char value_type;
	typedef size_t size_type;

	static_assert(sizeof(value_type) == 1);

private:
	value_type *Start;
	value_type *Finish;
	size_type FreeSize;

	value_type *resize_and_allocate(size_type n) noexcept {
		auto *start = Start;
		auto *finish = Finish;
		size_type cur_size = finish - start;
		size_type new_size;

		if (!__builtin_add_overflow(cur_size, n, &new_size)) {
			size_type free_size = FreeSize;
			size_type cur_capacity = cur_size + free_size;
			size_type new_capacity = new_size;
			if (cur_capacity <= SIZE_MAX / 2) {
				new_capacity = cur_capacity * 2;
			}

			start = Alloc::realloc_array(start, cur_capacity, new_capacity);
			Start = start;
			Finish = start + new_size;
			FreeSize = new_capacity - new_size;
			return start + cur_size;
		}
		abort();
	}

	value_type *allocate(size_type n) noexcept {
		size_type free_size = FreeSize;
		if (EJ_LIKELY(n <= free_size)) {
			auto *new_block = Finish;
			Finish = new_block + n;
			FreeSize -= n;
			return new_block;
		} else {
			return resize_and_allocate(n);
		}
	}

	void write(char c) noexcept {
		auto *buffer = allocate(sizeof(c));
		*buffer = c;
	}
	void write(signed char c) noexcept {
		write(static_cast<char>(c));
	}
	void write(unsigned char c) noexcept {
		write(static_cast<char>(c));
	}
	void write(int32_t value) noexcept {
		auto *buffer = allocate(INT32_TO_STRING_BUFFER_SIZE);
		auto *new_finish = int32_to_string_no_nul(buffer, value);
		Finish = new_finish;
		FreeSize += INT32_TO_STRING_BUFFER_SIZE - static_cast<unsigned>(new_finish - buffer);
	}
	void write(uint32_t value) noexcept {
		auto *buffer = allocate(UINT32_TO_STRING_BUFFER_SIZE);
		auto *new_finish = uint32_to_string_no_nul(buffer, value);
		Finish = new_finish;
		FreeSize += INT32_TO_STRING_BUFFER_SIZE - static_cast<unsigned>(new_finish - buffer);
	}
	void write(int64_t value) noexcept {
		auto *buffer = allocate(INT64_TO_STRING_BUFFER_SIZE);
		auto *new_finish = int64_to_string_no_nul(buffer, value);
		Finish = new_finish;
		FreeSize += INT64_TO_STRING_BUFFER_SIZE - static_cast<unsigned>(new_finish - buffer);
	}
	void write(uint64_t value) noexcept {
		auto *buffer = allocate(UINT64_TO_STRING_BUFFER_SIZE);
		auto *new_finish = uint64_to_string_no_nul(buffer, value);
		Finish = new_finish;
		FreeSize += INT64_TO_STRING_BUFFER_SIZE - static_cast<unsigned>(new_finish - buffer);
	}
	void write(float value) noexcept {
		auto *buffer = allocate(FLOAT_TO_STRING_BUFFER_SIZE);
		auto *new_finish = float_to_string_no_nul(buffer, value);
		Finish = new_finish;
		FreeSize += FLOAT_TO_STRING_BUFFER_SIZE - static_cast<unsigned>(new_finish - buffer);
	}
	void write(Hex<uint32_t> value) noexcept {
		auto *buffer = allocate(UINT32_TO_HEX_STRING_BUFFER_SIZE);
		auto *new_finish = uint32_to_hex_string_no_nul(buffer, value.Value);
		Finish = new_finish;
		FreeSize += UINT32_TO_HEX_STRING_BUFFER_SIZE - static_cast<unsigned>(new_finish - buffer);
	}
	void write(Hex<uint64_t> value) noexcept {
		auto *buffer = allocate(UINT64_TO_HEX_STRING_BUFFER_SIZE);
		auto *new_finish = uint64_to_hex_string_no_nul(buffer, value.Value);
		Finish = new_finish;
		FreeSize += UINT64_TO_HEX_STRING_BUFFER_SIZE - static_cast<unsigned>(new_finish - buffer);
	}

	void write(const void *s, size_t n) noexcept {
		auto *buffer = allocate(n);
		memcpy(buffer, s, n);
	}
	void write(const char *s) noexcept {
		write(s, strlen(s));
	}
	template <typename T, typename S = size_t>
	void write(const StringView<T, S> &s) noexcept {
		write(s.data(), s.size());
	}

	template <typename T, typename ... A>
	EJ_ALWAYS_INLINE void print_nl(const T &v, A &&... args) noexcept {
		write(v);
		print_nl(std::forward<A>(args) ...);
	}
	template <typename T>
	EJ_ALWAYS_INLINE void print_nl(const T &v) noexcept {
		write(v);
	}

public:
	explicit StringStream(size_type n = Alloc::min_size() < 256 ? 256 : Alloc::min_size()) noexcept {
		if (n < Alloc::min_size()) {
			n = Alloc::min_size();
		}
		auto *new_start = Alloc::template alloc_array<value_type>(n);
		Start = new_start;
		Finish = new_start;
		FreeSize = n;
	}
	~StringStream() noexcept {
		Alloc::dealloc_array(Start, static_cast<size_t>(Finish - Start) + FreeSize);
	}

	constexpr auto view() const noexcept {
		return ej::StringView<const value_type, void>(Start, Finish);
	}

	void clear() noexcept {
		auto *start = Start;
		auto *finish = Finish;
		size_type n = finish - start;
		Finish = start;
		FreeSize += n;
	}

	template <typename ... A>
	EJ_ALWAYS_INLINE void print(A &&... args) noexcept {
		print_nl(std::forward<A>(args) ...);
	}

	template <typename ... A>
	EJ_ALWAYS_INLINE void println(A &&... args) noexcept {
		return print(std::forward<A>(args) ..., '\n');
	}
};

}
