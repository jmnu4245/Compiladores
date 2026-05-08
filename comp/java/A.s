	.file	"A.ll"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function main
.LCPI0_0:
	.quad	0x4000000000000000              # double 2
.LCPI0_1:
	.quad	0x400199999999999a              # double 2.2000000000000002
.LCPI0_2:
	.quad	0x3ff0000000000000              # double 1
.LCPI0_3:
	.quad	0xc00199999999999a              # double -2.2000000000000002
	.text
	.globl	main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$40, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movq	%rsi, 32(%rsp)
	decl	%edi
	movl	%edi, 28(%rsp)
	movl	$1, 12(%rsp)
	xorl	%ebx, %ebx
	movl	$.L.str.int, %edi
	movl	$1, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$1, 12(%rsp)
	movl	$.L.str.int, %edi
	movl	$1, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$-1, 12(%rsp)
	movl	$.L.str.int, %edi
	movl	$-1, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movb	$1, 11(%rsp)
	testb	%bl, %bl
	jne	.LBB0_2
# %bb.1:                                # %L_true_25
	movl	$.L.str.true, %edi
	jmp	.LBB0_3
.LBB0_2:                                # %L_false_25
	movl	$.L.str.false, %edi
.LBB0_3:                                # %L_endbool_25
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movb	$0, 11(%rsp)
	movb	$1, %al
	testb	%al, %al
	jne	.LBB0_5
# %bb.4:                                # %L_true_34
	movl	$.L.str.true, %edi
	jmp	.LBB0_6
.LBB0_5:                                # %L_false_34
	movl	$.L.str.false, %edi
.LBB0_6:                                # %L_endbool_34
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movabsq	$4611686018427387904, %r14      # imm = 0x4000000000000000
	movq	%r14, 16(%rsp)
	movsd	.LCPI0_0(%rip), %xmm0           # xmm0 = [2.0E+0,0.0E+0]
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movabsq	$4612136378390124954, %rbx      # imm = 0x400199999999999A
	movq	%rbx, 16(%rsp)
	movsd	.LCPI0_1(%rip), %xmm0           # xmm0 = [2.2000000000000002E+0,0.0E+0]
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	%r14, 16(%rsp)
	movl	$.L.str.double, %edi
	movsd	.LCPI0_0(%rip), %xmm0           # xmm0 = [2.0E+0,0.0E+0]
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$1, 12(%rsp)
	movabsq	$4607182418800017408, %rax      # imm = 0x3FF0000000000000
	movq	%rax, 16(%rsp)
	movsd	.LCPI0_2(%rip), %xmm0           # xmm0 = [1.0E+0,0.0E+0]
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	%rbx, 16(%rsp)
	movl	$.L.str.double, %edi
	movsd	.LCPI0_1(%rip), %xmm0           # xmm0 = [2.2000000000000002E+0,0.0E+0]
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movabsq	$-4611235658464650854, %rax     # imm = 0xC00199999999999A
	movq	%rax, 16(%rsp)
	movsd	.LCPI0_3(%rip), %xmm0           # xmm0 = [-2.2000000000000002E+0,0.0E+0]
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	xorps	%xmm0, %xmm0
	cvtsi2sdl	12(%rsp), %xmm0
	movsd	%xmm0, 16(%rsp)
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	subl	12(%rsp), %eax
	xorps	%xmm0, %xmm0
	cvtsi2sd	%eax, %xmm0
	movsd	%xmm0, 16(%rsp)
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	_global_var@GOTPCREL(%rip), %rax
	movl	$10, (%rax)
	movl	$.L.str.int, %edi
	movl	$10, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.str.lit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$40, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str.int,@object              # @.str.int
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.int:
	.asciz	"%d\n"
	.size	.L.str.int, 4

	.type	.L.str.double,@object           # @.str.double
.L.str.double:
	.asciz	"%.16e\n"
	.size	.L.str.double, 7

	.type	.L.str.true,@object             # @.str.true
.L.str.true:
	.asciz	"true\n"
	.size	.L.str.true, 6

	.type	.L.str.false,@object            # @.str.false
.L.str.false:
	.asciz	"false\n"
	.size	.L.str.false, 7

	.type	.L.str.string,@object           # @.str.string
.L.str.string:
	.asciz	"%s"
	.size	.L.str.string, 3

	.type	.L.str.lit.0,@object            # @.str.lit.0
.L.str.lit.0:
	.asciz	"\n"
	.size	.L.str.lit.0, 2

	.type	_global_var,@object             # @_global_var
	.bss
	.globl	_global_var
	.p2align	2, 0x0
_global_var:
	.long	0                               # 0x0
	.size	_global_var, 4

	.section	".note.GNU-stack","",@progbits
