//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stdint.h>

#include "duint.h"
#include "quint.h"

namespace ej {

//! Implements one round of the Philox random number generator algorithm for a 2 word counter
template <typename T, T M>
EJ_ALWAYS_INLINE constexpr duint<T> philox2xN_round(T counter0, T counter1, T key) noexcept {
	auto v = duint_s<T>::mul(M, counter0);
	auto v0 = duint_s<T>::get_low(v), v1 = duint_s<T>::get_high(v);
	return duint_s<T>::make(v1 ^ key ^ counter1, v0);
}

template <typename T, T M, T K, unsigned N>
EJ_ALWAYS_INLINE constexpr duint<T> philox2xN_r(T counter0, T counter1, T *key) noexcept {
	static_assert(N > 0);
	if constexpr (N > 1) {
		auto r = philox2xN_r<T, M, K, N - 1>(counter0, counter1, key);
		counter0 = duint_s<T>::get_low(r), counter1 = duint_s<T>::get_high(r);
		*key += K;
	}
	return philox2xN_round<T, M>(counter0, counter1, *key);
}

template <typename T, T M, T K, unsigned N>
EJ_ALWAYS_INLINE constexpr duint<T> philox2xN_r(duint<T> counter, T *key) noexcept {
	return philox2xN_r<T, M, K, N>(duint_s<T>::get_low(counter), duint_s<T>::get_high(counter), key);
}

//! Implements one round of the Philox random number generator algorithm for a 4 word counter
template <typename T, T M0, T M1, T K0, T K1>
EJ_ALWAYS_INLINE constexpr void philox4xN_round(T r[4], T counter0, T counter1, T counter2, T counter3, T key0, T key1) noexcept {
	auto u0 = duint_s<T>::mul(M0, counter0);
	auto u1 = duint_s<T>::mul(M1, counter2);
	auto v0 = duint_s<T>::get_low(u0), v1 = duint_s<T>::get_high(u0);
	auto v2 = duint_s<T>::get_low(u1), v3 = duint_s<T>::get_high(u1);

	r[0] = v3 ^ counter1 ^ key0;
	r[1] = v2;
	r[2] = v1 ^ counter3 ^ key1;
	r[3] = v0;
}

template <typename T, T M0, T M1, T K0, T K1, unsigned N>
EJ_ALWAYS_INLINE constexpr void philox4xN_r(T r[4], T counter0, T counter1, T counter2, T counter3, T *key0, T *key1) noexcept {
	static_assert(N > 0);
	if constexpr (N > 1) {
		philox4xN_r<T, M0, M1, K0, K1, N - 1>(r, counter0, counter1, counter2, counter3, key0, key1);
		counter0 = r[0], counter1 = r[1], counter2 = r[2], counter3 = r[3];
		*key0 += K0;
		*key1 += K1;
	}
	philox4xN_round<T, M0, M1, K0, K1>(r, counter0, counter1, counter2, counter3, *key0, *key1);
}

template <unsigned N = 10>
constexpr duint32 philox2x32(uint32_t counter0, uint32_t counter1, uint32_t key) noexcept {
	return philox2xN_r<uint32_t, 0xD256D193u, 0x9E3779B9u, N>(counter0, counter1, &key);
}

template <unsigned N = 10>
constexpr duint64 philox2x64(uint64_t counter0, uint64_t counter1, uint64_t key) noexcept {
	return philox2xN_r<uint64_t, UINT64_C(0xD2B74407B1CE6E93), UINT64_C(0x9E3779B97F4A7C15), N>(counter0, counter1, &key);
}

template <unsigned N = 10>
constexpr duint32 philox2x32(const uint32_t counter[2], uint32_t key) noexcept {
	return philox2x32<N>(counter[0], counter[1], key);
}

template <unsigned N = 10>
constexpr duint64 philox2x64(const uint64_t counter[2], uint64_t key) noexcept {
	return philox2x64<N>(counter[0], counter[1], key);
}

template <unsigned N = 10>
constexpr duint32 philox2x32(duint32 counter, uint32_t key) noexcept {
	return philox2x32<N>(duint32_get_low(counter), duint32_get_high(counter), key);
}

template <unsigned N = 10>
constexpr duint64 philox2x64(duint64 counter, uint64_t key) noexcept {
	return philox2x64<N>(duint64_get_low(counter), duint64_get_high(counter), key);
}

template <unsigned N = 10>
constexpr void philox4x32(uint32_t r[4], uint32_t counter0, uint32_t counter1, uint32_t counter2, uint32_t counter3, uint32_t key0, uint32_t key1) noexcept {
	return philox4xN_r<uint32_t, 0xD2511F53u, 0xCD9E8D57u, 0x9E3779B9u, 0xBB67AE85u, N>(r, counter0, counter1, counter2, counter3, &key0, &key1);
}

template <unsigned N = 10>
constexpr quint32 philox4x32(uint32_t counter0, uint32_t counter1, uint32_t counter2, uint32_t counter3, uint32_t key0, uint32_t key1) noexcept {
	quint32 r(0, 0, 0, 0);
	philox4x32<N>(r.data(), counter0, counter1, counter2, counter3, key0, key1);
	return r;
}

template <unsigned N = 10>
constexpr void philox4x64(uint64_t r[4], uint64_t counter0, uint64_t counter1, uint64_t counter2, uint64_t counter3, uint64_t key0, uint64_t key1) noexcept {
	return philox4xN_r<uint64_t, UINT64_C(0xD2E7470EE14C6C93), UINT64_C(0xCA5A826395121157), UINT64_C(0x9E3779B97F4A7C15), UINT64_C(0xBB67AE8584CAA73B), N>(r, counter0, counter1, counter2, counter3, &key0, &key1);
}

template <unsigned N = 10>
constexpr quint64 philox4x64(uint64_t counter0, uint64_t counter1, uint64_t counter2, uint64_t counter3, uint64_t key0, uint64_t key1) noexcept {
	quint64 r(0, 0, 0, 0);
	philox4x64<N>(r.data(), counter0, counter1, counter2, counter3, key0, key1);
	return r;
}

template <unsigned N = 10>
constexpr void philox4x32(uint32_t r[4], const uint32_t counter[4], const uint32_t key[2]) noexcept {
	uint32_t key0 = key[0], key1 = key[1];
	return philox4xN_r<uint32_t, 0xD2511F53u, 0xCD9E8D57u, 0x9E3779B9u, 0xBB67AE85u, N>(r, counter[0], counter[1], counter[2], counter[3], &key0, &key1);
}

template <unsigned N = 10>
constexpr quint32 philox4x32(const uint32_t counter[4], const uint32_t key[2]) noexcept {
	quint32 r(0, 0, 0, 0);
	philox4x32<N>(r.data(), counter[0], counter[1], counter[2], counter[3], key[0], key[1]);
	return r;
}

template <unsigned N = 10>
constexpr void philox4x64(uint64_t r[4], const uint64_t counter[4], const uint64_t key[2]) noexcept {
	uint64_t key0 = key[0], key1 = key[1];
	return philox4xN_r<uint64_t, UINT64_C(0xD2E7470EE14C6C93), UINT64_C(0xCA5A826395121157), UINT64_C(0x9E3779B97F4A7C15), UINT64_C(0xBB67AE8584CAA73B), N>(r, counter[0], counter[1], counter[2], counter[3], &key0, &key1);
}

template <unsigned N = 10>
constexpr quint64 philox4x64(const uint64_t counter[4], const uint64_t key[2]) noexcept {
	quint64 r(0, 0, 0, 0);
	philox4x64<N>(r.data(), counter[0], counter[1], counter[2], counter[3], key[0], key[1]);
	return r;
}

}
