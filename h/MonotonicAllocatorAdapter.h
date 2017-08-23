//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_MONOTONIC_ALLOCATOR_ADAPTER_H
#define EJ_MONOTONIC_ALLOCATOR_ADAPTER_H

#include <utility>

#include "AllocatorAdapter.h"

namespace ej {

template <class Base>
class MonotonicAllocatorAdapter : public AllocatorAdapter<Base> {
public:
	using Base::initialize;
	using Base::destroy;
	using Base::getEnd;
	using Base::pop_frame;

	template <typename F, typename ... A>
	EJ_ALWAYS_INLINE static bool with(F f, A && ... args) noexcept {
		MonotonicAllocatorAdapter self;
		if (EJ_LIKELY(self.initialize(std::forward<A>(args) ...))) {
			f(self);
			self.destroy();
			return true;
		}
		return false;
	}

	template <typename F>
	EJ_ALWAYS_INLINE void frame(F f) noexcept {
		auto frame_start = getEnd();
		f();
		pop_frame(frame_start);
	}
};

}

#endif
