//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_FILE_H
#define EJ_FILE_H

#include <assert.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Common.h"
#include "StatusCode.h"

namespace ej {

class File {
public:
	typedef int native_type;

	struct OpenOutcome {
		native_type Value;
		StatusCode Error;
	};

	static OpenOutcome open(const char *path, int flags) noexcept {
		assert((flags & O_CREAT) == 0 && (flags & O_TMPFILE) == 0);

		native_type fd = ::open(path, flags);
		if (fd >= 0) {
			return OpenOutcome{ fd, StatusCode() };
		} else {
			return OpenOutcome{ fd, StatusCode(errno) };
		}
	}

	StatusCode close(native_type fd) noexcept {
		auto status = ::close(fd);
		if (EJ_LIKELY(status == 0)) {
			return StatusCode();
		} else {
			return StatusCode(errno);
		}
	}

	static StatusCode write(native_type fd, const void *buf, size_t count) noexcept;
	static StatusCode lwrite(native_type fd, const void *buf, size_t count) noexcept;
};

};

#endif
