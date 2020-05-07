//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "djb2.h"

namespace ej {

enum : uint32_t {
	DJB2_OFFSET_BASIS = UINT32_C(5381),
	DJB2_MULTIPLIER = UINT32_C(33),
};

uint32_t djb2_32(const char *s) noexcept {
	uint32_t hash;
	unsigned char ch;

	for (hash = DJB2_OFFSET_BASIS; (ch = static_cast<unsigned char>(*s)) != '\0'; ++s) {
		hash = (hash * DJB2_MULTIPLIER) + ch;
	}
	return hash;
}

uint64_t djb2_64(const char *s) noexcept {
	uint64_t hash;
	unsigned char ch;

	for (hash = DJB2_OFFSET_BASIS; (ch = static_cast<unsigned char>(*s)) != '\0'; ++s) {
		hash = (hash * DJB2_MULTIPLIER) + ch;
	}
	return hash;
}

enum : uint32_t {
	DJB2_32_OFFSET_BASIS = UINT32_C(5381),
	DJB2_32_MULTIPLIER_P1 = UINT32_C(33),
	DJB2_32_MULTIPLIER_P2 = UINT32_C(1089),
	DJB2_32_MULTIPLIER_P3 = UINT32_C(35937),
	DJB2_32_MULTIPLIER_P4 = UINT32_C(1185921),
	DJB2_32_MULTIPLIER_P5 = UINT32_C(39135393),
	DJB2_32_MULTIPLIER_P6 = UINT32_C(1291467969),
	DJB2_32_MULTIPLIER_P7 = UINT32_C(3963737313),
	DJB2_32_MULTIPLIER_P8 = UINT32_C(1954312449),
};

uint32_t djb2_32(const void *s, size_t n) noexcept {
	const void *start;
	const void *end;
	size_t aligned_n;
	uint32_t hash;

	end = s;
	hash = DJB2_32_OFFSET_BASIS;
	aligned_n = (n / 8) * 8;
	if (aligned_n > 0) {
		start = s;
		end = lea(end, aligned_n);
		do {
			hash = hash * DJB2_32_MULTIPLIER_P8 +
				load<uint8_t>(start, 0) * DJB2_32_MULTIPLIER_P7 +
				load<uint8_t>(start, 1) * DJB2_32_MULTIPLIER_P6 +
				load<uint8_t>(start, 2) * DJB2_32_MULTIPLIER_P5 +
				load<uint8_t>(start, 3) * DJB2_32_MULTIPLIER_P4 +
				load<uint8_t>(start, 4) * DJB2_32_MULTIPLIER_P3 +
				load<uint8_t>(start, 5) * DJB2_32_MULTIPLIER_P2 +
				load<uint8_t>(start, 6) * DJB2_32_MULTIPLIER_P1 +
				load<uint8_t>(start, 7);

			start = lea(start, 8);
		} while (start < end);
	}

	switch (n & 7) {
	case 7:
		hash = hash * DJB2_32_MULTIPLIER_P7 +
			load<uint8_t>(end, 0) * DJB2_32_MULTIPLIER_P6 +
			load<uint8_t>(end, 1) * DJB2_32_MULTIPLIER_P5 +
			load<uint8_t>(end, 2) * DJB2_32_MULTIPLIER_P4 +
			load<uint8_t>(end, 3) * DJB2_32_MULTIPLIER_P3 +
			load<uint8_t>(end, 4) * DJB2_32_MULTIPLIER_P2 +
			load<uint8_t>(end, 5) * DJB2_32_MULTIPLIER_P1 +
			load<uint8_t>(end, 6);
		break;

	case 6:
		hash = hash * DJB2_32_MULTIPLIER_P6 +
			load<uint8_t>(end, 0) * DJB2_32_MULTIPLIER_P5 +
			load<uint8_t>(end, 1) * DJB2_32_MULTIPLIER_P4 +
			load<uint8_t>(end, 2) * DJB2_32_MULTIPLIER_P3 +
			load<uint8_t>(end, 3) * DJB2_32_MULTIPLIER_P2 +
			load<uint8_t>(end, 4) * DJB2_32_MULTIPLIER_P1 +
			load<uint8_t>(end, 5);
		break;

	case 5:
		hash = hash * DJB2_32_MULTIPLIER_P5 +
			load<uint8_t>(end, 0) * DJB2_32_MULTIPLIER_P4 +
			load<uint8_t>(end, 1) * DJB2_32_MULTIPLIER_P3 +
			load<uint8_t>(end, 2) * DJB2_32_MULTIPLIER_P2 +
			load<uint8_t>(end, 3) * DJB2_32_MULTIPLIER_P1 +
			load<uint8_t>(end, 4);
		break;

	case 4:
		hash = hash * DJB2_32_MULTIPLIER_P4 +
			load<uint8_t>(end, 0) * DJB2_32_MULTIPLIER_P3 +
			load<uint8_t>(end, 1) * DJB2_32_MULTIPLIER_P2 +
			load<uint8_t>(end, 2) * DJB2_32_MULTIPLIER_P1 +
			load<uint8_t>(end, 3);
		break;

	case 3:
		hash = hash * DJB2_32_MULTIPLIER_P3 +
			load<uint8_t>(end, 0) * DJB2_32_MULTIPLIER_P2 +
			load<uint8_t>(end, 1) * DJB2_32_MULTIPLIER_P1 +
			load<uint8_t>(end, 2);
		break;

	case 2:
		hash = hash * DJB2_32_MULTIPLIER_P2 +
			load<uint8_t>(end, 0) * DJB2_32_MULTIPLIER_P1 +
			load<uint8_t>(end, 1);
		break;

	case 1:
		hash = hash * DJB2_32_MULTIPLIER_P1 +
			load<uint8_t>(end, 0);
		break;
	}
	return hash;
}

uint32_t djb2_32(uint32_t value) noexcept {
	uint32_t hash;

	hash = DJB2_32_OFFSET_BASIS;
	hash = hash * DJB2_32_MULTIPLIER_P4 +
		static_cast<uint8_t>(value) * DJB2_32_MULTIPLIER_P3 +
		static_cast<uint8_t>(value >> 8) * DJB2_32_MULTIPLIER_P2 +
		static_cast<uint8_t>(value >> 16) * DJB2_32_MULTIPLIER_P1 +
		static_cast<uint8_t>(value >> 24);

	return hash;
}

uint32_t djb2_32(uint64_t value) noexcept {
	uint32_t hash;

	hash = DJB2_32_OFFSET_BASIS;
	hash = hash * DJB2_32_MULTIPLIER_P8 +
		static_cast<uint8_t>(value) * DJB2_32_MULTIPLIER_P7 +
		static_cast<uint8_t>(value >> 8) * DJB2_32_MULTIPLIER_P6 +
		static_cast<uint8_t>(value >> 16) * DJB2_32_MULTIPLIER_P5 +
		static_cast<uint8_t>(value >> 24) * DJB2_32_MULTIPLIER_P4 +
		static_cast<uint8_t>(value >> 32) * DJB2_32_MULTIPLIER_P3 +
		static_cast<uint8_t>(value >> 40) * DJB2_32_MULTIPLIER_P2 +
		static_cast<uint8_t>(value >> 48) * DJB2_32_MULTIPLIER_P1 +
		static_cast<uint8_t>(value >> 56);

	return hash;
}

enum : uint64_t {
	DJB2_64_OFFSET_BASIS = UINT64_C(5381),
	DJB2_64_MULTIPLIER_P1 = UINT64_C(33),
	DJB2_64_MULTIPLIER_P2 = UINT64_C(1089),
	DJB2_64_MULTIPLIER_P3 = UINT64_C(35937),
	DJB2_64_MULTIPLIER_P4 = UINT64_C(1185921),
	DJB2_64_MULTIPLIER_P5 = UINT64_C(39135393),
	DJB2_64_MULTIPLIER_P6 = UINT64_C(1291467969),
	DJB2_64_MULTIPLIER_P7 = UINT64_C(42618442977),
	DJB2_64_MULTIPLIER_P8 = UINT64_C(1406408618241),
};

uint64_t djb2_64(const void *s, size_t n) noexcept {
	const void *start;
	const void *end;
	size_t aligned_n;
	uint64_t hash;

	end = s;
	hash = DJB2_64_OFFSET_BASIS;
	aligned_n = (n / 8) * 8;
	if (aligned_n > 0) {
		start = s;
		end = lea(end, aligned_n);
		do {
			hash = hash * DJB2_64_MULTIPLIER_P8 +
				load<uint8_t>(start, 0) * DJB2_64_MULTIPLIER_P7 +
				load<uint8_t>(start, 1) * DJB2_64_MULTIPLIER_P6 +
				load<uint8_t>(start, 2) * DJB2_64_MULTIPLIER_P5 +
				load<uint8_t>(start, 3) * DJB2_64_MULTIPLIER_P4 +
				load<uint8_t>(start, 4) * DJB2_64_MULTIPLIER_P3 +
				load<uint8_t>(start, 5) * DJB2_64_MULTIPLIER_P2 +
				load<uint8_t>(start, 6) * DJB2_64_MULTIPLIER_P1 +
				load<uint8_t>(start, 7);

			start = lea(start, 8);
		} while (start < end);
	}

	switch (n & 7) {
	case 7:
		hash = hash * DJB2_64_MULTIPLIER_P7 +
			load<uint8_t>(end, 0) * DJB2_64_MULTIPLIER_P6 +
			load<uint8_t>(end, 1) * DJB2_64_MULTIPLIER_P5 +
			load<uint8_t>(end, 2) * DJB2_64_MULTIPLIER_P4 +
			load<uint8_t>(end, 3) * DJB2_64_MULTIPLIER_P3 +
			load<uint8_t>(end, 4) * DJB2_64_MULTIPLIER_P2 +
			load<uint8_t>(end, 5) * DJB2_64_MULTIPLIER_P1 +
			load<uint8_t>(end, 6);
		break;

	case 6:
		hash = hash * DJB2_64_MULTIPLIER_P6 +
			load<uint8_t>(end, 0) * DJB2_64_MULTIPLIER_P5 +
			load<uint8_t>(end, 1) * DJB2_64_MULTIPLIER_P4 +
			load<uint8_t>(end, 2) * DJB2_64_MULTIPLIER_P3 +
			load<uint8_t>(end, 3) * DJB2_64_MULTIPLIER_P2 +
			load<uint8_t>(end, 4) * DJB2_64_MULTIPLIER_P1 +
			load<uint8_t>(end, 5);
		break;

	case 5:
		hash = hash * DJB2_64_MULTIPLIER_P5 +
			load<uint8_t>(end, 0) * DJB2_64_MULTIPLIER_P4 +
			load<uint8_t>(end, 1) * DJB2_64_MULTIPLIER_P3 +
			load<uint8_t>(end, 2) * DJB2_64_MULTIPLIER_P2 +
			load<uint8_t>(end, 3) * DJB2_64_MULTIPLIER_P1 +
			load<uint8_t>(end, 4);
		break;

	case 4:
		hash = hash * DJB2_64_MULTIPLIER_P4 +
			load<uint8_t>(end, 0) * DJB2_64_MULTIPLIER_P3 +
			load<uint8_t>(end, 1) * DJB2_64_MULTIPLIER_P2 +
			load<uint8_t>(end, 2) * DJB2_64_MULTIPLIER_P1 +
			load<uint8_t>(end, 3);
		break;

	case 3:
		hash = hash * DJB2_64_MULTIPLIER_P3 +
			load<uint8_t>(end, 0) * DJB2_64_MULTIPLIER_P2 +
			load<uint8_t>(end, 1) * DJB2_64_MULTIPLIER_P1 +
			load<uint8_t>(end, 2);
		break;

	case 2:
		hash = hash * DJB2_64_MULTIPLIER_P2 +
			load<uint8_t>(end, 0) * DJB2_64_MULTIPLIER_P1 +
			load<uint8_t>(end, 1);
		break;

	case 1:
		hash = hash * DJB2_64_MULTIPLIER_P1 +
			load<uint8_t>(end, 0);
		break;
	}
	return hash;
}

uint64_t djb2_64(uint32_t value) noexcept {
	uint64_t hash;

	hash = DJB2_64_OFFSET_BASIS;
	hash = hash * DJB2_64_MULTIPLIER_P4 +
		static_cast<uint8_t>(value) * DJB2_64_MULTIPLIER_P3 +
		static_cast<uint8_t>(value >> 8) * DJB2_64_MULTIPLIER_P2 +
		static_cast<uint8_t>(value >> 16) * DJB2_64_MULTIPLIER_P1 +
		static_cast<uint8_t>(value >> 24);

	return hash;
}

uint64_t djb2_64(uint64_t value) noexcept {
	uint64_t hash;

	hash = DJB2_64_OFFSET_BASIS;
	hash = hash * DJB2_64_MULTIPLIER_P8 +
		static_cast<uint8_t>(value) * DJB2_64_MULTIPLIER_P7 +
		static_cast<uint8_t>(value >> 8) * DJB2_64_MULTIPLIER_P6 +
		static_cast<uint8_t>(value >> 16) * DJB2_64_MULTIPLIER_P5 +
		static_cast<uint8_t>(value >> 24) * DJB2_64_MULTIPLIER_P4 +
		static_cast<uint8_t>(value >> 32) * DJB2_64_MULTIPLIER_P3 +
		static_cast<uint8_t>(value >> 40) * DJB2_64_MULTIPLIER_P2 +
		static_cast<uint8_t>(value >> 48) * DJB2_64_MULTIPLIER_P1 +
		static_cast<uint8_t>(value >> 56);

	return hash;
}

}
