//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

.section .text._ZN2ej10fill_int16EPsS0_s, "ax", @progbits

.p2align 4
.global _ZN2ej10fill_int16EPsS0_s
.global _ZN2ej11fill_uint16EPtS0_t
_ZN2ej10fill_int16EPsS0_s:
_ZN2ej11fill_uint16EPtS0_t:
	movq %rsi, %rax
	movd %edx, %xmm0
	subq %rdi, %rax
	punpcklwd %xmm0, %xmm0
	pshufd $0, %xmm0, %xmm0
	cmpq $127, %rax
	ja .Lfill_int16_size_at_least_128
	jmp *.Lfill_int16_small_fill_table(, %rax, 4)

.p2align 4
.Lfill_int16_size_at_least_128:
	movq %rdi, %rax
	lea 31(%rdi), %rdi
	andl $31, %eax
	andq $-32, %rdi
	jmp *.Lfill_int16_align_table(, %rax, 4)

.p2align 4,,10
.Lfill_int16_align_6:
	movw %dx, -26(%rdi)
	movq %xmm0, -24(%rdi)
	jmp .Lfill_int16_align_16

.p2align 4,,10
.Lfill_int16_align_10:
	movw %dx, -22(%rdi)
.Lfill_int16_align_12:
	movd %xmm0, -20(%rdi)
	jmp .Lfill_int16_align_16

.p2align 4,,5
.Lfill_int16_align_14:
	movw %dx, -18(%rdi)
	jmp .Lfill_int16_align_16

.p2align 4,,5
.Lfill_int16_align_30:
	movw %dx, -2(%rdi)
	jmp .Lfill_int16_align_0

.p2align 4
.Lfill_int16_align_18:
	movw %dx, -14(%rdi)
.Lfill_int16_align_20:
	movd %xmm0, -12(%rdi)
.Lfill_int16_align_24:
	movq %xmm0, -8(%rdi)
	jmp .Lfill_int16_align_0

.p2align 4,,10
.Lfill_int16_align_22:
	movw %dx, -10(%rdi)
	movq %xmm0, -8(%rdi)
	jmp .Lfill_int16_align_0

.p2align 4,,10
.Lfill_int16_align_26:
	movw %dx, -6(%rdi)
.Lfill_int16_align_28:
	movd %xmm0, -4(%rdi)
	jmp .Lfill_int16_align_0

	.byte 0x0f, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00
.Lfill_int16_align_2:
	movw %dx, -30(%rdi)
.Lfill_int16_align_4:
	movd %xmm0, -28(%rdi)
.Lfill_int16_align_8:
	movq %xmm0, -24(%rdi)

.p2align 4
.Lfill_int16_align_16:
	movdqa %xmm0, -16(%rdi)
.Lfill_int16_align_0:
	movq %rsi, %rcx
	subq %rdi, %rcx
	cmpq $1024, %rcx
	ja .Lfill_int16_do_rep_stos
	andq $-128, %rcx
	leaq (%rcx, %rdi), %rax
	jz .Lfill_int16_final_aligned_block

.p2align 4,,7
.Lfill_int16_loop_aligned_block_128:
	movdqa %xmm0, (%rdi)
	subq $-128, %rdi
	movdqa %xmm0, -112(%rdi)
	movdqa %xmm0, -96(%rdi)
	movdqa %xmm0, -80(%rdi)
	movdqa %xmm0, -64(%rdi)
	movdqa %xmm0, -48(%rdi)
	movdqa %xmm0, -32(%rdi)
	movdqa %xmm0, -16(%rdi)
	cmp %rdi, %rax
	ja .Lfill_int16_loop_aligned_block_128

.Lfill_int16_final_aligned_block:
	movq %rsi, %rax
	subq %rdi, %rax
	jmp *.Lfill_int16_final_aligned_fill_table(, %rax, 4)

.p2align 4,,8
.Lfill_int16_do_rep_stos:
	shrq $1, %rcx
	movl %edx, %eax
	rep stosw
	ret

	xchgw %ax, %ax
.Lsmall_fill_126:
	movdqu %xmm0, -126(%rsi)
.p2align 4
.Lsmall_fill_110:
	movdqu %xmm0, -110(%rsi)
.Lsmall_fill_94:
	movdqu %xmm0, -94(%rsi)
.Lsmall_fill_78:
	movdqu %xmm0, -78(%rsi)
.p2align 4
.Lsmall_fill_62:
	movdqu %xmm0, -62(%rsi)
.Lsmall_fill_46:
	movdqu %xmm0, -46(%rsi)
.Lsmall_fill_30:
	movdqu %xmm0, -30(%rsi)
.p2align 4
.Lsmall_fill_14:
	movq %xmm0, -14(%rsi)
	movd %xmm0, -6(%rsi)
	movw %dx, -2(%rsi)
	ret

	xchgw %ax, %ax
.Lsmall_fill_122:
	movdqu %xmm0, -122(%rsi)
.Lsmall_fill_106:
	movdqu %xmm0, -106(%rsi)
.Lsmall_fill_90:
	movdqu %xmm0, -90(%rsi)
.p2align 4
.Lsmall_fill_74:
	movdqu %xmm0, -74(%rsi)
.Lsmall_fill_58:
	movdqu %xmm0, -58(%rsi)
.Lsmall_fill_42:
	movdqu %xmm0, -42(%rsi)
.p2align 4
.Lsmall_fill_26:
	movdqu %xmm0, -26(%rsi)
.Lsmall_fill_10:
	movq %xmm0, -10(%rsi)
	movw %dx, -2(%rsi)
	ret

	xchgw %ax, %ax
.Lsmall_fill_118:
	movdqu %xmm0, -118(%rsi)
.Lsmall_fill_102:
	movdqu %xmm0, -102(%rsi)
.Lsmall_fill_86:
	movdqu %xmm0, -86(%rsi)
.p2align 4
.Lsmall_fill_70:
	movdqu %xmm0, -70(%rsi)
.Lsmall_fill_54:
	movdqu %xmm0, -54(%rsi)
.Lsmall_fill_38:
	movdqu %xmm0, -38(%rsi)
.p2align 4
.Lsmall_fill_22:
	movdqu %xmm0, -22(%rsi)
.Lsmall_fill_6:
	movd %xmm0, -6(%rsi)
	movw %dx, -2(%rsi)
	ret

	xchgw %ax, %ax
.Lsmall_fill_114:
	movdqu %xmm0, -114(%rsi)
.Lsmall_fill_98:
	movdqu %xmm0, -98(%rsi)
.Lsmall_fill_82:
	movdqu %xmm0, -82(%rsi)
.p2align 4
.Lsmall_fill_66:
	movdqu %xmm0, -66(%rsi)
.Lsmall_fill_50:
	movdqu %xmm0, -50(%rsi)
.Lsmall_fill_34:
	movdqu %xmm0, -34(%rsi)
.p2align 4
.Lsmall_fill_18:
	movdqu %xmm0, -18(%rsi)
.Lsmall_fill_2:
	movw %dx, -2(%rsi)
	ret

	nop
.Lfinal_aligned_fill_126:
	movdqa %xmm0, -126(%rsi)
.p2align 4
.Lfinal_aligned_fill_110:
	movdqa %xmm0, -110(%rsi)
.Lfinal_aligned_fill_94:
	movdqa %xmm0, -94(%rsi)
.Lfinal_aligned_fill_78:
	movdqa %xmm0, -78(%rsi)
.p2align 4
.Lfinal_aligned_fill_62:
	movdqa %xmm0, -62(%rsi)
.Lfinal_aligned_fill_46:
	movdqa %xmm0, -46(%rsi)
.Lfinal_aligned_fill_30:
	movdqa %xmm0, -30(%rsi)
.p2align 4
.Lfinal_aligned_fill_14:
	movq %xmm0, -14(%rsi)
	movd %xmm0, -6(%rsi)
	movw %dx, -2(%rsi)
	ret

	xchgw %ax, %ax
.Lfinal_aligned_fill_122:
	movdqa %xmm0, -122(%rsi)
.Lfinal_aligned_fill_106:
	movdqa %xmm0, -106(%rsi)
.Lfinal_aligned_fill_90:
	movdqa %xmm0, -90(%rsi)
.p2align 4
.Lfinal_aligned_fill_74:
	movdqa %xmm0, -74(%rsi)
.Lfinal_aligned_fill_58:
	movdqa %xmm0, -58(%rsi)
.Lfinal_aligned_fill_42:
	movdqa %xmm0, -42(%rsi)
.p2align 4
.Lfinal_aligned_fill_26:
	movdqa %xmm0, -26(%rsi)
.Lfinal_aligned_fill_10:
	movq %xmm0, -10(%rsi)
	movw %dx, -2(%rsi)
	ret

	xchgw %ax, %ax
.Lfinal_aligned_fill_118:
	movdqa %xmm0, -118(%rsi)
.Lfinal_aligned_fill_102:
	movdqa %xmm0, -102(%rsi)
.Lfinal_aligned_fill_86:
	movdqa %xmm0, -86(%rsi)
.p2align 4
.Lfinal_aligned_fill_70:
	movdqa %xmm0, -70(%rsi)
.Lfinal_aligned_fill_54:
	movdqa %xmm0, -54(%rsi)
.Lfinal_aligned_fill_38:
	movdqa %xmm0, -38(%rsi)
.p2align 4
.Lfinal_aligned_fill_22:
	movdqa %xmm0, -22(%rsi)
.Lfinal_aligned_fill_6:
	movd %xmm0, -6(%rsi)
	movw %dx, -2(%rsi)
	ret

	xchgw %ax, %ax
.Lfinal_aligned_fill_114:
	movdqa %xmm0, -114(%rsi)
.Lfinal_aligned_fill_98:
	movdqa %xmm0, -98(%rsi)
.Lfinal_aligned_fill_82:
	movdqa %xmm0, -82(%rsi)
.p2align 4
.Lfinal_aligned_fill_66:
	movdqa %xmm0, -66(%rsi)
.Lfinal_aligned_fill_50:
	movdqa %xmm0, -50(%rsi)
.Lfinal_aligned_fill_34:
	movdqa %xmm0, -34(%rsi)
.p2align 4
.Lfinal_aligned_fill_18:
	movdqa %xmm0, -18(%rsi)
.Lfinal_aligned_fill_2:
	movw %dx, -2(%rsi)
	ret

.section .rodata._ZN2ej10fill_int16EPsS0_s
.align 8
.Lfill_int16_small_fill_table:
	.quad .Lsmall_fill_0
	.quad .Lsmall_fill_2
	.quad .Lsmall_fill_4
	.quad .Lsmall_fill_6
	.quad .Lsmall_fill_8
	.quad .Lsmall_fill_10
	.quad .Lsmall_fill_12
	.quad .Lsmall_fill_14
	.quad .Lsmall_fill_16
	.quad .Lsmall_fill_18
	.quad .Lsmall_fill_20
	.quad .Lsmall_fill_22
	.quad .Lsmall_fill_24
	.quad .Lsmall_fill_26
	.quad .Lsmall_fill_28
	.quad .Lsmall_fill_30
	.quad .Lsmall_fill_32
	.quad .Lsmall_fill_34
	.quad .Lsmall_fill_36
	.quad .Lsmall_fill_38
	.quad .Lsmall_fill_40
	.quad .Lsmall_fill_42
	.quad .Lsmall_fill_44
	.quad .Lsmall_fill_46
	.quad .Lsmall_fill_48
	.quad .Lsmall_fill_50
	.quad .Lsmall_fill_52
	.quad .Lsmall_fill_54
	.quad .Lsmall_fill_56
	.quad .Lsmall_fill_58
	.quad .Lsmall_fill_60
	.quad .Lsmall_fill_62
	.quad .Lsmall_fill_64
	.quad .Lsmall_fill_66
	.quad .Lsmall_fill_68
	.quad .Lsmall_fill_70
	.quad .Lsmall_fill_72
	.quad .Lsmall_fill_74
	.quad .Lsmall_fill_76
	.quad .Lsmall_fill_78
	.quad .Lsmall_fill_80
	.quad .Lsmall_fill_82
	.quad .Lsmall_fill_84
	.quad .Lsmall_fill_86
	.quad .Lsmall_fill_88
	.quad .Lsmall_fill_90
	.quad .Lsmall_fill_92
	.quad .Lsmall_fill_94
	.quad .Lsmall_fill_96
	.quad .Lsmall_fill_98
	.quad .Lsmall_fill_100
	.quad .Lsmall_fill_102
	.quad .Lsmall_fill_104
	.quad .Lsmall_fill_106
	.quad .Lsmall_fill_108
	.quad .Lsmall_fill_110
	.quad .Lsmall_fill_112
	.quad .Lsmall_fill_114
	.quad .Lsmall_fill_116
	.quad .Lsmall_fill_118
	.quad .Lsmall_fill_120
	.quad .Lsmall_fill_122
	.quad .Lsmall_fill_124
	.quad .Lsmall_fill_126

.Lfill_int16_align_table:
	.quad .Lfill_int16_align_0
	.quad .Lfill_int16_align_2
	.quad .Lfill_int16_align_4
	.quad .Lfill_int16_align_6
	.quad .Lfill_int16_align_8
	.quad .Lfill_int16_align_10
	.quad .Lfill_int16_align_12
	.quad .Lfill_int16_align_14
	.quad .Lfill_int16_align_16
	.quad .Lfill_int16_align_18
	.quad .Lfill_int16_align_20
	.quad .Lfill_int16_align_22
	.quad .Lfill_int16_align_24
	.quad .Lfill_int16_align_26
	.quad .Lfill_int16_align_28
	.quad .Lfill_int16_align_30

.Lfill_int16_final_aligned_fill_table:
	.quad .Lfinal_aligned_fill_0
	.quad .Lfinal_aligned_fill_2
	.quad .Lfinal_aligned_fill_4
	.quad .Lfinal_aligned_fill_6
	.quad .Lfinal_aligned_fill_8
	.quad .Lfinal_aligned_fill_10
	.quad .Lfinal_aligned_fill_12
	.quad .Lfinal_aligned_fill_14
	.quad .Lfinal_aligned_fill_16
	.quad .Lfinal_aligned_fill_18
	.quad .Lfinal_aligned_fill_20
	.quad .Lfinal_aligned_fill_22
	.quad .Lfinal_aligned_fill_24
	.quad .Lfinal_aligned_fill_26
	.quad .Lfinal_aligned_fill_28
	.quad .Lfinal_aligned_fill_30
	.quad .Lfinal_aligned_fill_32
	.quad .Lfinal_aligned_fill_34
	.quad .Lfinal_aligned_fill_36
	.quad .Lfinal_aligned_fill_38
	.quad .Lfinal_aligned_fill_40
	.quad .Lfinal_aligned_fill_42
	.quad .Lfinal_aligned_fill_44
	.quad .Lfinal_aligned_fill_46
	.quad .Lfinal_aligned_fill_48
	.quad .Lfinal_aligned_fill_50
	.quad .Lfinal_aligned_fill_52
	.quad .Lfinal_aligned_fill_54
	.quad .Lfinal_aligned_fill_56
	.quad .Lfinal_aligned_fill_58
	.quad .Lfinal_aligned_fill_60
	.quad .Lfinal_aligned_fill_62
	.quad .Lfinal_aligned_fill_64
	.quad .Lfinal_aligned_fill_66
	.quad .Lfinal_aligned_fill_68
	.quad .Lfinal_aligned_fill_70
	.quad .Lfinal_aligned_fill_72
	.quad .Lfinal_aligned_fill_74
	.quad .Lfinal_aligned_fill_76
	.quad .Lfinal_aligned_fill_78
	.quad .Lfinal_aligned_fill_80
	.quad .Lfinal_aligned_fill_82
	.quad .Lfinal_aligned_fill_84
	.quad .Lfinal_aligned_fill_86
	.quad .Lfinal_aligned_fill_88
	.quad .Lfinal_aligned_fill_90
	.quad .Lfinal_aligned_fill_92
	.quad .Lfinal_aligned_fill_94
	.quad .Lfinal_aligned_fill_96
	.quad .Lfinal_aligned_fill_98
	.quad .Lfinal_aligned_fill_100
	.quad .Lfinal_aligned_fill_102
	.quad .Lfinal_aligned_fill_104
	.quad .Lfinal_aligned_fill_106
	.quad .Lfinal_aligned_fill_108
	.quad .Lfinal_aligned_fill_110
	.quad .Lfinal_aligned_fill_112
	.quad .Lfinal_aligned_fill_114
	.quad .Lfinal_aligned_fill_116
	.quad .Lfinal_aligned_fill_118
	.quad .Lfinal_aligned_fill_120
	.quad .Lfinal_aligned_fill_122
	.quad .Lfinal_aligned_fill_124
	.quad .Lfinal_aligned_fill_126

.section .text._ZN2ej10fill_int32EPiS0_i, "ax", @progbits

.p2align 4
.global _ZN2ej10fill_int32EPiS0_i
.global _ZN2ej11fill_uint32EPjS0_j
_ZN2ej10fill_int32EPiS0_i:
_ZN2ej11fill_uint32EPjS0_j:
	movq %rsi, %rax
	movd %edx, %xmm0
	subq %rdi, %rax
	pshufd $0, %xmm0, %xmm0
	cmpq $127, %rax
	ja .Lfill_int32_size_at_least_128
	jmp *.Lfill_int32_small_fill_table(, %rax, 2)

.p2align 4
.Lfill_int32_size_at_least_128:
	movq %rdi, %rax
	lea 31(%rdi), %rdi
	andl $31, %eax
	andq $-32, %rdi
	jmp *.Lfill_int32_align_table(, %rax, 2)

.p2align 4,,9
.Lfill_int32_align_12:
	movl %edx, -20(%rdi)
	movdqa %xmm0, -16(%rdi)
	jmp .Lfill_int32_align_0

.p2align 4,,9
.Lfill_int32_align_20:
	movl %edx, -12(%rdi)
.Lfill_int32_align_24:
	movq %xmm0, -8(%rdi)
	jmp .Lfill_int32_align_0

.p2align 4,,4
.Lfill_int32_align_28:
	movl %edx, -4(%rdi)
	jmp .Lfill_int32_align_0

	.byte 0x0f, 0x1f, 0x40, 0x00
.Lfill_int32_align_4:
	movl %edx, -28(%rdi)
.Lfill_int32_align_8:
	movq %xmm0, -24(%rdi)
.Lfill_int32_align_16:
	movdqa %xmm0, -16(%rdi)

.p2align 4,,14
.Lfill_int32_align_0:
	movq %rsi, %rcx
	subq %rdi, %rcx
	cmpq $1024, %rcx
	ja .Lfill_int32_do_rep_stos
	andq $-128, %rcx
	leaq (%rcx, %rdi), %rax
	jz .Lfill_int32_final_aligned_block

.p2align 4,,7
.Lfill_int32_loop_aligned_block_128:
	movdqa %xmm0, (%rdi)
	subq $-128, %rdi
	movdqa %xmm0, -112(%rdi)
	movdqa %xmm0, -96(%rdi)
	movdqa %xmm0, -80(%rdi)
	movdqa %xmm0, -64(%rdi)
	movdqa %xmm0, -48(%rdi)
	movdqa %xmm0, -32(%rdi)
	movdqa %xmm0, -16(%rdi)
	cmp %rdi, %rax
	ja .Lfill_int32_loop_aligned_block_128

.Lfill_int32_final_aligned_block:
	movq %rsi, %rax
	subq %rdi, %rax
	jmp *.Lfill_int32_final_aligned_fill_table(, %rax, 2)

.p2align 4,,8
.Lfill_int32_do_rep_stos:
	shrq $2, %rcx
	movl %edx, %eax
	rep stosl
	ret

.section .rodata._ZN2ej10fill_int32EPiS0_i
.align 8
.Lfill_int32_small_fill_table:
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

.Lfill_int32_align_table:
	.quad .Lfill_int32_align_0
	.quad .Lfill_int32_align_4
	.quad .Lfill_int32_align_8
	.quad .Lfill_int32_align_12
	.quad .Lfill_int32_align_16
	.quad .Lfill_int32_align_20
	.quad .Lfill_int32_align_24
	.quad .Lfill_int32_align_28

.Lfill_int32_final_aligned_fill_table:
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

.section .text._ZN2ej10fill_int64EPlS0_l, "ax", @progbits

.p2align 4
.global _ZN2ej10fill_int64EPlS0_l
.global _ZN2ej11fill_uint64EPmS0_m
_ZN2ej10fill_int64EPlS0_l:
_ZN2ej11fill_uint64EPmS0_m:
	movq %rsi, %rax
	movq %rdx, %xmm0
	subq %rdi, %rax
	punpcklqdq %xmm0, %xmm0
	cmpq $127, %rax
	ja .Lfill_int64_size_at_least_128
	jmp *.Lfill_int64_small_fill_table(%rax)

.p2align 4,,13
.Lfill_int64_size_at_least_128:
	movq %rdi, %rax
	lea 31(%rdi), %rdi
	andl $31, %eax
	andq $-32, %rdi
	jmp *.Lfill_int64_align_table(%rax)

.p2align 4
	nop
.Lfill_int64_align_24:
	movq %rdx, -8(%rdi)
	jmp .Lfill_int64_align_0

.Lfill_int64_align_8:
	movq %rdx, -24(%rdi)
.Lfill_int64_align_16:
	movdqa %xmm0, -16(%rdi)

.p2align 4,,12
.Lfill_int64_align_0:
	movq %rsi, %rcx
	subq %rdi, %rcx
	cmpq $1024, %rcx
	ja .Lfill_int64_do_rep_stos
	andq $-128, %rcx
	leaq (%rcx, %rdi), %rax
	jz .Lfill_int64_final_aligned_block

.p2align 4,,7
.Lfill_int64_loop_aligned_block_128:
	movdqa %xmm0, (%rdi)
	subq $-128, %rdi
	movdqa %xmm0, -112(%rdi)
	movdqa %xmm0, -96(%rdi)
	movdqa %xmm0, -80(%rdi)
	movdqa %xmm0, -64(%rdi)
	movdqa %xmm0, -48(%rdi)
	movdqa %xmm0, -32(%rdi)
	movdqa %xmm0, -16(%rdi)
	cmp %rdi, %rax
	ja .Lfill_int64_loop_aligned_block_128

.p2align 4,,11
.Lfill_int64_final_aligned_block:
	movq %rsi, %rax
	subq %rdi, %rax
	jmp *.Lfill_int64_final_aligned_fill_table(%rax)

.p2align 4,,10
.Lfill_int64_do_rep_stos:
	shrq $3, %rcx
	movq %rdx, %rax
	rep stosq
	ret

.section .rodata._ZN2ej10fill_int64EPlS0_l
.align 8
.Lfill_int64_small_fill_table:
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

.Lfill_int64_align_table:
	.quad .Lfill_int64_align_0
	.quad .Lfill_int64_align_8
	.quad .Lfill_int64_align_16
	.quad .Lfill_int64_align_24

.Lfill_int64_final_aligned_fill_table:
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

.section .text.fill_int_16_32_common, "ax", @progbits
.p2align 4
	nop
.Lsmall_fill_124:
	movdqu %xmm0, -124(%rsi)
.Lsmall_fill_108:
	movdqu %xmm0, -108(%rsi)
.Lsmall_fill_92:
	movdqu %xmm0, -92(%rsi)
.p2align 4
.Lsmall_fill_76:
	movdqu %xmm0, -76(%rsi)
.Lsmall_fill_60:
	movdqu %xmm0, -60(%rsi)
.Lsmall_fill_44:
	movdqu %xmm0, -44(%rsi)
.p2align 4
.Lsmall_fill_28:
	movdqu %xmm0, -28(%rsi)
.Lsmall_fill_12:
	movq %xmm0, -12(%rsi)
	movd %xmm0, -4(%rsi)
	ret

.p2align 4
	nop
.Lsmall_fill_116:
	movdqu %xmm0, -116(%rsi)
.Lsmall_fill_100:
	movdqu %xmm0, -100(%rsi)
.Lsmall_fill_84:
	movdqu %xmm0, -84(%rsi)
.p2align 4
.Lsmall_fill_68:
	movdqu %xmm0, -68(%rsi)
.Lsmall_fill_52:
	movdqu %xmm0, -52(%rsi)
.Lsmall_fill_36:
	movdqu %xmm0, -36(%rsi)
.p2align 4
.Lsmall_fill_20:
	movdqu %xmm0, -20(%rsi)
.Lsmall_fill_4:
	movd %xmm0, -4(%rsi)
	ret

.Lfinal_aligned_fill_124:
	movdqa %xmm0, -124(%rsi)
.p2align 4
.Lfinal_aligned_fill_108:
	movdqa %xmm0, -108(%rsi)
.Lfinal_aligned_fill_92:
	movdqa %xmm0, -92(%rsi)
.Lfinal_aligned_fill_76:
	movdqa %xmm0, -76(%rsi)
.p2align 4
.Lfinal_aligned_fill_60:
	movdqa %xmm0, -60(%rsi)
.Lfinal_aligned_fill_44:
	movdqa %xmm0, -44(%rsi)
.Lfinal_aligned_fill_28:
	movdqa %xmm0, -28(%rsi)
.p2align 4
.Lfinal_aligned_fill_12:
	movq %xmm0, -12(%rsi)
	movd %xmm0, -4(%rsi)
	ret

.Lfinal_aligned_fill_116:
	movdqa %xmm0, -116(%rsi)
.p2align 4
.Lfinal_aligned_fill_100:
	movdqa %xmm0, -100(%rsi)
.Lfinal_aligned_fill_84:
	movdqa %xmm0, -84(%rsi)
.Lfinal_aligned_fill_68:
	movdqa %xmm0, -68(%rsi)
.p2align 4
.Lfinal_aligned_fill_52:
	movdqa %xmm0, -52(%rsi)
.Lfinal_aligned_fill_36:
	movdqa %xmm0, -36(%rsi)
.Lfinal_aligned_fill_20:
	movdqa %xmm0, -20(%rsi)
.p2align 4
.Lfinal_aligned_fill_4:
	movd %xmm0, -4(%rsi)
	ret

.section .text.fill_int_common, "ax", @progbits
.p2align 4
	nop
.Lsmall_fill_120:
	movdqu %xmm0, -120(%rsi)
.Lsmall_fill_104:
	movdqu %xmm0, -104(%rsi)
.Lsmall_fill_88:
	movdqu %xmm0, -88(%rsi)
.p2align 4
.Lsmall_fill_72:
	movdqu %xmm0, -72(%rsi)
.Lsmall_fill_56:
	movdqu %xmm0, -56(%rsi)
.Lsmall_fill_40:
	movdqu %xmm0, -40(%rsi)
.p2align 4
.Lsmall_fill_24:
	movdqu %xmm0, -24(%rsi)
.Lsmall_fill_8:
	movq %xmm0, -8(%rsi)
	ret

.Lsmall_fill_112:
	movdqu %xmm0, -112(%rsi)
.p2align 4
.Lsmall_fill_96:
	movdqu %xmm0, -96(%rsi)
.Lsmall_fill_80:
	movdqu %xmm0, -80(%rsi)
.Lsmall_fill_64:
	movdqu %xmm0, -64(%rsi)
.p2align 4
.Lsmall_fill_48:
	movdqu %xmm0, -48(%rsi)
.Lsmall_fill_32:
	movdqu %xmm0, -32(%rsi)
.Lsmall_fill_16:
	movdqu %xmm0, -16(%rsi)
.Lsmall_fill_0:
	ret

	nop
.Lfinal_aligned_fill_120:
	movdqa %xmm0, -120(%rsi)
.Lfinal_aligned_fill_104:
	movdqa %xmm0, -104(%rsi)
.Lfinal_aligned_fill_88:
	movdqa %xmm0, -88(%rsi)
.p2align 4
.Lfinal_aligned_fill_72:
	movdqa %xmm0, -72(%rsi)
.Lfinal_aligned_fill_56:
	movdqa %xmm0, -56(%rsi)
.Lfinal_aligned_fill_40:
	movdqa %xmm0, -40(%rsi)
.p2align 4
.Lfinal_aligned_fill_24:
	movdqa %xmm0, -24(%rsi)
.Lfinal_aligned_fill_8:
	movq %xmm0, -8(%rsi)
	ret

.Lfinal_aligned_fill_112:
	movdqa %xmm0, -112(%rsi)
.p2align 4
.Lfinal_aligned_fill_96:
	movdqa %xmm0, -96(%rsi)
.Lfinal_aligned_fill_80:
	movdqa %xmm0, -80(%rsi)
.Lfinal_aligned_fill_64:
	movdqa %xmm0, -64(%rsi)
.p2align 4
.Lfinal_aligned_fill_48:
	movdqa %xmm0, -48(%rsi)
.Lfinal_aligned_fill_32:
	movdqa %xmm0, -32(%rsi)
.Lfinal_aligned_fill_16:
	movdqa %xmm0, -16(%rsi)
.Lfinal_aligned_fill_0:
	ret
