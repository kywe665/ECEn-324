	.file	"3_56.c"
	.text
	.type	loop.1252, @function
loop.1252:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	8(%ebp), %eax
	andl	$1, %eax
	movl	%eax, -8(%ebp)
	movl	$1, -4(%ebp)
	jmp	.L2
.L3:
	notl	-8(%ebp)
.L2:
	cmpl	$0, -4(%ebp)
	jg	.L3
	movl	-8(%ebp), %eax
	leave
	ret
	.size	loop.1252, .-loop.1252
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20110731 (Red Hat 4.4.6-3)"
	.section	.note.GNU-stack,"",@progbits
