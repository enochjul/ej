//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_FNV_H
#define EJ_FNV_H

#include <stddef.h>
#include <stdint.h>

#include "Common.h"

namespace ej {

uint32_t fnv1_32(const char *s) noexcept;
uint64_t fnv1_64(const char *s) noexcept;

uint32_t fnv1a_32(const char *s) noexcept;
uint64_t fnv1a_64(const char *s) noexcept;

uint32_t fnv1_add_32(const char *s) noexcept;
uint64_t fnv1_add_64(const char *s) noexcept;

uint32_t fnv1a_add_32(const char *s) noexcept;
uint64_t fnv1a_add_64(const char *s) noexcept;

uint32_t fnv1_32(const void *s, size_t n) noexcept;
uint32_t fnv1_32(uint32_t value) noexcept;
uint32_t fnv1_32(uint64_t value) noexcept;
EJ_ALWAYS_INLINE uint32_t fnv1_32(int32_t value) noexcept {
	return fnv1_32(static_cast<uint32_t>(value));
}
EJ_ALWAYS_INLINE uint32_t fnv1_32(int64_t value) noexcept {
	return fnv1_32(static_cast<uint64_t>(value));
}

uint64_t fnv1_64(const void *s, size_t n) noexcept;
uint64_t fnv1_64(uint32_t value) noexcept;
uint64_t fnv1_64(uint64_t value) noexcept;
EJ_ALWAYS_INLINE uint64_t fnv1_64(int32_t value) noexcept {
	return fnv1_64(static_cast<uint32_t>(value));
}
EJ_ALWAYS_INLINE uint64_t fnv1_64(int64_t value) noexcept {
	return fnv1_64(static_cast<uint64_t>(value));
}

uint32_t fnv1a_32(const void *s, size_t n) noexcept;
uint32_t fnv1a_32(uint32_t value) noexcept;
uint32_t fnv1a_32(uint64_t value) noexcept;
EJ_ALWAYS_INLINE uint32_t fnv1a_32(int32_t value) noexcept {
	return fnv1a_32(static_cast<uint32_t>(value));
}
EJ_ALWAYS_INLINE uint32_t fnv1a_32(int64_t value) noexcept {
	return fnv1a_32(static_cast<uint64_t>(value));
}

uint64_t fnv1a_64(const void *s, size_t n) noexcept;
uint64_t fnv1a_64(uint32_t value) noexcept;
uint64_t fnv1a_64(uint64_t value) noexcept;
EJ_ALWAYS_INLINE uint64_t fnv1a_64(int32_t value) noexcept {
	return fnv1a_64(static_cast<uint32_t>(value));
}
EJ_ALWAYS_INLINE uint64_t fnv1a_64(int64_t value) noexcept {
	return fnv1a_64(static_cast<uint64_t>(value));
}

uint32_t fnv1_add_32(const void *s, size_t n) noexcept;
uint32_t fnv1_add_32(uint32_t value) noexcept;
uint32_t fnv1_add_32(uint64_t value) noexcept;
EJ_ALWAYS_INLINE uint32_t fnv1_add_32(int32_t value) noexcept {
	return fnv1_add_32(static_cast<uint32_t>(value));
}
EJ_ALWAYS_INLINE uint32_t fnv1_add_32(int64_t value) noexcept {
	return fnv1_add_32(static_cast<uint64_t>(value));
}

uint64_t fnv1_add_64(const void *s, size_t n) noexcept;
uint64_t fnv1_add_64(uint32_t value) noexcept;
uint64_t fnv1_add_64(uint64_t value) noexcept;
EJ_ALWAYS_INLINE uint64_t fnv1_add_64(int32_t value) noexcept {
	return fnv1_add_64(static_cast<uint32_t>(value));
}
EJ_ALWAYS_INLINE uint64_t fnv1_add_64(int64_t value) noexcept {
	return fnv1_add_64(static_cast<uint64_t>(value));
}

uint32_t fnv1a_add_32(const void *s, size_t n) noexcept;
uint32_t fnv1a_add_32(uint32_t value) noexcept;
uint32_t fnv1a_add_32(uint64_t value) noexcept;
EJ_ALWAYS_INLINE uint32_t fnv1a_add_32(int32_t value) noexcept {
	return fnv1a_add_32(static_cast<uint32_t>(value));
}
EJ_ALWAYS_INLINE uint32_t fnv1a_add_32(int64_t value) noexcept {
	return fnv1a_add_32(static_cast<uint64_t>(value));
}

uint64_t fnv1a_add_64(const void *s, size_t n) noexcept;
uint64_t fnv1a_add_64(uint32_t value) noexcept;
uint64_t fnv1a_add_64(uint64_t value) noexcept;
EJ_ALWAYS_INLINE uint64_t fnv1a_add_64(int32_t value) noexcept {
	return fnv1a_add_64(static_cast<uint32_t>(value));
}
EJ_ALWAYS_INLINE uint64_t fnv1a_add_64(int64_t value) noexcept {
	return fnv1a_add_64(static_cast<uint64_t>(value));
}

//! Helper class for computing the FNV1 hash of a string
template <typename T>
class FNV1Hash;

template <>
class FNV1Hash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1_32(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1_32(s, n);
	}

	template <typename K>
	static value_type eval(const K &k) noexcept {
		return eval(&k, sizeof(k));
	}
};

template <>
class FNV1Hash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1_64(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1_64(s, n);
	}
};

//! Helper class for computing the FNV1 hash of a string
template <typename T>
class FNV1aHash;

template <>
class FNV1aHash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1a_32(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1a_32(s, n);
	}
};

template <>
class FNV1aHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1a_64(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1a_64(s, n);
	}
};

//! Helper class for computing the FNV1 hash (with add instead of xor) of a string
template <typename T>
class FNV1AddHash;

template <>
class FNV1AddHash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1_add_32(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1_add_32(s, n);
	}

	template <typename K>
	static value_type eval(const K &k) noexcept {
		return eval(&k, sizeof(k));
	}
};

template <>
class FNV1AddHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1_add_64(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1_add_64(s, n);
	}

	template <typename K>
	static value_type eval(const K &k) noexcept {
		return eval(&k, sizeof(k));
	}
};

//! Helper class for computing the FNV1a hash (with add instead of xor) of a string
template <typename T>
class FNV1aAddHash;

template <>
class FNV1aAddHash<uint32_t> {
public:
	typedef uint32_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1a_add_32(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1a_add_32(s, n);
	}

	template <typename K>
	static value_type eval(const K &k) noexcept {
		return eval(&k, sizeof(k));
	}
};

template <>
class FNV1aAddHash<uint64_t> {
public:
	typedef uint64_t value_type;

	static value_type eval(const char *s) noexcept {
		return fnv1a_add_64(s);
	}

	static value_type eval(const void *s, size_t n) noexcept {
		return fnv1a_add_64(s, n);
	}

	template <typename K>
	static value_type eval(const K &k) noexcept {
		return eval(&k, sizeof(k));
	}
};

}

#endif
