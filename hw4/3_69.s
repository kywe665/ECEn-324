	.file	"3_69.c"
	.text
.globl trace
	.type	trace, @function
trace:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	$0, %eax
	testl	%edx, %edx
	je	.L3
	movl	(%edx), %eax
	testl	%eax, %eax
	je	.L3
.L4:
	movl	16(%edx), %edx
	movl	(%edx), %eax
	testl	%eax, %eax
	jne	.L4
.L3:
	popl	%ebp
	ret
	.size	trace, .-trace
	.ident	"GCC: (GNU) 4.4.6 20110731 (Red Hat 4.4.6-3)"
	.section	.note.GNU-stack,"",@progbits
