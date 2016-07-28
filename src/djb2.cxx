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
	const uint8_t *start;
	const uint8_t *end;
	size_t aligned_n;
	uint32_t hash;

	end = static_cast<const uint8_t *>(s);
	hash = DJB2_32_OFFSET_BASIS;
	aligned_n = (n / 8) * 8;
	if (aligned_n > 0) {
		start = static_cast<const uint8_t *>(s);
		end += aligned_n;
		do {
			hash = hash * DJB2_32_MULTIPLIER_P8 +
				start[0] * DJB2_32_MULTIPLIER_P7 +
				start[1] * DJB2_32_MULTIPLIER_P6 +
				start[2] * DJB2_32_MULTIPLIER_P5 +
				start[3] * DJB2_32_MULTIPLIER_P4 +
				start[4] * DJB2_32_MULTIPLIER_P3 +
				start[5] * DJB2_32_MULTIPLIER_P2 +
				start[6] * DJB2_32_MULTIPLIER_P1 +
				start[7];

			start += 8;
		} while (start < end);
	}

	switch (n & 7) {
	case 7:
		hash = hash * DJB2_32_MULTIPLIER_P7 +
			end[0] * DJB2_32_MULTIPLIER_P6 +
			end[1] * DJB2_32_MULTIPLIER_P5 +
			end[2] * DJB2_32_MULTIPLIER_P4 +
			end[3] * DJB2_32_MULTIPLIER_P3 +
			end[4] * DJB2_32_MULTIPLIER_P2 +
			end[5] * DJB2_32_MULTIPLIER_P1 +
			end[6];
		break;

	case 6:
		hash = hash * DJB2_32_MULTIPLIER_P6 +
			end[0] * DJB2_32_MULTIPLIER_P5 +
			end[1] * DJB2_32_MULTIPLIER_P4 +
			end[2] * DJB2_32_MULTIPLIER_P3 +
			end[3] * DJB2_32_MULTIPLIER_P2 +
			end[4] * DJB2_32_MULTIPLIER_P1 +
			end[5];
		break;

	case 5:
		hash = hash * DJB2_32_MULTIPLIER_P5 +
			end[0] * DJB2_32_MULTIPLIER_P4 +
			end[1] * DJB2_32_MULTIPLIER_P3 +
			end[2] * DJB2_32_MULTIPLIER_P2 +
			end[3] * DJB2_32_MULTIPLIER_P1 +
			end[4];
		break;

	case 4:
		hash = hash * DJB2_32_MULTIPLIER_P4 +
			end[0] * DJB2_32_MULTIPLIER_P3 +
			end[1] * DJB2_32_MULTIPLIER_P2 +
			end[2] * DJB2_32_MULTIPLIER_P1 +
			end[3];
		break;

	case 3:
		hash = hash * DJB2_32_MULTIPLIER_P3 +
			end[0] * DJB2_32_MULTIPLIER_P2 +
			end[1] * DJB2_32_MULTIPLIER_P1 +
			end[2];
		break;

	case 2:
		hash = hash * DJB2_32_MULTIPLIER_P2 +
			end[0] * DJB2_32_MULTIPLIER_P1 +
			end[1];
		break;

	case 1:
		hash = hash * DJB2_32_MULTIPLIER_P1 +
			end[0];
		break;
	}
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
	const uint8_t *start;
	const uint8_t *end;
	size_t aligned_n;
	uint64_t hash;

	end = static_cast<const uint8_t *>(s);
	hash = DJB2_64_OFFSET_BASIS;
	aligned_n = (n / 8) * 8;
	if (aligned_n > 0) {
		start = static_cast<const uint8_t *>(s);
		end += aligned_n;
		do {
			hash = hash * DJB2_64_MULTIPLIER_P8 +
				start[0] * DJB2_64_MULTIPLIER_P7 +
				start[1] * DJB2_64_MULTIPLIER_P6 +
				start[2] * DJB2_64_MULTIPLIER_P5 +
				start[3] * DJB2_64_MULTIPLIER_P4 +
				start[4] * DJB2_64_MULTIPLIER_P3 +
				start[5] * DJB2_64_MULTIPLIER_P2 +
				start[6] * DJB2_64_MULTIPLIER_P1 +
				start[7];

			start += 8;
		} while (start < end);
	}

	switch (n & 7) {
	case 7:
		hash = hash * DJB2_64_MULTIPLIER_P7 +
			end[0] * DJB2_64_MULTIPLIER_P6 +
			end[1] * DJB2_64_MULTIPLIER_P5 +
			end[2] * DJB2_64_MULTIPLIER_P4 +
			end[3] * DJB2_64_MULTIPLIER_P3 +
			end[4] * DJB2_64_MULTIPLIER_P2 +
			end[5] * DJB2_64_MULTIPLIER_P1 +
			end[6];
		break;

	case 6:
		hash = hash * DJB2_64_MULTIPLIER_P6 +
			end[0] * DJB2_64_MULTIPLIER_P5 +
			end[1] * DJB2_64_MULTIPLIER_P4 +
			end[2] * DJB2_64_MULTIPLIER_P3 +
			end[3] * DJB2_64_MULTIPLIER_P2 +
			end[4] * DJB2_64_MULTIPLIER_P1 +
			end[5];
		break;

	case 5:
		hash = hash * DJB2_64_MULTIPLIER_P5 +
			end[0] * DJB2_64_MULTIPLIER_P4 +
			end[1] * DJB2_64_MULTIPLIER_P3 +
			end[2] * DJB2_64_MULTIPLIER_P2 +
			end[3] * DJB2_64_MULTIPLIER_P1 +
			end[4];
		break;

	case 4:
		hash = hash * DJB2_64_MULTIPLIER_P4 +
			end[0] * DJB2_64_MULTIPLIER_P3 +
			end[1] * DJB2_64_MULTIPLIER_P2 +
			end[2] * DJB2_64_MULTIPLIER_P1 +
			end[3];
		break;

	case 3:
		hash = hash * DJB2_64_MULTIPLIER_P3 +
			end[0] * DJB2_64_MULTIPLIER_P2 +
			end[1] * DJB2_64_MULTIPLIER_P1 +
			end[2];
		break;

	case 2:
		hash = hash * DJB2_64_MULTIPLIER_P2 +
			end[0] * DJB2_64_MULTIPLIER_P1 +
			end[1];
		break;

	case 1:
		hash = hash * DJB2_64_MULTIPLIER_P1 +
			end[0];
		break;
	}
	return hash;
}

}
