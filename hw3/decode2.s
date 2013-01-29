	.file	"decode2.c"
	.text
	.type	decode.1253, @function
decode.1253:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	movl	16(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	imull	8(%ebp), %eax
	movl	16(%ebp), %edx
	movl	12(%ebp), %ecx
	movl	%ecx, %ebx
	subl	%edx, %ebx
	movl	%ebx, %edx
	sall	$31, %edx
	sarl	$31, %edx
	xorl	%edx, %eax
	popl	%ebx
	popl	%ebp
	ret
	.size	decode.1253, .-decode.1253
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
