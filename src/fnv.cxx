//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "fnv.h"

namespace ej {

enum : uint32_t {
	FNV_32_OFFSET_BASIS = UINT32_C(2166136261),
	FNV_32_MULTIPLIER = UINT32_C(16777619),
};

enum : uint64_t {
	FNV_64_OFFSET_BASIS = UINT64_C(14695981039346656037),
	FNV_64_MULTIPLIER = UINT64_C(1099511628211),
};

uint32_t fnv1_32(const char *s) noexcept {
	uint32_t hash;
	unsigned char ch;

	for (hash = FNV_32_OFFSET_BASIS; (ch = static_cast<unsigned char>(*s)) != '\0'; ++s) {
		hash = (hash * FNV_32_MULTIPLIER) ^ ch;
	}
	return hash;
}

uint64_t fnv1_64(const char *s) noexcept {
	uint64_t hash;
	unsigned char ch;

	for (hash = FNV_64_OFFSET_BASIS; (ch = static_cast<unsigned char>(*s)) != '\0'; ++s) {
		hash = (hash * FNV_64_MULTIPLIER) ^ ch;
	}
	return hash;
}

uint32_t fnv1a_32(const char *s) noexcept {
	uint32_t hash;
	unsigned char ch;

	for (hash = FNV_32_OFFSET_BASIS; (ch = static_cast<unsigned char>(*s)) != '\0'; ++s) {
		hash = (hash ^ ch) * FNV_32_MULTIPLIER;
	}
	return hash;
}

uint64_t fnv1a_64(const char *s) noexcept {
	uint64_t hash;
	unsigned char ch;

	for (hash = FNV_64_OFFSET_BASIS; (ch = static_cast<unsigned char>(*s)) != '\0'; ++s) {
		hash = (hash ^ ch) * FNV_64_MULTIPLIER;
	}
	return hash;
}

uint32_t fnv1_32(const void *s, size_t n) noexcept {
	const uint8_t *start;
	const uint8_t *end;
	uint32_t hash;

	hash = FNV_32_OFFSET_BASIS;
	for (start = static_cast<const uint8_t *>(s), end = start + n; start < end; ++start) {
		hash = (hash * FNV_32_MULTIPLIER) ^ *start;
	}
	return hash;
}

uint64_t fnv1_64(const void *s, size_t n) noexcept {
	const uint8_t *start;
	const uint8_t *end;
	uint64_t hash;

	hash = FNV_64_OFFSET_BASIS;
	for (start = static_cast<const uint8_t *>(s), end = start + n; start < end; ++start) {
		hash = (hash * FNV_64_MULTIPLIER) ^ *start;
	}
	return hash;
}

uint32_t fnv1a_32(const void *s, size_t n) noexcept {
	const uint8_t *start;
	const uint8_t *end;
	uint32_t hash;

	hash = FNV_32_OFFSET_BASIS;
	for (start = static_cast<const uint8_t *>(s), end = start + n; start < end; ++start) {
		hash = (hash ^ *start) * FNV_32_MULTIPLIER;
	}
	return hash;
}

uint64_t fnv1a_64(const void *s, size_t n) noexcept {
	const uint8_t *start;
	const uint8_t *end;
	uint64_t hash;

	hash = FNV_64_OFFSET_BASIS;
	for (start = static_cast<const uint8_t *>(s), end = start + n; start < end; ++start) {
		hash = (hash ^ *start) * FNV_64_MULTIPLIER;
	}
	return hash;
}

enum : uint32_t {
	FNV1_ADD_32_OFFSET_BASIS = UINT32_C(2166136261),
	FNV1_ADD_32_MULTIPLIER_P1 = UINT32_C(16777619),
	FNV1_ADD_32_MULTIPLIER_P2 = UINT32_C(637696617),
	FNV1_ADD_32_MULTIPLIER_P3 = UINT32_C(1055306571),
	FNV1_ADD_32_MULTIPLIER_P4 = UINT32_C(1345077009),
	FNV1_ADD_32_MULTIPLIER_P5 = UINT32_C(1185368003),
	FNV1_ADD_32_MULTIPLIER_P6 = UINT32_C(4233492473),
	FNV1_ADD_32_MULTIPLIER_P7 = UINT32_C(878009595),
	FNV1_ADD_32_MULTIPLIER_P8 = UINT32_C(1566662433),
};

uint32_t fnv1_add_32(const void *s, size_t n) noexcept {
	const uint8_t *start;
	const uint8_t *end;
	size_t aligned_n;
	uint32_t hash;

	end = static_cast<const uint8_t *>(s);
	hash = FNV1_ADD_32_OFFSET_BASIS;
	aligned_n = (n / 8) * 8;
	if (aligned_n > 0) {
		start = static_cast<const uint8_t *>(s);
		end += aligned_n;
		do {
			hash = hash * FNV1_ADD_32_MULTIPLIER_P8 +
				start[0] * FNV1_ADD_32_MULTIPLIER_P7 +
				start[1] * FNV1_ADD_32_MULTIPLIER_P6 +
				start[2] * FNV1_ADD_32_MULTIPLIER_P5 +
				start[3] * FNV1_ADD_32_MULTIPLIER_P4 +
				start[4] * FNV1_ADD_32_MULTIPLIER_P3 +
				start[5] * FNV1_ADD_32_MULTIPLIER_P2 +
				start[6] * FNV1_ADD_32_MULTIPLIER_P1 +
				start[7];

			start += 8;
		} while (start < end);
	}

	switch (n & 7) {
	case 7:
		hash = hash * FNV1_ADD_32_MULTIPLIER_P7 +
			end[0] * FNV1_ADD_32_MULTIPLIER_P6 +
			end[1] * FNV1_ADD_32_MULTIPLIER_P5 +
			end[2] * FNV1_ADD_32_MULTIPLIER_P4 +
			end[3] * FNV1_ADD_32_MULTIPLIER_P3 +
			end[4] * FNV1_ADD_32_MULTIPLIER_P2 +
			end[5] * FNV1_ADD_32_MULTIPLIER_P1 +
			end[6];
		break;

	case 6:
		hash = hash * FNV1_ADD_32_MULTIPLIER_P6 +
			end[0] * FNV1_ADD_32_MULTIPLIER_P5 +
			end[1] * FNV1_ADD_32_MULTIPLIER_P4 +
			end[2] * FNV1_ADD_32_MULTIPLIER_P3 +
			end[3] * FNV1_ADD_32_MULTIPLIER_P2 +
			end[4] * FNV1_ADD_32_MULTIPLIER_P1 +
			end[5];
		break;

	case 5:
		hash = hash * FNV1_ADD_32_MULTIPLIER_P5 +
			end[0] * FNV1_ADD_32_MULTIPLIER_P4 +
			end[1] * FNV1_ADD_32_MULTIPLIER_P3 +
			end[2] * FNV1_ADD_32_MULTIPLIER_P2 +
			end[3] * FNV1_ADD_32_MULTIPLIER_P1 +
			end[4];
		break;

	case 4:
		hash = hash * FNV1_ADD_32_MULTIPLIER_P4 +
			end[0] * FNV1_ADD_32_MULTIPLIER_P3 +
			end[1] * FNV1_ADD_32_MULTIPLIER_P2 +
			end[2] * FNV1_ADD_32_MULTIPLIER_P1 +
			end[3];
		break;

	case 3:
		hash = hash * FNV1_ADD_32_MULTIPLIER_P3 +
			end[0] * FNV1_ADD_32_MULTIPLIER_P2 +
			end[1] * FNV1_ADD_32_MULTIPLIER_P1 +
			end[2];
		break;

	case 2:
		hash = hash * FNV1_ADD_32_MULTIPLIER_P2 +
			end[0] * FNV1_ADD_32_MULTIPLIER_P1 +
			end[1];
		break;

	case 1:
		hash = hash * FNV1_ADD_32_MULTIPLIER_P1 +
			end[0];
		break;
	}
	return hash;
}

enum : uint64_t {
	FNV1_ADD_64_OFFSET_BASIS = UINT64_C(14695981039346656037),
	FNV1_ADD_64_MULTIPLIER_P1 = UINT64_C(1099511628211),
	FNV1_ADD_64_MULTIPLIER_P2 = UINT64_C(956575116354345),
	FNV1_ADD_64_MULTIPLIER_P3 = UINT64_C(624165263380053675),
	FNV1_ADD_64_MULTIPLIER_P4 = UINT64_C(11527715348014283921),
	FNV1_ADD_64_MULTIPLIER_P5 = UINT64_C(913917546033277539),
	FNV1_ADD_64_MULTIPLIER_P6 = UINT64_C(15895002104753931833),
	FNV1_ADD_64_MULTIPLIER_P7 = UINT64_C(14218562807570617051),
	FNV1_ADD_64_MULTIPLIER_P8 = UINT64_C(2232315406967589409),
};

uint64_t fnv1_add_64(const void *s, size_t n) noexcept {
	const uint8_t *start;
	const uint8_t *end;
	size_t aligned_n;
	uint64_t hash;

	end = static_cast<const uint8_t *>(s);
	hash = FNV1_ADD_64_OFFSET_BASIS;
	aligned_n = (n / 8) * 8;
	if (aligned_n > 0) {
		start = static_cast<const uint8_t *>(s);
		end += aligned_n;
		do {
			hash = hash * FNV1_ADD_64_MULTIPLIER_P8 +
				start[0] * FNV1_ADD_64_MULTIPLIER_P7 +
				start[1] * FNV1_ADD_64_MULTIPLIER_P6 +
				start[2] * FNV1_ADD_64_MULTIPLIER_P5 +
				start[3] * FNV1_ADD_64_MULTIPLIER_P4 +
				start[4] * FNV1_ADD_64_MULTIPLIER_P3 +
				start[5] * FNV1_ADD_64_MULTIPLIER_P2 +
				start[6] * FNV1_ADD_64_MULTIPLIER_P1 +
				start[7];

			start += 8;
		} while (start < end);
	}

	switch (n & 7) {
	case 7:
		hash = hash * FNV1_ADD_64_MULTIPLIER_P7 +
			end[0] * FNV1_ADD_64_MULTIPLIER_P6 +
			end[1] * FNV1_ADD_64_MULTIPLIER_P5 +
			end[2] * FNV1_ADD_64_MULTIPLIER_P4 +
			end[3] * FNV1_ADD_64_MULTIPLIER_P3 +
			end[4] * FNV1_ADD_64_MULTIPLIER_P2 +
			end[5] * FNV1_ADD_64_MULTIPLIER_P1 +
			end[6];
		break;

	case 6:
		hash = hash * FNV1_ADD_64_MULTIPLIER_P6 +
			end[0] * FNV1_ADD_64_MULTIPLIER_P5 +
			end[1] * FNV1_ADD_64_MULTIPLIER_P4 +
			end[2] * FNV1_ADD_64_MULTIPLIER_P3 +
			end[3] * FNV1_ADD_64_MULTIPLIER_P2 +
			end[4] * FNV1_ADD_64_MULTIPLIER_P1 +
			end[5];
		break;

	case 5:
		hash = hash * FNV1_ADD_64_MULTIPLIER_P5 +
			end[0] * FNV1_ADD_64_MULTIPLIER_P4 +
			end[1] * FNV1_ADD_64_MULTIPLIER_P3 +
			end[2] * FNV1_ADD_64_MULTIPLIER_P2 +
			end[3] * FNV1_ADD_64_MULTIPLIER_P1 +
			end[4];
		break;

	case 4:
		hash = hash * FNV1_ADD_64_MULTIPLIER_P4 +
			end[0] * FNV1_ADD_64_MULTIPLIER_P3 +
			end[1] * FNV1_ADD_64_MULTIPLIER_P2 +
			end[2] * FNV1_ADD_64_MULTIPLIER_P1 +
			end[3];
		break;

	case 3:
		hash = hash * FNV1_ADD_64_MULTIPLIER_P3 +
			end[0] * FNV1_ADD_64_MULTIPLIER_P2 +
			end[1] * FNV1_ADD_64_MULTIPLIER_P1 +
			end[2];
		break;

	case 2:
		hash = hash * FNV1_ADD_64_MULTIPLIER_P2 +
			end[0] * FNV1_ADD_64_MULTIPLIER_P1 +
			end[1];
		break;

	case 1:
		hash = hash * FNV1_ADD_64_MULTIPLIER_P1 +
			end[0];
		break;
	}
	return hash;
}

enum : uint32_t {
	FNV1A_ADD_32_OFFSET_BASIS = UINT32_C(2166136261),
	FNV1A_ADD_32_MULTIPLIER_P1 = UINT32_C(16777619),
	FNV1A_ADD_32_MULTIPLIER_P2 = UINT32_C(637696617),
	FNV1A_ADD_32_MULTIPLIER_P3 = UINT32_C(1055306571),
	FNV1A_ADD_32_MULTIPLIER_P4 = UINT32_C(1345077009),
	FNV1A_ADD_32_MULTIPLIER_P5 = UINT32_C(1185368003),
	FNV1A_ADD_32_MULTIPLIER_P6 = UINT32_C(4233492473),
	FNV1A_ADD_32_MULTIPLIER_P7 = UINT32_C(878009595),
	FNV1A_ADD_32_MULTIPLIER_P8 = UINT32_C(1566662433),
};

uint32_t fnv1a_add_32(const void *s, size_t n) noexcept {
	const uint8_t *start;
	const uint8_t *end;
	size_t aligned_n;
	uint32_t hash;

	end = static_cast<const uint8_t *>(s);
	hash = FNV1A_ADD_32_OFFSET_BASIS;
	aligned_n = (n / 8) * 8;
	if (aligned_n > 0) {
		start = static_cast<const uint8_t *>(s);
		end += aligned_n;
		do {
			hash = (hash + start[0]) * FNV1A_ADD_32_MULTIPLIER_P8 +
				start[1] * FNV1A_ADD_32_MULTIPLIER_P7 +
				start[2] * FNV1A_ADD_32_MULTIPLIER_P6 +
				start[3] * FNV1A_ADD_32_MULTIPLIER_P5 +
				start[4] * FNV1A_ADD_32_MULTIPLIER_P4 +
				start[5] * FNV1A_ADD_32_MULTIPLIER_P3 +
				start[6] * FNV1A_ADD_32_MULTIPLIER_P2 +
				start[7] * FNV1A_ADD_32_MULTIPLIER_P1;

			start += 8;
		} while (start < end);
	}

	switch (n & 7) {
	case 7:
		hash = (hash + end[0]) * FNV1A_ADD_32_MULTIPLIER_P7 +
			end[1] * FNV1A_ADD_32_MULTIPLIER_P6 +
			end[2] * FNV1A_ADD_32_MULTIPLIER_P5 +
			end[3] * FNV1A_ADD_32_MULTIPLIER_P4 +
			end[4] * FNV1A_ADD_32_MULTIPLIER_P3 +
			end[5] * FNV1A_ADD_32_MULTIPLIER_P2 +
			end[6] * FNV1A_ADD_32_MULTIPLIER_P1;
		break;

	case 6:
		hash = (hash + end[0]) * FNV1A_ADD_32_MULTIPLIER_P6 +
			end[1] * FNV1A_ADD_32_MULTIPLIER_P5 +
			end[2] * FNV1A_ADD_32_MULTIPLIER_P4 +
			end[3] * FNV1A_ADD_32_MULTIPLIER_P3 +
			end[4] * FNV1A_ADD_32_MULTIPLIER_P2 +
			end[5] * FNV1A_ADD_32_MULTIPLIER_P1;
		break;

	case 5:
		hash = (hash + end[0]) * FNV1A_ADD_32_MULTIPLIER_P5 +
			end[1] * FNV1A_ADD_32_MULTIPLIER_P4 +
			end[2] * FNV1A_ADD_32_MULTIPLIER_P3 +
			end[3] * FNV1A_ADD_32_MULTIPLIER_P2 +
			end[4] * FNV1A_ADD_32_MULTIPLIER_P1;
		break;

	case 4:
		hash = (hash + end[0]) * FNV1A_ADD_32_MULTIPLIER_P4 +
			end[1] * FNV1A_ADD_32_MULTIPLIER_P3 +
			end[2] * FNV1A_ADD_32_MULTIPLIER_P2 +
			end[3] * FNV1A_ADD_32_MULTIPLIER_P1;
		break;

	case 3:
		hash = (hash + end[0]) * FNV1A_ADD_32_MULTIPLIER_P3 +
			end[1] * FNV1A_ADD_32_MULTIPLIER_P2 +
			end[2] * FNV1A_ADD_32_MULTIPLIER_P1;
		break;

	case 2:
		hash = (hash + end[0]) * FNV1A_ADD_32_MULTIPLIER_P2 +
			end[1] * FNV1A_ADD_32_MULTIPLIER_P1;
		break;

	case 1:
		hash = (hash + end[0]) * FNV1A_ADD_32_MULTIPLIER_P1;
		break;
	}
	return hash;
}

enum : uint64_t {
	FNV1A_ADD_64_OFFSET_BASIS = UINT64_C(14695981039346656037),
	FNV1A_ADD_64_MULTIPLIER_P1 = UINT64_C(1099511628211),
	FNV1A_ADD_64_MULTIPLIER_P2 = UINT64_C(956575116354345),
	FNV1A_ADD_64_MULTIPLIER_P3 = UINT64_C(624165263380053675),
	FNV1A_ADD_64_MULTIPLIER_P4 = UINT64_C(11527715348014283921),
	FNV1A_ADD_64_MULTIPLIER_P5 = UINT64_C(913917546033277539),
	FNV1A_ADD_64_MULTIPLIER_P6 = UINT64_C(15895002104753931833),
	FNV1A_ADD_64_MULTIPLIER_P7 = UINT64_C(14218562807570617051),
	FNV1A_ADD_64_MULTIPLIER_P8 = UINT64_C(2232315406967589409),
};

uint64_t fnv1a_add_64(const void *s, size_t n) noexcept {
	const uint8_t *start;
	const uint8_t *end;
	size_t aligned_n;
	uint64_t hash;

	end = static_cast<const uint8_t *>(s);
	hash = FNV1A_ADD_64_OFFSET_BASIS;
	aligned_n = (n / 8) * 8;
	if (aligned_n > 0) {
		start = static_cast<const uint8_t *>(s);
		end += aligned_n;
		do {
			hash = (hash + start[0]) * FNV1A_ADD_64_MULTIPLIER_P8 +
				start[1] * FNV1A_ADD_64_MULTIPLIER_P7 +
				start[2] * FNV1A_ADD_64_MULTIPLIER_P6 +
				start[3] * FNV1A_ADD_64_MULTIPLIER_P5 +
				start[4] * FNV1A_ADD_64_MULTIPLIER_P4 +
				start[5] * FNV1A_ADD_64_MULTIPLIER_P3 +
				start[6] * FNV1A_ADD_64_MULTIPLIER_P2 +
				start[7] * FNV1A_ADD_64_MULTIPLIER_P1;

			start += 8;
		} while (start < end);
	}

	switch (n & 7) {
	case 7:
		hash = (hash + end[0]) * FNV1A_ADD_64_MULTIPLIER_P7 +
			end[1] * FNV1A_ADD_64_MULTIPLIER_P6 +
			end[2] * FNV1A_ADD_64_MULTIPLIER_P5 +
			end[3] * FNV1A_ADD_64_MULTIPLIER_P4 +
			end[4] * FNV1A_ADD_64_MULTIPLIER_P3 +
			end[5] * FNV1A_ADD_64_MULTIPLIER_P2 +
			end[6] * FNV1A_ADD_64_MULTIPLIER_P1;
		break;

	case 6:
		hash = (hash + end[0]) * FNV1A_ADD_64_MULTIPLIER_P6 +
			end[1] * FNV1A_ADD_64_MULTIPLIER_P5 +
			end[2] * FNV1A_ADD_64_MULTIPLIER_P4 +
			end[3] * FNV1A_ADD_64_MULTIPLIER_P3 +
			end[4] * FNV1A_ADD_64_MULTIPLIER_P2 +
			end[5] * FNV1A_ADD_64_MULTIPLIER_P1;
		break;

	case 5:
		hash = (hash + end[0]) * FNV1A_ADD_64_MULTIPLIER_P5 +
			end[1] * FNV1A_ADD_64_MULTIPLIER_P4 +
			end[2] * FNV1A_ADD_64_MULTIPLIER_P3 +
			end[3] * FNV1A_ADD_64_MULTIPLIER_P2 +
			end[4] * FNV1A_ADD_64_MULTIPLIER_P1;
		break;

	case 4:
		hash = (hash + end[0]) * FNV1A_ADD_64_MULTIPLIER_P4 +
			end[1] * FNV1A_ADD_64_MULTIPLIER_P3 +
			end[2] * FNV1A_ADD_64_MULTIPLIER_P2 +
			end[3] * FNV1A_ADD_64_MULTIPLIER_P1;
		break;

	case 3:
		hash = (hash + end[0]) * FNV1A_ADD_64_MULTIPLIER_P3 +
			end[1] * FNV1A_ADD_64_MULTIPLIER_P2 +
			end[2] * FNV1A_ADD_64_MULTIPLIER_P1;
		break;

	case 2:
		hash = (hash + end[0]) * FNV1A_ADD_64_MULTIPLIER_P2 +
			end[1] * FNV1A_ADD_64_MULTIPLIER_P1;
		break;

	case 1:
		hash = (hash + end[0]) * FNV1A_ADD_64_MULTIPLIER_P1;
		break;
	}
	return hash;
}

}
