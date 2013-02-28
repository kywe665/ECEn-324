	.file	"examples.c"
	.text
	.p2align 4,,15
.globl sum_list
	.type	sum_list, @function
sum_list:
.LFB0:
	.cfi_startproc
	xorl	%eax, %eax
	testq	%rdi, %rdi
	je	.L3
	.p2align 4,,10
	.p2align 3
.L6:
	addl	(%rdi), %eax
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	jne	.L6
.L3:
	rep
	ret
	.cfi_endproc
.LFE0:
	.size	sum_list, .-sum_list
	.p2align 4,,15
.globl rsum_list
	.type	rsum_list, @function
rsum_list:
.LFB1:
	.cfi_startproc
	xorl	%eax, %eax
	testq	%rdi, %rdi
	je	.L11
	.p2align 4,,10
	.p2align 3
.L14:
	movl	(%rdi), %edx
	movq	8(%rdi), %rdi
	addl	%edx, %eax
	testq	%rdi, %rdi
	jne	.L14
.L11:
	rep
	ret
	.cfi_endproc
.LFE1:
	.size	rsum_list, .-rsum_list
	.p2align 4,,15
.globl copy_block
	.type	copy_block, @function
copy_block:
.LFB2:
	.cfi_startproc
	xorl	%eax, %eax
	testl	%edx, %edx
	jle	.L18
	subl	$1, %edx
	leaq	4(,%rdx,4), %r8
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L19:
	movl	(%rdi,%rdx), %ecx
	movl	%ecx, (%rsi,%rdx)
	addq	$4, %rdx
	xorl	%ecx, %eax
	cmpq	%r8, %rdx
	jne	.L19
.L18:
	rep
	ret
	.cfi_endproc
.LFE2:
	.size	copy_block, .-copy_block
	.ident	"GCC: (GNU) 4.4.6 20120305 (Red Hat 4.4.6-4)"
	.section	.note.GNU-stack,"",@progbits
