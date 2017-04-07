//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "Common.h"
#include "linux/FastMutex.h"
#include "MirrorBuffer.h"

namespace ej {

auto MirrorBuffer::create(const char *name, size_type n) noexcept -> CreateReturnType {
	static FastMutex mutex;

	if (n <= SIZE_MAX / 2) {
		//Align the size to a multiple of the page size
		auto page_size = sysconf(_SC_PAGE_SIZE);
		auto page_mask = page_size - 1;
		n = (n + page_mask) & ~page_mask;

		//Reserve an address range that is double in size of the requested size
		auto new_data = mmap(nullptr, n * 2, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (new_data != MAP_FAILED) {
			mutex.lock();
			auto shm_id = shm_open(name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
			if (shm_id >= 0) {
				shm_unlink(name);
				mutex.unlock();

				if (EJ_LIKELY(ftruncate(shm_id, n) == 0)) {
					if (mmap(new_data, n, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, shm_id, 0) != MAP_FAILED) {
						if (mmap(static_cast<char *>(new_data) + n, n, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, shm_id, 0) != MAP_FAILED) {
							return { new_data, n };
						}
					}
				}
			} else {
				mutex.unlock();
			}
		}
	}

	return { nullptr, 0 };
}

}
