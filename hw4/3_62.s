	.file	"3_62.c"
	.text
	.p2align 4,,15
.globl transpose
	.type	transpose, @function
transpose:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %ecx
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	movl	$1, %ebx
	addl	$16, %ecx
	cmpl	$4, %ebx
	je	.L5
.L8:
	movl	8(%ebp), %eax
	leal	(%eax,%ebx,4), %edx
	addl	$1, %ebx
	movl	$1, %eax
.L3:
	movl	(%edx), %edi
	movl	-4(%ecx,%eax,4), %esi
	movl	%edi, -4(%ecx,%eax,4)
	addl	$1, %eax
	movl	%esi, (%edx)
	addl	$16, %edx
	cmpl	%ebx, %eax
	jne	.L3
	addl	$16, %ecx
	cmpl	$4, %ebx
	jne	.L8
.L5:
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.size	transpose, .-transpose
	.p2align 4,,15
.globl transpose_optimized
	.type	transpose_optimized, @function
transpose_optimized:
	pushl	%ebp
	movl	%esp, %ebp
	popl	%ebp
	ret
	.size	transpose_optimized, .-transpose_optimized
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%8d   "
	.text
	.p2align 4,,15
.globl print_matrix
	.type	print_matrix, @function
print_matrix:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	xorl	%edi, %edi
	pushl	%esi
	pushl	%ebx
	subl	$28, %esp
	movl	8(%ebp), %esi
.L12:
	xorl	%ebx, %ebx
.L13:
	movl	(%esi,%ebx,4), %eax
	addl	$1, %ebx
	movl	$.LC0, (%esp)
	movl	%eax, 4(%esp)
	call	printf
	cmpl	$4, %ebx
	jne	.L13
	addl	$1, %edi
	addl	$16, %esi
	movl	$10, (%esp)
	call	putchar
	cmpl	$4, %edi
	jne	.L12
	addl	$28, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.size	print_matrix, .-print_matrix
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	pushl	%ebx
	subl	$92, %esp
	leal	16(%esp), %ebx
	movl	%ebx, (%esp)
	movl	$1, 16(%esp)
	movl	$2, 20(%esp)
	movl	$3, 24(%esp)
	movl	$4, 28(%esp)
	movl	$1, 32(%esp)
	movl	$2, 36(%esp)
	movl	$3, 40(%esp)
	movl	$4, 44(%esp)
	movl	$1, 48(%esp)
	movl	$2, 52(%esp)
	movl	$3, 56(%esp)
	movl	$4, 60(%esp)
	movl	$1, 64(%esp)
	movl	$2, 68(%esp)
	movl	$3, 72(%esp)
	movl	$4, 76(%esp)
	call	print_matrix
	movl	$10, (%esp)
	call	putchar
	movl	%ebx, (%esp)
	call	transpose
	movl	%ebx, (%esp)
	call	print_matrix
	addl	$92, %esp
	xorl	%eax, %eax
	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20120305 (Red Hat 4.4.6-4)"
	.section	.note.GNU-stack,"",@progbits
