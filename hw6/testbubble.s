	.file	"testbubble.c"
	.text
	.p2align 4,,15
.globl bubble_a
	.type	bubble_a, @function
bubble_a:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	movl	12(%ebp), %esi
	pushl	%ebx
	movl	8(%ebp), %edx
	subl	$1, %esi
	testl	%esi, %esi
	jle	.L6
	.p2align 4,,7
	.p2align 3
.L8:
	xorl	%eax, %eax
	.p2align 4,,7
	.p2align 3
.L5:
	movl	4(%edx,%eax,4), %ecx
	movl	(%edx,%eax,4), %ebx
	cmpl	%ebx, %ecx
	jge	.L4
	movl	%ebx, 4(%edx,%eax,4)
	movl	%ecx, (%edx,%eax,4)
.L4:
	addl	$1, %eax
	cmpl	%esi, %eax
	jl	.L5
	subl	$1, %esi
	jne	.L8
.L6:
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
	.size	bubble_a, .-bubble_a
	.p2align 4,,15
.globl bubble_p
	.type	bubble_p, @function
bubble_p:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	movl	12(%ebp), %esi
	pushl	%ebx
	movl	8(%ebp), %edx
	subl	$1, %esi
	testl	%esi, %esi
	jle	.L17
	.p2align 4,,7
	.p2align 3
.L12:
	xorl	%eax, %eax
	testl	%esi, %esi
	jle	.L16
	.p2align 4,,7
	.p2align 3
.L18:
	movl	(%edx,%eax,4), %ecx
	movl	4(%edx,%eax,4), %ebx
	cmpl	%ebx, %ecx
	jle	.L13
	movl	%ebx, (%edx,%eax,4)
	movl	%ecx, 4(%edx,%eax,4)
.L13:
	addl	$1, %eax
	cmpl	%eax, %esi
	jne	.L18
.L16:
	subl	$1, %esi
	jne	.L12
.L17:
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
	.size	bubble_p, .-bubble_p
	.section	.rodata.str1.1,"aMS",@progbits,1
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
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	pushl	%ebx
	xorl	%ebx, %ebx
	subl	$28, %esp
	movl	$.LC0, (%esp)
	call	puts
	movl	$.LC1, (%esp)
	call	printf
	.p2align 4,,7
	.p2align 3
.L21:
	movl	array1(,%ebx,4), %eax
	addl	$1, %ebx
	movl	$.LC2, (%esp)
	movl	%eax, 4(%esp)
	call	printf
	cmpl	$6, %ebx
	jne	.L21
	movl	$10, (%esp)
	xorb	%bl, %bl
	call	putchar
	movl	$6, 4(%esp)
	movl	$array1, (%esp)
	call	bubble_a
	movl	$.LC3, (%esp)
	call	printf
	.p2align 4,,7
	.p2align 3
.L22:
	movl	array1(,%ebx,4), %eax
	addl	$1, %ebx
	movl	$.LC2, (%esp)
	movl	%eax, 4(%esp)
	call	printf
	cmpl	$6, %ebx
	jne	.L22
	movl	$10, (%esp)
	xorb	%bl, %bl
	call	putchar
	movl	$.LC4, (%esp)
	call	puts
	movl	$.LC1, (%esp)
	call	printf
	.p2align 4,,7
	.p2align 3
.L23:
	movl	array2(,%ebx,4), %eax
	addl	$1, %ebx
	movl	$.LC2, (%esp)
	movl	%eax, 4(%esp)
	call	printf
	cmpl	$6, %ebx
	jne	.L23
	movl	$10, (%esp)
	xorb	%bl, %bl
	call	putchar
	movl	$6, 4(%esp)
	movl	$array2, (%esp)
	call	bubble_p
	movl	$.LC3, (%esp)
	call	printf
	.p2align 4,,7
	.p2align 3
.L24:
	movl	array2(,%ebx,4), %eax
	addl	$1, %ebx
	movl	$.LC2, (%esp)
	movl	%eax, 4(%esp)
	call	printf
	cmpl	$6, %ebx
	jne	.L24
	movl	$10, (%esp)
	call	putchar
	addl	$28, %esp
	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret
	.size	main, .-main
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
	.ident	"GCC: (GNU) 4.4.6 20120305 (Red Hat 4.4.6-4)"
	.section	.note.GNU-stack,"",@progbits
