//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

namespace ej {

//! Represent error code from system calls and/or library calls
class StatusCode {
	int Value;

public:
	constexpr StatusCode() noexcept : Value(0) {
	}
	constexpr explicit StatusCode(int v) noexcept : Value(v) {
	}

	constexpr int value() const noexcept {
		return Value;
	}

	constexpr void set(int value) noexcept {
		Value = value;
	}

	constexpr void clear() noexcept {
		Value = 0;
	}

	constexpr bool success() const noexcept {
		return Value == 0;
	}

	constexpr bool failure() const noexcept {
		return Value != 0;
	}
};

}
