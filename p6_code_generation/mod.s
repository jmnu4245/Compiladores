	.file	"mod.ll"
	.text
	.globl	_mod                            # -- Begin function _mod
	.p2align	4
	.type	_mod,@function
_mod:                                   # @_mod
	.cfi_startproc
# %bb.0:
	movl	%edi, %eax
	cltd
	idivl	%esi
	movl	%edx, %eax
	retq
.Lfunc_end0:
	.size	_mod, .Lfunc_end0-_mod
	.cfi_endproc
                                        # -- End function
	.globl	_main                           # -- Begin function _main
	.p2align	4
	.type	_main,@function
_main:                                  # @_main
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	xorl	%edi, %edi
	callq	_read@PLT
	movl	%eax, %ebx
	xorl	%edi, %edi
	callq	_read@PLT
	movl	%ebx, %edi
	movl	%eax, %esi
	callq	_mod@PLT
	movl	%eax, %edi
	callq	_write@PLT
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	_main, .Lfunc_end1-_main
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	xorl	%edi, %edi
	callq	_main@PLT
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
