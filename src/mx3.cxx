//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "mx3.h"

namespace ej {

uint64_t mx3_v1_hash(const void *data, size_t n, uint64_t seed) noexcept {
	static_assert(sizeof(uint64_t) == 8);

	uint64_t hash = static_cast<uint64_t>(n) ^ seed;

	const auto *start = data;
	for (auto aligned_n = n & ~static_cast<size_t>(31); aligned_n > 0; aligned_n -= 32) {
		hash = mx3_v1_stream(hash, loadu<uint64_t>(start));
		hash = mx3_v1_stream(hash, loadu<uint64_t>(lea(start, 8)));
		hash = mx3_v1_stream(hash, loadu<uint64_t>(lea(start, 16)));
		hash = mx3_v1_stream(hash, loadu<uint64_t>(lea(start, 24)));

		start = lea(start, 32);
	}

	for (auto aligned_n = n & 0x18; aligned_n > 0; aligned_n -= 8) {
		hash = mx3_v1_stream(hash, loadu<uint64_t>(start));

		start = lea(start, 8);
	}

	uint64_t last_block, low, high;
	switch (n & 7) {
	case 7:
		low = static_cast<uint64_t>(loadu<uint32_t>(start));
		high = static_cast<uint64_t>(loadu<uint32_t>(lea(start, 3)));
		last_block = low | (high << 24);
		goto final_round;

	case 6:
		low = static_cast<uint64_t>(loadu<uint32_t>(start));
		high = static_cast<uint64_t>(loadu<uint16_t>(lea(start, 4)));
		last_block = low | (high << 32);
		goto final_round;

	case 5:
		low = static_cast<uint64_t>(loadu<uint32_t>(start));
		high = static_cast<uint64_t>(loadu<uint8_t>(lea(start, 4)));
		last_block = low | (high << 32);
		goto final_round;

	case 4:
		last_block = static_cast<uint64_t>(loadu<uint32_t>(start));
		goto final_round;

	case 3:
		low = static_cast<uint64_t>(loadu<uint16_t>(start));
		high = static_cast<uint64_t>(loadu<uint8_t>(lea(start, 2)));
		last_block = low | (high << 16);
		goto final_round;

	case 2:
		last_block = static_cast<uint64_t>(loadu<uint16_t>(start));
		goto final_round;

	case 1:
		last_block = static_cast<uint64_t>(loadu<uint8_t>(start));
		goto final_round;

	case 0:
		goto final_mix;
	}

final_round:
	hash = mx3_v1_stream(hash, last_block);
final_mix:
	return mx3_v1_mix(hash);
}

uint64_t mx3_hash(const void *data, size_t n, uint64_t seed) noexcept {
	static_assert(sizeof(uint64_t) == 8);

	uint64_t hash = static_cast<uint64_t>(n) ^ seed;

	const auto *start = data;
	for (auto aligned_n = n & ~static_cast<size_t>(31); aligned_n > 0; aligned_n -= 32) {
		hash = mx3_stream(hash, loadu<uint64_t>(start));
		hash = mx3_stream(hash, loadu<uint64_t>(lea(start, 8)));
		hash = mx3_stream(hash, loadu<uint64_t>(lea(start, 16)));
		hash = mx3_stream(hash, loadu<uint64_t>(lea(start, 24)));

		start = lea(start, 32);
	}

	for (auto aligned_n = n & 0x18; aligned_n > 0; aligned_n -= 8) {
		hash = mx3_stream(hash, loadu<uint64_t>(start));

		start = lea(start, 8);
	}

	uint64_t last_block, low, high;
	switch (n & 7) {
	case 7:
		low = static_cast<uint64_t>(loadu<uint32_t>(start));
		high = static_cast<uint64_t>(loadu<uint32_t>(lea(start, 3)));
		last_block = low | (high << 24);
		goto final_round;

	case 6:
		low = static_cast<uint64_t>(loadu<uint32_t>(start));
		high = static_cast<uint64_t>(loadu<uint16_t>(lea(start, 4)));
		last_block = low | (high << 32);
		goto final_round;

	case 5:
		low = static_cast<uint64_t>(loadu<uint32_t>(start));
		high = static_cast<uint64_t>(loadu<uint8_t>(lea(start, 4)));
		last_block = low | (high << 32);
		goto final_round;

	case 4:
		last_block = static_cast<uint64_t>(loadu<uint32_t>(start));
		goto final_round;

	case 3:
		low = static_cast<uint64_t>(loadu<uint16_t>(start));
		high = static_cast<uint64_t>(loadu<uint8_t>(lea(start, 2)));
		last_block = low | (high << 16);
		goto final_round;

	case 2:
		last_block = static_cast<uint64_t>(loadu<uint16_t>(start));
		goto final_round;

	case 1:
		last_block = static_cast<uint64_t>(loadu<uint8_t>(start));
		goto final_round;

	case 0:
		goto final_mix;
	}

final_round:
	hash = mx3_stream(hash, last_block);
final_mix:
	return mx3_mix(hash);
}

}
