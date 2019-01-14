//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_STRING_TO_H
#define EJ_STRING_TO_H

#include <stdint.h>

namespace ej {

template <typename T>
struct StringToReturnType {
	T Value;
	bool Status;
};

StringToReturnType<int32_t> c_string_to_int32(const char *s) noexcept;
StringToReturnType<int64_t> c_string_to_int64(const char *s) noexcept;
StringToReturnType<uint32_t> c_string_to_uint32(const char *s) noexcept;
StringToReturnType<uint64_t> c_string_to_uint64(const char *s) noexcept;
float decimal_to_float(uint32_t significand, int exponent, bool negative) noexcept;
StringToReturnType<float> c_string_to_float(const char *s) noexcept;
double decimal_to_double(uint64_t significand, int exponent, bool negative) noexcept;
StringToReturnType<double> c_string_to_double(const char *s) noexcept;

constexpr unsigned UINT32_TO_STRING_BUFFER_SIZE = 16;
unsigned uint32_find_number_of_digits(uint32_t value) noexcept;
char *uint32_to_string_no_nul(char *s, uint32_t value) noexcept;
inline char *uint32_to_c_string(char *s, uint32_t value) noexcept {
	auto *e = uint32_to_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

constexpr unsigned INT32_TO_STRING_BUFFER_SIZE = 16;
inline char *int32_to_string_no_nul(char *s, int32_t value) noexcept {
	auto u = static_cast<uint32_t>(value);
	if (value < 0) {
		s[0] = '-';
		s++;
		u = static_cast<uint32_t>(-value);
	}
	return uint32_to_string_no_nul(s, u);
}
inline char *int32_to_c_string(char *s, int32_t value) {
	auto *e = int32_to_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

constexpr unsigned UINT64_TO_STRING_BUFFER_SIZE = 32;
unsigned uint64_find_number_of_digits(uint64_t value) noexcept;
char *uint64_to_string_no_nul(char *s, uint64_t value) noexcept;
inline char *uint64_to_c_string(char *s, uint64_t value) noexcept {
	auto *e = uint64_to_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

template <typename T>
unsigned uint_find_number_of_digits(T value) noexcept;

template <>
inline unsigned uint_find_number_of_digits<uint32_t>(uint32_t value) noexcept {
	return uint32_find_number_of_digits(value);
}

template <>
inline unsigned uint_find_number_of_digits<uint64_t>(uint64_t value) noexcept {
	return uint64_find_number_of_digits(value);
}

constexpr unsigned INT64_TO_STRING_BUFFER_SIZE = 32;
inline char *int64_to_string_no_nul(char *s, int64_t value) noexcept {
	auto u = static_cast<uint64_t>(value);
	if (value < 0) {
		s[0] = '-';
		s++;
		u = static_cast<uint64_t>(-value);
	}
	return uint64_to_string_no_nul(s, u);
}
inline char *int64_to_c_string(char *s, int64_t value) {
	auto *e = int64_to_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

constexpr unsigned UINT32_TO_HEX_STRING_BUFFER_SIZE = 8;
char *uint32_to_hex_string_no_nul(char *s, uint32_t value) noexcept;
inline char *uint32_to_hex_c_string(char *s, uint32_t value) {
	auto *e = uint32_to_hex_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

constexpr unsigned UINT64_TO_HEX_STRING_BUFFER_SIZE = 16;
char *uint64_to_hex_string_no_nul(char *s, uint64_t value) noexcept;
inline char *uint64_to_hex_c_string(char *s, uint64_t value) {
	auto *e = uint64_to_hex_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

char *uint32_to_uhex_string_no_nul(char *s, uint32_t value) noexcept;
inline char *uint32_to_uhex_c_string(char *s, uint32_t value) {
	auto *e = uint32_to_uhex_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

char *uint64_to_uhex_string_no_nul(char *s, uint64_t value) noexcept;
inline char *uint64_to_uhex_c_string(char *s, uint64_t value) {
	auto *e = uint64_to_uhex_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

constexpr unsigned FLOAT_TO_STRING_BUFFER_SIZE = 16;
char *float_to_string_no_nul(char *s, float value) noexcept;
inline char *float_to_c_string(char *s, float value) noexcept {
	auto *e = float_to_string_no_nul(s, value);
	e[0] = '\0';
	e++;
	return e;
}

}

#endif
