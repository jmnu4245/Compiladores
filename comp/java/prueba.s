	.file	"prueba.ll"
	.text
	.globl	_suma                           # -- Begin function _suma
	.p2align	4
	.type	_suma,@function
_suma:                                  # @_suma
	.cfi_startproc
# %bb.0:                                # %entry
                                        # kill: def $esi killed $esi def $rsi
                                        # kill: def $edi killed $edi def $rdi
	movl	%edi, -4(%rsp)
	movl	%esi, -8(%rsp)
	leal	(%rdi,%rsi), %eax
	retq
.Lfunc_end0:
	.size	_suma, .Lfunc_end0-_suma
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rsi, 16(%rsp)
	movl	%edi, 12(%rsp)
	movl	$5, %edi
	movl	$10, %esi
	callq	_suma@PLT
	movl	$.L.str.int, %edi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
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

	.type	_a,@object                      # @_a
	.bss
	.globl	_a
	.p2align	2, 0x0
_a:
	.long	0                               # 0x0
	.size	_a, 4

	.section	".note.GNU-stack","",@progbits
