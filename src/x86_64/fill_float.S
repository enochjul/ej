//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

.section .text._ZN2ej10fill_floatEPfS0_f, "ax", @progbits

.p2align 4
.global _ZN2ej10fill_floatEPfS0_f
_ZN2ej10fill_floatEPfS0_f:
	movq %rsi, %rax
	shufps $0, %xmm0, %xmm0
	subq %rdi, %rax
	cmpq $127, %rax
	ja .Lsize_at_least_128
	jmp *.Lsmall_fill_table(, %rax, 2)

.section .rodata.fill_float
.align 8
.Lsmall_fill_table:
	.quad .Lsmall_fill_0
	.quad .Lsmall_fill_4
	.quad .Lsmall_fill_8
	.quad .Lsmall_fill_12
	.quad .Lsmall_fill_16
	.quad .Lsmall_fill_20
	.quad .Lsmall_fill_24
	.quad .Lsmall_fill_28
	.quad .Lsmall_fill_32
	.quad .Lsmall_fill_36
	.quad .Lsmall_fill_40
	.quad .Lsmall_fill_44
	.quad .Lsmall_fill_48
	.quad .Lsmall_fill_52
	.quad .Lsmall_fill_56
	.quad .Lsmall_fill_60
	.quad .Lsmall_fill_64
	.quad .Lsmall_fill_68
	.quad .Lsmall_fill_72
	.quad .Lsmall_fill_76
	.quad .Lsmall_fill_80
	.quad .Lsmall_fill_84
	.quad .Lsmall_fill_88
	.quad .Lsmall_fill_92
	.quad .Lsmall_fill_96
	.quad .Lsmall_fill_100
	.quad .Lsmall_fill_104
	.quad .Lsmall_fill_108
	.quad .Lsmall_fill_112
	.quad .Lsmall_fill_116
	.quad .Lsmall_fill_120
	.quad .Lsmall_fill_124

.section .text._ZN2ej10fill_floatEPfS0_f
	nop
.Lsmall_fill_124:
	movups %xmm0, -124(%rsi)
.p2align 4
.Lsmall_fill_108:
	movups %xmm0, -108(%rsi)
.Lsmall_fill_92:
	movups %xmm0, -92(%rsi)
.Lsmall_fill_76:
	movups %xmm0, -76(%rsi)
.Lsmall_fill_60:
	movups %xmm0, -60(%rsi)
.p2align 4
.Lsmall_fill_44:
	movups %xmm0, -44(%rsi)
.Lsmall_fill_28:
	movups %xmm0, -28(%rsi)
.Lsmall_fill_12:
	movlps %xmm0, -12(%rsi)
	movss %xmm0, -4(%rsi)
	ret

	xchgw %ax, %ax
.Lsmall_fill_120:
	movups %xmm0, -120(%rsi)
.Lsmall_fill_104:
	movups %xmm0, -104(%rsi)
.Lsmall_fill_88:
	movups %xmm0, -88(%rsi)
.Lsmall_fill_72:
	movups %xmm0, -72(%rsi)
.Lsmall_fill_56:
	movups %xmm0, -56(%rsi)
.Lsmall_fill_40:
	movups %xmm0, -40(%rsi)
.Lsmall_fill_24:
	movups %xmm0, -24(%rsi)
.Lsmall_fill_8:
	movlps %xmm0, -8(%rsi)
	ret

	nopl (%rax)
.Lsmall_fill_116:
	movups %xmm0, -116(%rsi)
.Lsmall_fill_100:
	movups %xmm0, -100(%rsi)
.p2align 4
.Lsmall_fill_84:
	movups %xmm0, -84(%rsi)
.Lsmall_fill_68:
	movups %xmm0, -68(%rsi)
.Lsmall_fill_52:
	movups %xmm0, -52(%rsi)
.Lsmall_fill_36:
	movups %xmm0, -36(%rsi)
.Lsmall_fill_20:
	movups %xmm0, -20(%rsi)
.Lsmall_fill_4:
	movss %xmm0, -4(%rsi)
	ret

	xchgw %ax, %ax
.Lsmall_fill_112:
	movups %xmm0, -112(%rsi)
.p2align 4
.Lsmall_fill_96:
	movups %xmm0, -96(%rsi)
.Lsmall_fill_80:
	movups %xmm0, -80(%rsi)
.Lsmall_fill_64:
	movups %xmm0, -64(%rsi)
.Lsmall_fill_48:
	movups %xmm0, -48(%rsi)
.p2align 4
.Lsmall_fill_32:
	movups %xmm0, -32(%rsi)
.Lsmall_fill_16:
	movups %xmm0, -16(%rsi)
.Lsmall_fill_0:
	ret

.p2align 4
.Lsize_at_least_128:
	movq %rdi, %rax
	lea 31(%rdi), %rdi
	andl $31, %eax
	andq $-32, %rdi
	jmp *.Lalign_table(, %rax, 2)

.section .rodata.fill_float
.align 8
.Lalign_table:
	.quad .Lalign_0
	.quad .Lalign_4
	.quad .Lalign_8
	.quad .Lalign_12
	.quad .Lalign_16
	.quad .Lalign_20
	.quad .Lalign_24
	.quad .Lalign_28

.section .text._ZN2ej10fill_floatEPfS0_f
.p2align 4,,9
.Lalign_12:
	movss %xmm0, -20(%rdi)
	movaps %xmm0, -16(%rdi)
	jmp .Lalign_0

.p2align 4,,9
.Lalign_20:
	movss %xmm0, -12(%rdi)
.Lalign_24:
	movlps %xmm0, -8(%rdi)
	jmp .Lalign_0

.p2align 4,,4
.Lalign_28:
	movss %xmm0, -4(%rdi)
	jmp .Lalign_0

.p2align 4,,2
.Lalign_4:
	movss %xmm0, -28(%rdi)
.Lalign_8:
	movlps %xmm0, -24(%rdi)
.Lalign_16:
	movaps %xmm0, -16(%rdi)

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
	movaps %xmm0, (%rdi)
	subq $-128, %rdi
	movaps %xmm0, -112(%rdi)
	movaps %xmm0, -96(%rdi)
	movaps %xmm0, -80(%rdi)
	movaps %xmm0, -64(%rdi)
	movaps %xmm0, -48(%rdi)
	movaps %xmm0, -32(%rdi)
	movaps %xmm0, -16(%rdi)
	cmp %rdi, %rax
	ja .Lloop_aligned_block_128

.Lfinal_aligned_block:
	movq %rsi, %rax
	subq %rdi, %rax
	jmp *.Lfinal_aligned_fill_table(, %rax, 2)

.p2align 4,,8
.Ldo_rep_stos:
	shrq $2, %rcx
	movd %xmm0, %eax
	rep stosl
	ret

.section .rodata.fill_float
.align 8
.Lfinal_aligned_fill_table:
	.quad .Lfinal_aligned_fill_0
	.quad .Lfinal_aligned_fill_4
	.quad .Lfinal_aligned_fill_8
	.quad .Lfinal_aligned_fill_12
	.quad .Lfinal_aligned_fill_16
	.quad .Lfinal_aligned_fill_20
	.quad .Lfinal_aligned_fill_24
	.quad .Lfinal_aligned_fill_28
	.quad .Lfinal_aligned_fill_32
	.quad .Lfinal_aligned_fill_36
	.quad .Lfinal_aligned_fill_40
	.quad .Lfinal_aligned_fill_44
	.quad .Lfinal_aligned_fill_48
	.quad .Lfinal_aligned_fill_52
	.quad .Lfinal_aligned_fill_56
	.quad .Lfinal_aligned_fill_60
	.quad .Lfinal_aligned_fill_64
	.quad .Lfinal_aligned_fill_68
	.quad .Lfinal_aligned_fill_72
	.quad .Lfinal_aligned_fill_76
	.quad .Lfinal_aligned_fill_80
	.quad .Lfinal_aligned_fill_84
	.quad .Lfinal_aligned_fill_88
	.quad .Lfinal_aligned_fill_92
	.quad .Lfinal_aligned_fill_96
	.quad .Lfinal_aligned_fill_100
	.quad .Lfinal_aligned_fill_104
	.quad .Lfinal_aligned_fill_108
	.quad .Lfinal_aligned_fill_112
	.quad .Lfinal_aligned_fill_116
	.quad .Lfinal_aligned_fill_120
	.quad .Lfinal_aligned_fill_124

.section .text._ZN2ej10fill_floatEPfS0_f
.p2align 4
.Lfinal_aligned_fill_124:
	movaps %xmm0, -124(%rsi)
.Lfinal_aligned_fill_108:
	movaps %xmm0, -108(%rsi)
.Lfinal_aligned_fill_92:
	movaps %xmm0, -92(%rsi)
.Lfinal_aligned_fill_76:
	movaps %xmm0, -76(%rsi)
.Lfinal_aligned_fill_60:
	movaps %xmm0, -60(%rsi)
.Lfinal_aligned_fill_44:
	movaps %xmm0, -44(%rsi)
.Lfinal_aligned_fill_28:
	movaps %xmm0, -28(%rsi)
.p2align 4
.Lfinal_aligned_fill_12:
	movq %xmm0, -12(%rsi)
	movss %xmm0, -4(%rsi)
	ret

//manuall align 1 byte
	nop
.Lfinal_aligned_fill_120:
	movaps %xmm0, -120(%rsi)
.Lfinal_aligned_fill_104:
	movaps %xmm0, -104(%rsi)
.Lfinal_aligned_fill_88:
	movaps %xmm0, -88(%rsi)
.Lfinal_aligned_fill_72:
	movaps %xmm0, -72(%rsi)
.Lfinal_aligned_fill_56:
	movaps %xmm0, -56(%rsi)
.Lfinal_aligned_fill_40:
	movaps %xmm0, -40(%rsi)
.Lfinal_aligned_fill_24:
	movaps %xmm0, -24(%rsi)
.Lfinal_aligned_fill_8:
	movq %xmm0, -8(%rsi)
	ret

	xchgw %ax, %ax
.Lfinal_aligned_fill_116:
	movaps %xmm0, -116(%rsi)
.Lfinal_aligned_fill_100:
	movaps %xmm0, -100(%rsi)
.Lfinal_aligned_fill_84:
	movaps %xmm0, -84(%rsi)
.Lfinal_aligned_fill_68:
	movaps %xmm0, -68(%rsi)
.Lfinal_aligned_fill_52:
	movaps %xmm0, -52(%rsi)
.Lfinal_aligned_fill_36:
	movaps %xmm0, -36(%rsi)
.Lfinal_aligned_fill_20:
	movaps %xmm0, -20(%rsi)
.p2align 4
.Lfinal_aligned_fill_4:
	movss %xmm0, -4(%rsi)
	ret

	xchgw %ax, %ax
.Lfinal_aligned_fill_112:
	movaps %xmm0, -112(%rsi)
.Lfinal_aligned_fill_96:
	movaps %xmm0, -96(%rsi)
.p2align 4
.Lfinal_aligned_fill_80:
	movaps %xmm0, -80(%rsi)
.Lfinal_aligned_fill_64:
	movaps %xmm0, -64(%rsi)
.Lfinal_aligned_fill_48:
	movaps %xmm0, -48(%rsi)
.Lfinal_aligned_fill_32:
	movaps %xmm0, -32(%rsi)
.p2align 4
.Lfinal_aligned_fill_16:
	movaps %xmm0, -16(%rsi)
.Lfinal_aligned_fill_0:
	ret
