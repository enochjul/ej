//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_OUTPUT_STREAM_H
#define EJ_OUTPUT_STREAM_H

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <utility>

#include "CAlloc.h"
#include "Common.h"
#include "Construct.h"
#include "File.h"
#include "NoMutex.h"
#include "StatusCode.h"
#include "StringTo.h"

namespace ej {

template <typename T>
struct Hex {
	T Value;
};

template <
	//! Size of the buffer
	unsigned N = 65536,
	//! Type of mutex to use to synchronize access between different threads
	class MutexType = NoMutex,
	//! Allocator to use
	class Alloc = CAlloc>
class OutputStream : public MutexType {
	typedef OutputStream<N, MutexType, Alloc> this_type;

	//Allocate a bit more than the requested buffer size so that the number conversion functions can assume that there is always enough space in the buffer for the conversion
	enum : size_t {
		BUFFER_OVERFLOW_SIZE = 32,
		BUFFER_ALLOC_SIZE = static_cast<size_t>(N) + BUFFER_OVERFLOW_SIZE
	};

	File::native_type Handle;
	unsigned Size;
	char Buffer[BUFFER_ALLOC_SIZE];

	constexpr explicit OutputStream(File::native_type handle) noexcept : Handle(handle), Size(0) {
	}

	//! Flush a full buffer
	StatusCode flush_on_full() noexcept;
	//! Flush a full buffer and copy from the overflow area to the beginning of the buffer
	StatusCode flush_on_full(unsigned n) noexcept;
	//! Copy from the given string to the internal buffer and flush the buffer and as much of the given string to the file
	StatusCode flush_on_full(const void *s, size_t n, unsigned free_size) noexcept;

	StatusCode write(char c) noexcept {
		auto *buffer = Buffer;
		auto n = Size;
		buffer[n] = c;
		n++;
		if (EJ_LIKELY(n < N)) {
			Size = n;
			return StatusCode();
		} else {
			return flush_on_full();
		}
	}
	StatusCode write(signed char c) noexcept {
		return write(static_cast<char>(c));
	}
	StatusCode write(unsigned char c) noexcept {
		return write(static_cast<char>(c));
	}
	StatusCode write(int16_t value) noexcept {
		return write(static_cast<int32_t>(value));
	}
	StatusCode write(uint16_t value) noexcept {
		return write(static_cast<uint32_t>(value));
	}
	StatusCode write(int32_t value) noexcept {
		auto *buffer = Buffer;
		auto n = Size;
		n = static_cast<unsigned>(int32_to_string_no_nul(buffer + n, value) - buffer);
		if (EJ_LIKELY(n < N)) {
			Size = n;
			return StatusCode();
		} else {
			return flush_on_full(n);
		}
	}
	StatusCode write(uint32_t value) noexcept {
		auto *buffer = Buffer;
		auto n = Size;
		n = static_cast<unsigned>(uint32_to_string_no_nul(buffer + n, value) - buffer);
		if (EJ_LIKELY(n < N)) {
			Size = n;
			return StatusCode();
		} else {
			return flush_on_full(n);
		}
	}
	StatusCode write(int64_t value) noexcept {
		auto *buffer = Buffer;
		auto n = Size;
		n = static_cast<unsigned>(int64_to_string_no_nul(buffer + n, value) - buffer);
		if (EJ_LIKELY(n < N)) {
			Size = n;
			return StatusCode();
		} else {
			return flush_on_full(n);
		}
	}
	StatusCode write(uint64_t value) noexcept {
		auto *buffer = Buffer;
		auto n = Size;
		n = static_cast<unsigned>(uint64_to_string_no_nul(buffer + n, value) - buffer);
		if (EJ_LIKELY(n < N)) {
			Size = n;
			return StatusCode();
		} else {
			return flush_on_full(n);
		}
	}
	StatusCode write(float value) noexcept {
		auto *buffer = Buffer;
		auto n = Size;
		n = static_cast<unsigned>(float_to_string_no_nul(buffer + n, value) - buffer);
		if (EJ_LIKELY(n < N)) {
			Size = n;
			return StatusCode();
		} else {
			return flush_on_full(n);
		}
	}
	StatusCode write(Hex<uint32_t> value) noexcept {
		auto *buffer = Buffer;
		auto n = Size;
		n = static_cast<unsigned>(uint32_to_hex_string_no_nul(buffer + n, value.Value) - buffer);
		if (EJ_LIKELY(n < N)) {
			Size = n;
			return StatusCode();
		} else {
			return flush_on_full(n);
		}
	}
	StatusCode write(Hex<uint64_t> value) noexcept {
		auto *buffer = Buffer;
		auto n = Size;
		n = static_cast<unsigned>(uint64_to_hex_string_no_nul(buffer + n, value.Value) - buffer);
		if (EJ_LIKELY(n < N)) {
			Size = n;
			return StatusCode();
		} else {
			return flush_on_full(n);
		}
	}

	StatusCode write(const void *s, size_t n) noexcept {
		auto *buffer = Buffer;
		auto size = Size;
		auto free_size = N - size;

		if (EJ_LIKELY(free_size > n)) {
			Size = size + n;
			memcpy(buffer + size, s, n);
			return StatusCode();
		} else {
			return flush_on_full(s, n, free_size);
		}
	}
	StatusCode write(const char *s) noexcept {
		return write(s, strlen(s));
	}

	template <typename T, typename ... A>
	EJ_ALWAYS_INLINE StatusCode print_nl(const T &v, A &&... args) noexcept {
		auto status_code = write(v);
		if (EJ_LIKELY(status_code.success())) {
			status_code = print_nl(std::forward<A>(args) ...);
		}
		return status_code;
	}
	template <typename T>
	EJ_ALWAYS_INLINE StatusCode print_nl(const T &v) noexcept {
		return write(v);
	}

public:
	StatusCode flush() noexcept;

	template <bool flush_after = false, typename ... A>
	EJ_ALWAYS_INLINE StatusCode print(A &&... args) noexcept {
		MutexType::lock();
		auto r = print_nl(std::forward<A>(args) ...);
		if constexpr (flush_after) {
			if (EJ_LIKELY(r.success())) {
				r = flush();
			}
		}
		MutexType::unlock();
		return r;
	}

	template <bool flush_after = true, typename ... A>
	EJ_ALWAYS_INLINE StatusCode println(A &&... args) noexcept {
		return print<flush_after>(std::forward<A>(args) ..., '\n');
	}

	template <typename F>
	EJ_ALWAYS_INLINE static StatusCode with(File::native_type handle, F func) noexcept {
		auto *out = Alloc::template try_alloc<this_type>();
		if (out != nullptr) {
			new(out, 0) this_type(handle);

			auto status_code = func(*out);
			if (EJ_LIKELY(status_code.success())) {
				auto n = out->Size;
				if (n > 0) {
					out->Size = 0;
					status_code = File::write(handle, out->Buffer, n);
				}
			}

			Alloc::dealloc(out);
			return status_code;
		} else {
			return StatusCode(ENOMEM);
		}
	}

	template <typename F>
	EJ_ALWAYS_INLINE static StatusCode with(File::native_type handle_out, File::native_type handle_err, F func) noexcept {
		auto *out = Alloc::template try_alloc_array<this_type>(2);
		if (out != nullptr) {
			new(out, 0) this_type(handle_out);
			auto *err = out + 1;
			new(err, 0) this_type(handle_err);

			auto status_code = func(*out, *err);
			if (EJ_LIKELY(status_code.success())) {
				auto n = out->Size;
				if (n > 0) {
					out->Size = 0;
					status_code = File::write(handle_out, out->Buffer, n);
				}
				n = err->Size;
				if (n > 0) {
					err->Size = 0;
					status_code = File::write(handle_err, err->Buffer, n);
				}
			}

			Alloc::dealloc_array(out, 2);
			return status_code;
		} else {
			return StatusCode(ENOMEM);
		}
	}
};

template <unsigned N, class MutexType, class Alloc>
StatusCode OutputStream<N, MutexType, Alloc>::flush_on_full() noexcept {
	Size = 0;
	return File::write(Handle, Buffer, N);
}

template <unsigned N, class MutexType, class Alloc>
StatusCode OutputStream<N, MutexType, Alloc>::flush_on_full(unsigned n) noexcept {
	auto remaining_size = n - N;
	Size = remaining_size;

	auto buffer = Buffer;
	auto status_code = File::write(Handle, buffer, N);
	memcpy(buffer, buffer + N, remaining_size);

	return status_code;
}

template <unsigned N, class MutexType, class Alloc>
StatusCode OutputStream<N, MutexType, Alloc>::flush_on_full(const void *s, size_t n, unsigned free_size) noexcept {
	auto handle = Handle;
	auto *buffer = Buffer;
	auto size = Size;

	//Copy from the supplied buffer to the internal buffer and flush to the output file
	if (size > 0) {
		memcpy(buffer + size, s, free_size);
		auto status_code = File::write(handle, buffer, N);
		if (EJ_UNLIKELY(status_code.failure())) {
			return status_code;
		}
		n -= free_size;
		s = static_cast<const char *>(s) + free_size;
		Size = 0;
	}

	//Write directly from the supplied buffer to the output file
	size_t write_size = (n / N) * N;
	if (write_size > 0) {
		auto status_code = File::lwrite(handle, s, write_size);
		if (EJ_UNLIKELY(status_code.failure())) {
			return status_code;
		}
		n -= write_size;
		s = static_cast<const char *>(s) + write_size;
	}

	Size = n;
	memcpy(buffer, s, n);
	return StatusCode();
}

template <unsigned N, class MutexType, class Alloc>
StatusCode OutputStream<N, MutexType, Alloc>::flush() noexcept {
	MutexType::lock();

	StatusCode status_code;
	auto n = Size;
	if (n > 0) {
		Size = 0;
		status_code = File::write(Handle, Buffer, n);
	}

	MutexType::unlock();
	return status_code;
}

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

#endif
