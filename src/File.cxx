//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <limits.h>

#include "Common.h"
#include "File.h"

namespace ej {

StatusCode File::write(native_type fd, const void *buf, size_t count) noexcept {
	assert(count <= SSIZE_MAX);
	for (;;) {
		auto written_size = ::write(fd, buf, count);
		if (written_size >= 0) {
			count -= written_size;
			buf = static_cast<const char *>(buf) + written_size;
			if (EJ_LIKELY(count == 0)) {
				return StatusCode();
			}
		} else {
			auto ec = errno;
			if (ec != EINTR) {
				return StatusCode(ec);
			}
		}
	}
}

StatusCode File::lwrite(native_type fd, const void *buf, size_t count) noexcept {
	if (EJ_UNLIKELY(count > SSIZE_MAX)) {
		for (;;) {
			auto written_size = ::write(fd, buf, SSIZE_MAX);
			if (written_size >= 0) {
				count -= written_size;
				buf = static_cast<const char *>(buf) + written_size;
				if (EJ_LIKELY(count <= SSIZE_MAX)) {
					break;
				}
			} else {
				auto ec = errno;
				if (ec != EINTR) {
					return StatusCode(ec);
				}
			}
		}
	}
	for (;;) {
		auto written_size = ::write(fd, buf, count);
		if (written_size >= 0) {
			count -= written_size;
			buf = static_cast<const char *>(buf) + written_size;
			if (EJ_LIKELY(count == 0)) {
				return StatusCode();
			}
		} else {
			auto ec = errno;
			if (ec != EINTR) {
				return StatusCode(ec);
			}
		}
	}
}

}
