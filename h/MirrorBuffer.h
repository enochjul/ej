//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_MIRROR_BUFFER_H
#define EJ_MIRROR_BUFFER_H

#include <stddef.h>

#include <sys/mman.h>

namespace ej {

class MirrorBuffer {
public:
	typedef size_t size_type;

private:
	void *Data;
	size_type AllocatedSize;
	size_type Size;
	size_type CurrentPosition;

public:
	struct CreateReturnType {
		void *Data;
		size_type Size;
	};

	explicit MirrorBuffer(const CreateReturnType &a) noexcept : Data(a.Data), AllocatedSize(a.Size), Size(0), CurrentPosition(0) {
	}
	~MirrorBuffer() noexcept {
		munmap(Data, AllocatedSize * 2);
	}

	MirrorBuffer(const MirrorBuffer &) = delete;
	MirrorBuffer &operator =(const MirrorBuffer &) = delete;

	static CreateReturnType create(const char *name, size_type n) noexcept;
	template <typename F>
	static bool with(const char *name, size_type n, F f) noexcept {
		auto r = create(name, n);
		if (r.Data != nullptr) {
			MirrorBuffer b(r);
			f(b);
			return true;
		} else {
			return false;
		}
	}

	void *data() noexcept {
		return Data;
	}

	const void *data() const noexcept {
		return Data;
	}

	const void *cdata() const noexcept {
		return Data;
	}

	size_type capacity() const noexcept {
		return AllocatedSize;
	}
};

}

#endif
