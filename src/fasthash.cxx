/* The MIT License

   Copyright (C) 2012 Zilong Tan (eric.zltan@gmail.com)

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#include <string.h>

#include "fasthash.h"

namespace ej {

uint64_t fasthash64(const void *s, size_t n, uint64_t seed) noexcept {
	const void *start;
	const void *end;
	uint64_t hash, v;
	size_t aligned_n;
	uint32_t v32;
	uint16_t v16;
	uint8_t v8;

	end = s;
	hash = seed ^ (n * FASTHASH_M);
	aligned_n = n & ~static_cast<size_t>(7);
	if (aligned_n > 0) {
		start = s;
		end = lea(end, aligned_n);
		do {
			memcpy(&v, start, sizeof(v));
			start = lea(start, sizeof(uint64_t));
			hash ^= fasthash_mix(v);
			hash *= FASTHASH_M;
		} while (start < end);
	}

	switch (n & 7u) {
	case 7:
		memcpy(&v32, end, sizeof(v32));
		end = lea(end, sizeof(uint32_t));
		memcpy(&v16, end, sizeof(v16));
		end = lea(end, sizeof(uint16_t));
		memcpy(&v8, end, sizeof(v8));

		v = (static_cast<uint64_t>(v8) << 48) | (static_cast<uint64_t>(v16) << 32) | v32;
		goto last_block;

	case 6:
		memcpy(&v32, end, sizeof(v32));
		end = lea(end, sizeof(uint32_t));
		memcpy(&v16, end, sizeof(v16));

		v = (static_cast<uint64_t>(v16) << 32) | v32;
		goto last_block;

	case 5:
		memcpy(&v32, end, sizeof(v32));
		end = lea(end, sizeof(uint32_t));
		memcpy(&v8, end, sizeof(v8));

		v = (static_cast<uint64_t>(v8) << 32) | v32;
		goto last_block;

	case 4:
		memcpy(&v32, end, sizeof(v32));
		v = v32;
		goto last_block;

	case 3:
		memcpy(&v16, end, sizeof(v16));
		end = lea(end, sizeof(uint16_t));
		memcpy(&v8, end, sizeof(v8));

		v32 = (static_cast<uint32_t>(v8) << 16) | v16;
		v = v32;
		goto last_block;

	case 2:
		memcpy(&v16, end, sizeof(v16));
		v = v16;
		goto last_block;

	case 1:
		memcpy(&v8, end, sizeof(v8));
		v = v8;
		goto last_block;

	case 0:
	default:
		goto final_mix;
	}

last_block:
	hash ^= fasthash_mix(v);
	hash *= FASTHASH_M;

final_mix:
	return fasthash_mix(hash);
}

}
