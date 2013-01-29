	.file	"3_59.c"
	.text
	.type	switch_prob.1254, @function
switch_prob.1254:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	cmpl	$6, 12(%ebp)
	ja	.L2
	movl	12(%ebp), %eax
	sall	$2, %eax
	movl	.L9(%eax), %eax
	jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L9:
	.long	.L2
	.long	.L3
	.long	.L4
	.long	.L5
	.long	.L6
	.long	.L7
	.long	.L8
	.text
.L3:
	movl	8(%ebp), %eax
	sall	$2, %eax
	movl	%eax, -4(%ebp)
	jmp	.L10
.L4:
	movl	8(%ebp), %eax
	addl	$10, %eax
	movl	%eax, -4(%ebp)
	jmp	.L10
.L5:
	movl	8(%ebp), %eax
	sall	$2, %eax
	movl	%eax, -4(%ebp)
	jmp	.L10
.L6:
	movl	8(%ebp), %eax
	sarl	$10, %eax
	movl	%eax, -4(%ebp)
	jmp	.L10
.L7:
	movl	8(%ebp), %eax
	addl	$1, %eax
	addl	%eax, %eax
	imull	8(%ebp), %eax
	addl	$10, %eax
	movl	%eax, -4(%ebp)
	jmp	.L10
.L8:
	movl	8(%ebp), %eax
	imull	8(%ebp), %eax
	addl	$10, %eax
	movl	%eax, -4(%ebp)
	jmp	.L10
.L2:
	movl	12(%ebp), %eax
	subl	$32, %eax
	cmpl	$5, %eax
	jle	.L10
	movl	8(%ebp), %eax
	addl	$10, %eax
	movl	%eax, -4(%ebp)
.L10:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	switch_prob.1254, .-switch_prob.1254
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	movl	$0, %eax
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20110731 (Red Hat 4.4.6-3)"
	.section	.note.GNU-stack,"",@progbits
