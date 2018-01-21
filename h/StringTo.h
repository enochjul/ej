//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_STRING_TO_H
#define EJ_STRING_TO_H

#include <stdint.h>

namespace ej {

template <typename T>
struct StringToReturnType {
	T Value;
	bool Status;
};

StringToReturnType<int32_t> c_string_to_int32(const char *s) noexcept;
StringToReturnType<int64_t> c_string_to_int64(const char *s) noexcept;
StringToReturnType<uint32_t> c_string_to_uint32(const char *s) noexcept;
StringToReturnType<uint64_t> c_string_to_uint64(const char *s) noexcept;
StringToReturnType<float> c_string_to_float(const char *s) noexcept;
StringToReturnType<double> c_string_to_double(const char *s) noexcept;

}

#endif
