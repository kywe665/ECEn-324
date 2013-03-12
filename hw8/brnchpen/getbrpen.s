	.file	"getbrpen.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"cycle: "
.LC1:
	.string	"%u "
	.text
	.globl	dumpcycle
	.type	dumpcycle, @function
dumpcycle:
.LFB34:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	cmpl	$0, cindex(%rip)
	jle	.L2
	movl	$0, %ebx
.L3:
	movl	cycle(,%rbx,4), %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	leal	1(%rbx), %eax
	addq	$1, %rbx
	cmpl	%eax, cindex(%rip)
	jg	.L3
.L2:
	movl	$10, %edi
	call	putchar
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE34:
	.size	dumpcycle, .-dumpcycle
	.globl	sortcycle
	.type	sortcycle, @function
sortcycle:
.LFB35:
	.cfi_startproc
	movl	cindex(%rip), %edi
	movl	$1, %r8d
	movl	$1, %eax
	jmp	.L11
.L8:
	leal	-1(%rax), %ecx
	movslq	%ecx, %rdx
	movl	cycle(,%rdx,4), %esi
	movslq	%eax, %rdx
	movl	cycle(,%rdx,4), %edx
	cmpl	%edx, %esi
	jbe	.L7
	movslq	%eax, %r8
	movl	%esi, cycle(,%r8,4)
	movslq	%ecx, %rcx
	movl	%edx, cycle(,%rcx,4)
	movl	$0, %r8d
.L7:
	addl	$1, %eax
	jmp	.L11
.L10:
	movl	$1, %r8d
	movl	$1, %eax
.L11:
	cmpl	%edi, %eax
	jl	.L8
	testl	%r8d, %r8d
	je	.L10
	rep
	ret
	.cfi_endproc
.LFE35:
	.size	sortcycle, .-sortcycle
	.section	.rodata.str1.1
.LC2:
	.string	"Malloc failed\n"
	.text
	.globl	allocarrays
	.type	allocarrays, @function
allocarrays:
.LFB36:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leal	0(,%rdi,4), %ebx
	movslq	%ebx, %rbx
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, valA(%rip)
	testq	%rax, %rax
	jne	.L13
	movq	stderr(%rip), %rcx
	movl	$14, %edx
	movl	$1, %esi
	movl	$.LC2, %edi
	call	fwrite
	movl	$-1, %edi
	call	exit
.L13:
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, valB(%rip)
	testq	%rax, %rax
	jne	.L12
	movq	stderr(%rip), %rcx
	movl	$14, %edx
	movl	$1, %esi
	movl	$.LC2, %edi
	call	fwrite
	movl	$-1, %edi
	call	exit
.L12:
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE36:
	.size	allocarrays, .-allocarrays
	.globl	initarrays1
	.type	initarrays1, @function
initarrays1:
.LFB37:
	.cfi_startproc
	testl	%edi, %edi
	jle	.L15
	subl	$1, %edi
	leaq	4(,%rdi,4), %rcx
	movl	$0, %eax
.L17:
	movq	valA(%rip), %rdx
	movl	$0, (%rdx,%rax)
	movq	valB(%rip), %rdx
	movl	$10, (%rdx,%rax)
	addq	$4, %rax
	cmpq	%rcx, %rax
	jne	.L17
.L15:
	rep
	ret
	.cfi_endproc
.LFE37:
	.size	initarrays1, .-initarrays1
	.globl	initarrays2
	.type	initarrays2, @function
initarrays2:
.LFB38:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	%edi, %ebx
	movl	$0, %edi
	call	srand48
	testl	%ebx, %ebx
	jle	.L19
	subl	$1, %ebx
	leaq	4(,%rbx,4), %r12
	movl	$0, %ebp
.L21:
	movq	%rbp, %rbx
	addq	valA(%rip), %rbx
	call	mrand48
	movl	%eax, (%rbx)
	movq	%rbp, %rbx
	addq	valB(%rip), %rbx
	call	mrand48
	movl	%eax, (%rbx)
	addq	$4, %rbp
	cmpq	%r12, %rbp
	jne	.L21
.L19:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE38:
	.size	initarrays2, .-initarrays2
	.globl	clearlist
	.type	clearlist, @function
clearlist:
.LFB39:
	.cfi_startproc
	movl	$0, cindex(%rip)
	ret
	.cfi_endproc
.LFE39:
	.size	clearlist, .-clearlist
	.section	.rodata.str1.1
.LC3:
	.string	"Best time = %u\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"  Difference exceeds threshold: "
	.align 8
.LC7:
	.string	"third best = %u, diff: %.2f%%\n"
	.text
	.globl	testbest
	.type	testbest, @function
testbest:
.LFB40:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	cycle(%rip), %edx
	movl	$.LC3, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	movl	cycle+8(%rip), %eax
	cvtsi2sdq	%rax, %xmm0
	movl	cycle(%rip), %eax
	cvtsi2sdq	%rax, %xmm1
	movapd	%xmm0, %xmm2
	subsd	%xmm1, %xmm2
	movapd	%xmm2, %xmm1
	divsd	%xmm0, %xmm1
	ucomisd	.LC4(%rip), %xmm1
	jbe	.L24
	movl	$.LC5, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	movl	cycle+8(%rip), %edx
	movl	%edx, %eax
	cvtsi2sdq	%rax, %xmm1
	movl	cycle(%rip), %eax
	cvtsi2sdq	%rax, %xmm0
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	mulsd	.LC6(%rip), %xmm0
	divsd	%xmm1, %xmm0
	movl	$.LC7, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.L24:
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE40:
	.size	testbest, .-testbest
	.section	.rodata.str1.8
	.align 8
.LC8:
	.string	"fatal error: overflowed cycle array\n"
	.text
	.globl	addtolist
	.type	addtolist, @function
addtolist:
.LFB41:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	cindex(%rip), %eax
	movslq	%eax, %rdx
	movl	%edi, cycle(,%rdx,4)
	addl	$1, %eax
	movl	%eax, cindex(%rip)
	cmpl	$20, %eax
	jle	.L37
	movq	stderr(%rip), %rcx
	movl	$36, %edx
	movl	$1, %esi
	movl	$.LC8, %edi
	call	fwrite
	movl	$-1, %edi
	call	exit
.L37:
	call	sortcycle
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE41:
	.size	addtolist, .-addtolist
	.globl	absdiff
	.type	absdiff, @function
absdiff:
.LFB42:
	.cfi_startproc
	movl	%edi, %eax
	subl	%esi, %eax
	movl	%esi, %edx
	subl	%edi, %edx
	cmpl	%esi, %edi
	cmovle	%edx, %eax
	ret
	.cfi_endproc
.LFE42:
	.size	absdiff, .-absdiff
	.globl	measurecnt
	.type	measurecnt, @function
measurecnt:
.LFB43:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movl	$0, 12(%rsp)
	movl	$1, %esi
	movl	$0, %edi
	call	absdiff
	movl	%eax, %ebx
	movl	$1, %ebp
.L44:
	movl	12(%rsp), %eax
	addl	%ebx, %eax
	movl	%eax, 12(%rsp)
	call	start_counter
	testl	%ebp, %ebp
	jle	.L42
	leal	-1(%rbp), %eax
.L43:
	movl	12(%rsp), %edx
	addl	%ebx, %edx
	movl	%edx, 12(%rsp)
	subl	$1, %eax
	cmpl	$-1, %eax
	jne	.L43
.L42:
	call	get_counter
	addl	%ebp, %ebp
	cmpl	$999999, %eax
	jbe	.L44
	movl	%ebp, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE43:
	.size	measurecnt, .-measurecnt
	.globl	measure
	.type	measure, @function
measure:
.LFB44:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	movl	%edi, %r13d
	movl	$0, 12(%rsp)
	movl	$1, %esi
	movl	$0, %edi
	call	absdiff
	movl	12(%rsp), %edx
	addl	%edx, %eax
	movl	%eax, 12(%rsp)
	call	start_counter
	testl	%r13d, %r13d
	jle	.L48
	movq	valB(%rip), %rbp
	movq	valA(%rip), %r12
	movl	$0, %ebx
.L49:
	movl	0(%rbp,%rbx,4), %esi
	movl	(%r12,%rbx,4), %edi
	call	absdiff
	movl	12(%rsp), %edx
	addl	%edx, %eax
	movl	%eax, 12(%rsp)
	addq	$1, %rbx
	cmpl	%ebx, %r13d
	jg	.L49
.L48:
	call	get_counter
	movl	$0, %edx
	divl	%r13d
	addq	$24, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE44:
	.size	measure, .-measure
	.section	.rodata.str1.8
	.align 8
.LC9:
	.string	"Measuring using %d iterations\n"
	.align 8
.LC10:
	.string	"Cycles per function call, predictable branches"
	.align 8
.LC11:
	.string	"Cycles per function call, unpredictable branches"
	.text
	.globl	main
	.type	main, @function
main:
.LFB45:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	measurecnt
	movl	%eax, %ebx
	movl	%eax, %edx
	movl	$.LC9, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	movl	%ebx, %edi
	call	allocarrays
	movl	%ebx, %edi
	call	initarrays1
	call	clearlist
	movl	$20, %ebp
.L52:
	movl	%ebx, %edi
	call	measure
	movl	%eax, %edi
	call	addtolist
	subl	$1, %ebp
	jne	.L52
	movl	$.LC10, %edi
	call	puts
	movl	$0, %eax
	call	testbest
	movl	%ebx, %edi
	call	initarrays2
	call	clearlist
	movl	$20, %ebp
.L53:
	movl	%ebx, %edi
	call	measure
	movl	%eax, %edi
	call	addtolist
	subl	$1, %ebp
	jne	.L53
	movl	$.LC11, %edi
	call	puts
	movl	$0, %eax
	call	testbest
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE45:
	.size	main, .-main
	.comm	valB,8,8
	.comm	valA,8,8
	.comm	cindex,4,4
	.comm	cycle,80,32
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC4:
	.long	1202590843
	.long	1064598241
	.align 8
.LC6:
	.long	0
	.long	1079574528
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
