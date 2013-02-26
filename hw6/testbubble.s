	.file	"testbubble.c"
.globl array1
	.data
	.align 4
	.type	array1, @object
	.size	array1, 28
array1:
	.long	56797
	.long	61166
	.long	48059
	.long	43690
	.long	65535
	.long	52428
	.long	257
.globl array2
	.align 4
	.type	array2, @object
	.size	array2, 28
array2:
	.long	56797
	.long	61166
	.long	48059
	.long	43690
	.long	65535
	.long	52428
	.long	257
	.text
.globl bubble_a
	.type	bubble_a, @function
bubble_a:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	12(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	.L2
.L6:
	movl	$0, -12(%ebp)
	jmp	.L3
.L5:
	movl	-12(%ebp), %eax
	addl	$1, %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	(%eax), %edx
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jge	.L4
	movl	-12(%ebp), %eax
	addl	$1, %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	-12(%ebp), %eax
	addl	$1, %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	8(%ebp), %edx
	movl	(%edx), %edx
	movl	%edx, (%eax)
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%edx, (%eax)
.L4:
	addl	$1, -12(%ebp)
.L3:
	movl	-12(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jl	.L5
	subl	$1, -8(%ebp)
.L2:
	cmpl	$0, -8(%ebp)
	jg	.L6
	leave
	ret
	.size	bubble_a, .-bubble_a
.globl bubble_p
	.type	bubble_p, @function
bubble_p:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -12(%ebp)
	jmp	.L9
.L13:
	movl	$0, -8(%ebp)
	jmp	.L10
.L12:
	movl	-8(%ebp), %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	(%eax), %edx
	movl	-8(%ebp), %eax
	addl	$1, %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jle	.L11
	movl	-8(%ebp), %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	-8(%ebp), %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	-8(%ebp), %edx
	addl	$1, %edx
	sall	$2, %edx
	addl	8(%ebp), %edx
	movl	(%edx), %edx
	movl	%edx, (%eax)
	movl	-8(%ebp), %eax
	addl	$1, %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%edx, (%eax)
.L11:
	addl	$1, -8(%ebp)
.L10:
	movl	12(%ebp), %eax
	subl	$1, %eax
	subl	-12(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jg	.L12
	addl	$1, -12(%ebp)
.L9:
	movl	12(%ebp), %eax
	subl	$1, %eax
	cmpl	-12(%ebp), %eax
	jg	.L13
	leave
	ret
	.size	bubble_p, .-bubble_p
	.section	.rodata
.LC0:
	.string	"array version"
.LC1:
	.string	"  Before: "
.LC2:
	.string	"0x%.4x "
.LC3:
	.string	"  After:  "
.LC4:
	.string	"pointer version"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$32, %esp
	movl	$.LC0, (%esp)
	call	puts
	movl	$.LC1, %eax
	movl	%eax, (%esp)
	call	printf
	movl	$0, 28(%esp)
	jmp	.L16
.L17:
	movl	28(%esp), %eax
	movl	array1(,%eax,4), %edx
	movl	$.LC2, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$1, 28(%esp)
.L16:
	cmpl	$5, 28(%esp)
	jle	.L17
	movl	$10, (%esp)
	call	putchar
	movl	$6, 4(%esp)
	movl	$array1, (%esp)
	call	bubble_a
	movl	$.LC3, %eax
	movl	%eax, (%esp)
	call	printf
	movl	$0, 28(%esp)
	jmp	.L18
.L19:
	movl	28(%esp), %eax
	movl	array1(,%eax,4), %edx
	movl	$.LC2, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$1, 28(%esp)
.L18:
	cmpl	$5, 28(%esp)
	jle	.L19
	movl	$10, (%esp)
	call	putchar
	movl	$.LC4, (%esp)
	call	puts
	movl	$.LC1, %eax
	movl	%eax, (%esp)
	call	printf
	movl	$0, 28(%esp)
	jmp	.L20
.L21:
	movl	28(%esp), %eax
	movl	array2(,%eax,4), %edx
	movl	$.LC2, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$1, 28(%esp)
.L20:
	cmpl	$5, 28(%esp)
	jle	.L21
	movl	$10, (%esp)
	call	putchar
	movl	$6, 4(%esp)
	movl	$array2, (%esp)
	call	bubble_p
	movl	$.LC3, %eax
	movl	%eax, (%esp)
	call	printf
	movl	$0, 28(%esp)
	jmp	.L22
.L23:
	movl	28(%esp), %eax
	movl	array2(,%eax,4), %edx
	movl	$.LC2, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$1, 28(%esp)
.L22:
	cmpl	$5, 28(%esp)
	jle	.L23
	movl	$10, (%esp)
	call	putchar
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20120305 (Red Hat 4.4.6-4)"
	.section	.note.GNU-stack,"",@progbits
