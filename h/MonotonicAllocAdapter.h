//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_MONOTONIC_ALLOC_ADAPTER_H
#define EJ_MONOTONIC_ALLOC_ADAPTER_H

#include <utility>

#include "AllocAdapter.h"

namespace ej {

template <class Base>
class MonotonicAllocAdapter : public AllocAdapter<Base> {
public:
	using Base::initialize;
	using Base::destroy;
	using Base::getEnd;
	using Base::pop_frame;

	template <typename F, typename ... A>
	EJ_ALWAYS_INLINE static bool with(F f, A && ... args) noexcept {
		if (EJ_LIKELY(initialize(std::forward<A>(args) ...))) {
			f();
			destroy();
			return true;
		}
		return false;
	}

	template <typename F>
	EJ_ALWAYS_INLINE static void frame(F f) noexcept {
		auto frame_start = getEnd();
		f();
		pop_frame(frame_start);
	}
};

}

#endif
