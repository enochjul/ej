//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stddef.h>

#include "xxhash.h"

namespace ej {

//! Represents an immutable string, usually stored in a string pool
template <class T, class HasherType, bool null_terminated, typename CharType, typename SizeType>
class StringBase {
public:
	typedef CharType value_type;
	typedef HasherType hasher_type;
	typedef typename HasherType::value_type hash_type;
	typedef SizeType size_type;
	typedef const value_type *const_iterator;

	enum : bool {
		NullTerminated = null_terminated
	};

private:
	T *Next;
	hash_type Hash;
	size_type Size;
	value_type Data[];

public:
	static constexpr size_t sizeofHeader() noexcept {
		return offsetof(StringBase, Data);
	}

	T *next() const noexcept {
		return Next;
	}

	T *getNext() const noexcept {
		return Next;
	}

	void setNext(T *next) noexcept {
		Next = next;
	}

	hash_type hash() const noexcept {
		return Hash;
	}

	hash_type getHash() const noexcept {
		return Hash;
	}

	void setHash(hash_type hash) noexcept {
		Hash = hash;
	}

	size_type size() const noexcept {
		return Size;
	}

	size_type getSize() const noexcept {
		return Size;
	}

	void setSize(size_type size) noexcept {
		Size = size;
	}

	value_type *data() noexcept {
		return Data;
	}

	const value_type *data() const noexcept {
		return Data;
	}

	value_type *getData() noexcept {
		return Data;
	}

	const value_type *getData() const noexcept {
		return Data;
	}
};

template <class HasherType = ej::xxHash<size_t>, bool null_terminated = true, typename CharType = char, typename SizeType = size_t>
class String final : public StringBase<String<HasherType, null_terminated, CharType, SizeType>, HasherType, null_terminated, CharType, SizeType> {
};

template <class HasherType, typename SizeType>
class String<HasherType, true, char, SizeType> final : public StringBase<String<HasherType, true, char, SizeType>, HasherType, true, char, SizeType> {
	typedef StringBase<String<HasherType, true, char, SizeType>, HasherType, true, char, SizeType> super;

public:
	constexpr const typename super::value_type *c_str() const noexcept {
		return this->data();
	}
};

}
