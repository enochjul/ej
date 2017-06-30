//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

.section .text._ZN2ej11fill_doubleEPdS0_d, "ax", @progbits

.p2align 4
.global _ZN2ej11fill_doubleEPdS0_d
_ZN2ej11fill_doubleEPdS0_d:
	movq %rsi, %rax
	unpcklpd %xmm0, %xmm0
	subq %rdi, %rax
	cmpq $127, %rax
	ja .Lsize_at_least_128
	jmp *.Lsmall_fill_table(%rax)

.section .rodata.fill_double
.align 8
.Lsmall_fill_table:
	.quad .Lsmall_fill_0
	.quad .Lsmall_fill_8
	.quad .Lsmall_fill_16
	.quad .Lsmall_fill_24
	.quad .Lsmall_fill_32
	.quad .Lsmall_fill_40
	.quad .Lsmall_fill_48
	.quad .Lsmall_fill_56
	.quad .Lsmall_fill_64
	.quad .Lsmall_fill_72
	.quad .Lsmall_fill_80
	.quad .Lsmall_fill_88
	.quad .Lsmall_fill_96
	.quad .Lsmall_fill_104
	.quad .Lsmall_fill_112
	.quad .Lsmall_fill_120

.section .text._ZN2ej11fill_doubleEPdS0_d
.Lsmall_fill_120:
	movupd %xmm0, -120(%rsi)
.Lsmall_fill_104:
	movupd %xmm0, -104(%rsi)
.p2align 4
.Lsmall_fill_88:
	movupd %xmm0, -88(%rsi)
.Lsmall_fill_72:
	movupd %xmm0, -72(%rsi)
.Lsmall_fill_56:
	movupd %xmm0, -56(%rsi)
.p2align 4
.Lsmall_fill_40:
	movupd %xmm0, -40(%rsi)
.Lsmall_fill_24:
	movupd %xmm0, -24(%rsi)
.Lsmall_fill_8:
	movq %xmm0, -8(%rsi)
	ret

	nop
.Lsmall_fill_112:
	movupd %xmm0, -112(%rsi)
.Lsmall_fill_96:
	movupd %xmm0, -96(%rsi)
.Lsmall_fill_80:
	movupd %xmm0, -80(%rsi)
.p2align 4
.Lsmall_fill_64:
	movupd %xmm0, -64(%rsi)
.Lsmall_fill_48:
	movupd %xmm0, -48(%rsi)
.Lsmall_fill_32:
	movupd %xmm0, -32(%rsi)
.p2align 4
.Lsmall_fill_16:
	movupd %xmm0, -16(%rsi)
.Lsmall_fill_0:
	ret

.p2align 4,,9
.Lsize_at_least_128:
	movq %rdi, %rax
	lea 31(%rdi), %rdi
	andl $31, %eax
	andq $-32, %rdi
	jmp *.Lalign_table(%rax)

.section .rodata.fill_double
.align 8
.Lalign_table:
	.quad .Lalign_0
	.quad .Lalign_8
	.quad .Lalign_16
	.quad .Lalign_24

.section .text._ZN2ej11fill_doubleEPdS0_d
.p2align 4,,6
.Lalign_24:
	movq %xmm0, -8(%rdi)
	jmp .Lalign_0

.p2align 4,,9
.Lalign_8:
	movq %xmm0, -24(%rdi)
.Lalign_16:
	movapd %xmm0, -16(%rdi)

.p2align 4,,14
.Lalign_0:
	movq %rsi, %rcx
	subq %rdi, %rcx
	cmpq $1024, %rcx
	ja .Ldo_rep_stos
	andq $-128, %rcx
	leaq (%rcx, %rdi), %rax
	jz .Lfinal_aligned_block

.p2align 4,,7
.Lloop_aligned_block_128:
	movapd %xmm0, (%rdi)
	subq $-128, %rdi
	movapd %xmm0, -112(%rdi)
	movapd %xmm0, -96(%rdi)
	movapd %xmm0, -80(%rdi)
	movapd %xmm0, -64(%rdi)
	movapd %xmm0, -48(%rdi)
	movapd %xmm0, -32(%rdi)
	movapd %xmm0, -16(%rdi)
	cmp %rdi, %rax
	ja .Lloop_aligned_block_128

.Lfinal_aligned_block:
	movq %rsi, %rax
	subq %rdi, %rax
	jmp *.Lfinal_aligned_fill_table(%rax)

.p2align 4,,10
.Ldo_rep_stos:
	shrq $3, %rcx
	movq %xmm0, %rax
	rep stosq
	ret

.section .rodata.fill_double
.align 8
.Lfinal_aligned_fill_table:
	.quad .Lfinal_aligned_fill_0
	.quad .Lfinal_aligned_fill_8
	.quad .Lfinal_aligned_fill_16
	.quad .Lfinal_aligned_fill_24
	.quad .Lfinal_aligned_fill_32
	.quad .Lfinal_aligned_fill_40
	.quad .Lfinal_aligned_fill_48
	.quad .Lfinal_aligned_fill_56
	.quad .Lfinal_aligned_fill_64
	.quad .Lfinal_aligned_fill_72
	.quad .Lfinal_aligned_fill_80
	.quad .Lfinal_aligned_fill_88
	.quad .Lfinal_aligned_fill_96
	.quad .Lfinal_aligned_fill_104
	.quad .Lfinal_aligned_fill_112
	.quad .Lfinal_aligned_fill_120

.section .text._ZN2ej11fill_doubleEPdS0_d, "ax", @progbits

.p2align 4
.Lfinal_aligned_fill_120:
	movapd %xmm0, -120(%rsi)
.Lfinal_aligned_fill_104:
	movapd %xmm0, -104(%rsi)
.Lfinal_aligned_fill_88:
	movapd %xmm0, -88(%rsi)
.p2align 4
.Lfinal_aligned_fill_72:
	movapd %xmm0, -72(%rsi)
.Lfinal_aligned_fill_56:
	movapd %xmm0, -56(%rsi)
.Lfinal_aligned_fill_40:
	movapd %xmm0, -40(%rsi)
.p2align 4
.Lfinal_aligned_fill_24:
	movapd %xmm0, -24(%rsi)
.Lfinal_aligned_fill_8:
	movq %xmm0, -8(%rsi)
	ret

.Lfinal_aligned_fill_112:
	movapd %xmm0, -112(%rsi)
.p2align 4
.Lfinal_aligned_fill_96:
	movapd %xmm0, -96(%rsi)
.Lfinal_aligned_fill_80:
	movapd %xmm0, -80(%rsi)
.Lfinal_aligned_fill_64:
	movapd %xmm0, -64(%rsi)
.p2align 4
.Lfinal_aligned_fill_48:
	movapd %xmm0, -48(%rsi)
.Lfinal_aligned_fill_32:
	movapd %xmm0, -32(%rsi)
.Lfinal_aligned_fill_16:
	movapd %xmm0, -16(%rsi)
.Lfinal_aligned_fill_0:
	ret
