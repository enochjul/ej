//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EJ_FUTEX_H
#define EJ_FUTEX_H

#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>

#ifndef __amd64__
#include <unistd.h>
#endif

namespace ej {

inline long futex_wait(int32_t *addr, int32_t expected_value, struct timespec *timeout = nullptr) {
#ifdef __amd64__
	register struct timespec *timeout_r __asm__("%r10") = timeout;
	long status;

	asm volatile ("syscall"
		  : "=a" (status)
		  : "0" (SYS_futex), "D" (addr), "S" (FUTEX_WAIT_PRIVATE), "d" (expected_value), "r" (timeout_r)
		  : "memory", "cc", "rcx", "r11");
	return status;
#else
	return syscall(SYS_futex, addr, FUTEX_WAIT_PRIVATE, expected_value, timeout);
#endif
}

inline long futex_wait(uint32_t *addr, uint32_t expected_value, struct timespec *timeout = nullptr) {
#ifdef __amd64__
	register struct timespec *timeout_r __asm__("%r10") = timeout;
	long status;

	asm volatile ("syscall"
		  : "=a" (status)
		  : "0" (SYS_futex), "D" (addr), "S" (FUTEX_WAIT_PRIVATE), "d" (expected_value), "r" (timeout_r)
		  : "memory", "cc", "rcx", "r11");
	return status;
#else
	return syscall(SYS_futex, addr, FUTEX_WAIT_PRIVATE, expected_value, timeout);
#endif
}

inline long futex_wake(int32_t *addr, int32_t n = 1) {
#ifdef __amd64__
	long status;

	asm volatile ("syscall"
		  : "=a" (status)
		  : "0" (SYS_futex), "D" (addr), "S" (FUTEX_WAKE_PRIVATE), "d" (n)
		  : "memory", "cc", "rcx", "r11");
	return status;
#else
	return syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, n);
#endif
}

inline long futex_wake(uint32_t *addr, int32_t n = 1) {
#ifdef __amd64__
	long status;

	asm volatile ("syscall"
		  : "=a" (status)
		  : "0" (SYS_futex), "D" (addr), "S" (FUTEX_WAKE_PRIVATE), "d" (n)
		  : "memory", "cc", "rcx", "r11");
	return status;
#else
	return syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, n);
#endif
}

}

#endif
