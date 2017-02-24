//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "StringTo.h"

namespace ej {

StringToReturnType<int32_t> c_string_to_int32(const char *s) noexcept {
	uint32_t value;
	unsigned char ch;

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '1') {
		if (ch <= '9') {
			value = ch - '0';
			goto state_digits;
		}
	} else if (ch == '0') {
		goto state_zero;
	} else if (ch == '-') {
		goto state_neg;
	}
	return { 0, false };

state_digits:
	for (unsigned i = 0; i < 8; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		if (ch >= '0') {
			if (ch <= '9') {
				value = value * 10 + (ch - '0');
			} else {
				return { 0, false };
			}
		} else if (ch == '\0') {
			return { static_cast<int32_t>(value), true };
		} else {
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '0') {
		if (ch <= '9') {
			if (value < 214748364 || value == 214748364 && ch <= '7') {
				value = value * 10 + (ch - '0');
				ch = static_cast<unsigned char>(*s);
				if (ch == '\0') {
					return { static_cast<int32_t>(value), true };
				}
			}
		}
	} else if (ch == '\0') {
		return { static_cast<int32_t>(value), true };
	}
	return { 0, false };

state_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case 'x':
		goto state_hex_skip_first_zero;

	default:
		return { 0, false };
	}

state_hex_skip_first_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '0':
		goto state_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_hex_digits;

	default:
		return { 0, false };
	}

state_hex_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case '0':
		goto state_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_hex_digits;

	default:
		return { 0, false };
	}

state_hex_digits:
	for (unsigned i = 0; i < 7; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		switch (ch) {
		case '\0':
			return { static_cast<int32_t>(value), true };

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			value = value * 16 + (ch - '0');
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			value = value * 16 + (ch - 'A' + 10);
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			value = value * 16 + (ch - 'a' + 10);
			break;

		default:
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	if (ch == '\0') {
		if (value < (1u << 31)) {
			return { static_cast<int32_t>(value), true };
		}
	}
	return { 0, false };

state_neg:
	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '1') {
		if (ch <= '9') {
			value = ch - '0';
			goto state_neg_digits;
		}
	} else if (ch == '0') {
		goto state_neg_zero;
	}
	return { 0, false };

state_neg_digits:
	for (unsigned i = 0; i < 8; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		if (ch >= '0') {
			if (ch <= '9') {
				value = value * 10 + (ch - '0');
			} else {
				return { 0, false };
			}
		} else if (ch == '\0') {
			return { -static_cast<int32_t>(value), true };
		} else {
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '0') {
		if (ch <= '9') {
			if (value < 214748364 || value == 214748364 && ch <= '8') {
				value = value * 10 + (ch - '0');
				ch = static_cast<unsigned char>(*s);
				if (ch == '\0') {
					return { -static_cast<int32_t>(value), true };
				}
			}
		}
	} else if (ch == '\0') {
		return { -static_cast<int32_t>(value), true };
	}
	return { 0, false };

state_neg_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case 'x':
		goto state_neg_hex_skip_first_zero;

	default:
		return { 0, false };
	}

state_neg_hex_skip_first_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '0':
		goto state_neg_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_neg_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_neg_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_neg_hex_digits;

	default:
		return { 0, false };
	}

state_neg_hex_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case '0':
		goto state_neg_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_neg_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_neg_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_neg_hex_digits;

	default:
		return { 0, false };
	}

state_neg_hex_digits:
	for (unsigned i = 0; i < 7; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		switch (ch) {
		case '\0':
			return { -static_cast<int32_t>(value), true };

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			value = value * 16 + (ch - '0');
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			value = value * 16 + (ch - 'A' + 10);
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			value = value * 16 + (ch - 'a' + 10);
			break;

		default:
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	if (ch == '\0') {
		if (value <= (1u << 31)) {
			return { -static_cast<int32_t>(value), true };
		}
	}
	return { 0, false };
}

StringToReturnType<int64_t> c_string_to_int64(const char *s) noexcept {
	uint64_t value;
	unsigned char ch;

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '1') {
		if (ch <= '9') {
			value = ch - '0';
			goto state_digits;
		}
	} else if (ch == '0') {
		goto state_zero;
	} else if (ch == '-') {
		goto state_neg;
	}
	return { 0, false };

state_digits:
	for (unsigned i = 0; i < 17; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		if (ch >= '0') {
			if (ch <= '9') {
				value = value * 10 + (ch - '0');
			} else {
				return { 0, false };
			}
		} else if (ch == '\0') {
			return { static_cast<int64_t>(value), true };
		} else {
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '0') {
		if (ch <= '9') {
			if (value < UINT64_C(922337203685477580) || value == UINT64_C(922337203685477580) && ch <= '7') {
				value = value * 10 + (ch - '0');
				ch = static_cast<unsigned char>(*s);
				if (ch == '\0') {
					return { static_cast<int64_t>(value), true };
				}
			}
		}
	} else if (ch == '\0') {
		return { static_cast<int64_t>(value), true };
	}
	return { 0, false };

state_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case 'x':
		goto state_hex_skip_first_zero;

	default:
		return { 0, false };
	}

state_hex_skip_first_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '0':
		goto state_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_hex_digits;

	default:
		return { 0, false };
	}

state_hex_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case '0':
		goto state_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_hex_digits;

	default:
		return { 0, false };
	}

state_hex_digits:
	for (unsigned i = 0; i < 15; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		switch (ch) {
		case '\0':
			return { static_cast<int64_t>(value), true };

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			value = value * 16 + (ch - '0');
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			value = value * 16 + (ch - 'A' + 10);
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			value = value * 16 + (ch - 'a' + 10);
			break;

		default:
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	if (ch == '\0') {
		if (value < (static_cast<uint64_t>(1) << 63)) {
			return { static_cast<int64_t>(value), true };
		}
	}
	return { 0, false };

state_neg:
	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '1') {
		if (ch <= '9') {
			value = ch - '0';
			goto state_neg_digits;
		}
	} else if (ch == '0') {
		goto state_neg_zero;
	}
	return { 0, false };

state_neg_digits:
	for (unsigned i = 0; i < 17; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		if (ch >= '0') {
			if (ch <= '9') {
				value = value * 10 + (ch - '0');
			} else {
				return { 0, false };
			}
		} else if (ch == '\0') {
			return { -static_cast<int64_t>(value), true };
		} else {
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '0') {
		if (ch <= '9') {
			if (value < UINT64_C(922337203685477580) || value == UINT64_C(922337203685477580) && ch <= '8') {
				value = value * 10 + (ch - '0');
				ch = static_cast<unsigned char>(*s);
				if (ch == '\0') {
					return { -static_cast<int64_t>(value), true };
				}
			}
		}
	} else if (ch == '\0') {
		return { -static_cast<int64_t>(value), true };
	}
	return { 0, false };

state_neg_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case 'x':
		goto state_neg_hex_skip_first_zero;

	default:
		return { 0, false };
	}

state_neg_hex_skip_first_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '0':
		goto state_neg_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_neg_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_neg_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_neg_hex_digits;

	default:
		return { 0, false };
	}

state_neg_hex_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case '0':
		goto state_neg_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_neg_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_neg_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_neg_hex_digits;

	default:
		return { 0, false };
	}

state_neg_hex_digits:
	for (unsigned i = 0; i < 15; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		switch (ch) {
		case '\0':
			return { -static_cast<int64_t>(value), true };

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			value = value * 16 + (ch - '0');
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			value = value * 16 + (ch - 'A' + 10);
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			value = value * 16 + (ch - 'a' + 10);
			break;

		default:
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	if (ch == '\0') {
		if (value <= (static_cast<uint64_t>(1) << 63)) {
			return { -static_cast<int64_t>(value), true };
		}
	}
	return { 0, false };
}

StringToReturnType<uint32_t> c_string_to_uint32(const char *s) noexcept {
	uint32_t value;
	unsigned char ch;

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '1') {
		if (ch <= '9') {
			value = ch - '0';
			goto state_digits;
		}
	} else if (ch == '0') {
		goto state_zero;
	}
	return { 0, false };

state_digits:
	for (unsigned i = 0; i < 8; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		if (ch >= '0') {
			if (ch <= '9') {
				value = value * 10 + (ch - '0');
			} else {
				return { 0, false };
			}
		} else if (ch == '\0') {
			return { value, true };
		} else {
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '0') {
		if (ch <= '9') {
			if (value < 429496729 || value == 429496729 && ch <= '5') {
				value = value * 10 + (ch - '0');
				ch = static_cast<unsigned char>(*s);
				if (ch == '\0') {
					return { value, true };
				}
			}
		}
	} else if (ch == '\0') {
		return { value, true };
	}
	return { 0, false };

state_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case 'x':
		goto state_hex_skip_first_zero;

	default:
		return { 0, false };
	}

state_hex_skip_first_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '0':
		goto state_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_hex_digits;

	default:
		return { 0, false };
	}

state_hex_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case '0':
		goto state_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_hex_digits;

	default:
		return { 0, false };
	}

state_hex_digits:
	for (unsigned i = 0; i < 7; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		switch (ch) {
		case '\0':
			return { value, true };

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			value = value * 16 + (ch - '0');
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			value = value * 16 + (ch - 'A' + 10);
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			value = value * 16 + (ch - 'a' + 10);
			break;

		default:
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	if (ch == '\0') {
		return { value, true };
	} else {
		return { 0, false };
	}
}

StringToReturnType<uint64_t> c_string_to_uint64(const char *s) noexcept {
	uint64_t value;
	unsigned char ch;

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '1') {
		if (ch <= '9') {
			value = ch - '0';
			goto state_digits;
		}
	} else if (ch == '0') {
		goto state_zero;
	}
	return { 0, false };

state_digits:
	for (unsigned i = 0; i < 18; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		if (ch >= '0') {
			if (ch <= '9') {
				value = value * 10 + (ch - '0');
			} else {
				return { 0, false };
			}
		} else if (ch == '\0') {
			return { value, true };
		} else {
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	s++;
	if (ch >= '0') {
		if (ch <= '9') {
			if (value < UINT64_C(1844674407370955161) || value == UINT64_C(1844674407370955161) && ch <= '5') {
				value = value * 10 + (ch - '0');
				ch = static_cast<unsigned char>(*s);
				if (ch == '\0') {
					return { value, true };
				}
			}
		}
	} else if (ch == '\0') {
		return { value, true };
	}
	return { 0, false };

state_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case 'x':
		goto state_hex_skip_first_zero;

	default:
		return { 0, false };
	}

state_hex_skip_first_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '0':
		goto state_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_hex_digits;

	default:
		return { 0, false };
	}

state_hex_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '\0':
		return { 0, true };

	case '0':
		goto state_hex_skip_zeros;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		value = ch - '0';
		goto state_hex_digits;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		value = ch - 'A' + 10;
		goto state_hex_digits;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		value = ch - 'a' + 10;
		goto state_hex_digits;

	default:
		return { 0, false };
	}

state_hex_digits:
	for (unsigned i = 0; i < 15; i++) {
		ch = static_cast<unsigned char>(*s);
		s++;
		switch (ch) {
		case '\0':
			return { value, true };

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			value = value * 16 + (ch - '0');
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			value = value * 16 + (ch - 'A' + 10);
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			value = value * 16 + (ch - 'a' + 10);
			break;

		default:
			return { 0, false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	if (ch == '\0') {
		return { value, true };
	} else {
		return { 0, false };
	}
}

}
