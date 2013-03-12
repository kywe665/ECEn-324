	.text
	.align 4,0x90
.globl _sortcycle
_sortcycle:
LFB7:
	pushq	%rbp
LCFI0:
	movq	%rsp, %rbp
LCFI1:
	movq	_cindex@GOTPCREL(%rip), %rax
	movl	(%rax), %r10d
	movl	$1, %r8d
	movl	$1, %r9d
	.align 4,0x90
L14:
	cmpl	%r10d, %r8d
	jl	L4
L16:
	testl	%r9d, %r9d
	jne	L10
	movl	$1, %r8d
	movb	$1, %r9b
	cmpl	%r10d, %r8d
	jge	L16
L4:
	leal	-1(%r8), %eax
	movq	_cycle@GOTPCREL(%rip), %rsi
	cltq
	movl	(%rsi,%rax,4), %edi
	movslq	%r8d,%rcx
	movl	(%rsi,%rcx,4), %edx
	cmpl	%edx, %edi
	jbe	L5
	movl	%edi, (%rsi,%rcx,4)
	movl	%edx, (%rsi,%rax,4)
	xorl	%r9d, %r9d
L5:
	incl	%r8d
	jmp	L14
L10:
	leave
	ret
LFE7:
	.align 4,0x90
.globl _initarrays1
_initarrays1:
LFB9:
	pushq	%rbp
LCFI2:
	movq	%rsp, %rbp
LCFI3:
	testl	%edi, %edi
	jle	L21
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movq	_valA@GOTPCREL(%rip), %r8
	movq	_valB@GOTPCREL(%rip), %rsi
	.align 4,0x90
L20:
	movq	(%r8), %rax
	movl	$0, (%rax,%rdx)
	movq	(%rsi), %rax
	movl	$10, (%rax,%rdx)
	incl	%ecx
	addq	$4, %rdx
	cmpl	%edi, %ecx
	jne	L20
L21:
	leave
	ret
LFE9:
	.align 4,0x90
.globl _clearlist
_clearlist:
LFB11:
	pushq	%rbp
LCFI4:
	movq	%rsp, %rbp
LCFI5:
	movq	_cindex@GOTPCREL(%rip), %rax
	movl	$0, (%rax)
	leave
	ret
LFE11:
	.align 4,0x90
.globl _absdiff
_absdiff:
LFB14:
	pushq	%rbp
LCFI6:
	movq	%rsp, %rbp
LCFI7:
	movl	%edi, %eax
	subl	%esi, %eax
	movl	%esi, %edx
	subl	%edi, %edx
	cmpl	%esi, %edi
	cmovle	%edx, %eax
	leave
	ret
LFE14:
	.cstring
LC0:
	.ascii "Best time = %u\12\0"
	.align 3
LC2:
	.ascii "  Difference exceeds threshold: \0"
	.align 3
LC4:
	.ascii "third best = %u, diff: %.2f%%\12\0"
	.text
	.align 4,0x90
.globl _testbest
_testbest:
LFB12:
	pushq	%rbp
LCFI8:
	movq	%rsp, %rbp
LCFI9:
	pushq	%rbx
LCFI10:
	subq	$8, %rsp
LCFI11:
	movq	_cycle@GOTPCREL(%rip), %rbx
	movl	(%rbx), %esi
	leaq	LC0(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	mov	8(%rbx), %eax
	cvtsi2sdq	%rax, %xmm1
	mov	(%rbx), %eax
	cvtsi2sdq	%rax, %xmm0
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	divsd	%xmm1, %xmm0
	ucomisd	LC1(%rip), %xmm0
	jbe	L42
	leaq	LC2(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	movl	8(%rbx), %esi
	mov	%esi, %eax
	cvtsi2sdq	%rax, %xmm1
	mov	(%rbx), %eax
	cvtsi2sdq	%rax, %xmm0
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	mulsd	LC3(%rip), %xmm0
	divsd	%xmm1, %xmm0
	leaq	LC4(%rip), %rdi
	movl	$1, %eax
	addq	$8, %rsp
	popq	%rbx
	leave
	jmp	_printf
	.align 4,0x90
L42:
	addq	$8, %rsp
	popq	%rbx
	leave
	ret
LFE12:
	.align 4,0x90
.globl _measure
_measure:
LFB16:
	pushq	%rbp
LCFI12:
	movq	%rsp, %rbp
LCFI13:
	pushq	%rbx
LCFI14:
	subq	$8, %rsp
LCFI15:
	movl	%edi, %ebx
	call	_start_counter
	call	_get_counter
	xorl	%edx, %edx
	divl	%ebx
	addq	$8, %rsp
	popq	%rbx
	leave
	ret
LFE16:
	.align 4,0x90
.globl _measurecnt
_measurecnt:
LFB15:
	pushq	%rbp
LCFI16:
	movq	%rsp, %rbp
LCFI17:
	pushq	%rbx
LCFI18:
	subq	$8, %rsp
LCFI19:
	movl	$1, %ebx
	.align 4,0x90
L46:
	call	_start_counter
	call	_get_counter
	addl	%ebx, %ebx
	cmpl	$999999, %eax
	jbe	L46
	movl	%ebx, %eax
	addq	$8, %rsp
	popq	%rbx
	leave
	ret
LFE15:
	.cstring
	.align 3
LC5:
	.ascii "fatal error: overflowed cycle array\12\0"
	.text
	.align 4,0x90
.globl _addtolist
_addtolist:
LFB13:
	pushq	%rbp
LCFI20:
	movq	%rsp, %rbp
LCFI21:
	movq	_cindex@GOTPCREL(%rip), %rcx
	movl	(%rcx), %edx
	movq	_cycle@GOTPCREL(%rip), %r10
	movslq	%edx,%rax
	movl	%edi, (%r10,%rax,4)
	incl	%edx
	movl	%edx, (%rcx)
	movl	$1, %r9d
	movl	$1, %r8d
	cmpl	$20, %edx
	jg	L66
	.align 4,0x90
L65:
	cmpl	%edx, %r8d
	jl	L55
L67:
	testl	%r9d, %r9d
	jne	L61
	movb	$1, %r9b
	movl	$1, %r8d
	cmpl	%edx, %r8d
	jge	L67
L55:
	leal	-1(%r8), %eax
	cltq
	movl	(%r10,%rax,4), %edi
	movslq	%r8d,%rsi
	movl	(%r10,%rsi,4), %ecx
	cmpl	%ecx, %edi
	jbe	L56
	movl	%edi, (%r10,%rsi,4)
	movl	%ecx, (%r10,%rax,4)
	xorl	%r9d, %r9d
L56:
	incl	%r8d
	jmp	L65
L61:
	leave
	ret
L66:
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	(%rax), %rcx
	movl	$36, %edx
	movl	$1, %esi
	leaq	LC5(%rip), %rdi
	call	_fwrite
	movl	$-1, %edi
	call	_exit
LFE13:
	.align 4,0x90
.globl _initarrays2
_initarrays2:
LFB10:
	pushq	%rbp
LCFI22:
	movq	%rsp, %rbp
LCFI23:
	pushq	%r15
LCFI24:
	pushq	%r14
LCFI25:
	pushq	%r13
LCFI26:
	pushq	%r12
LCFI27:
	pushq	%rbx
LCFI28:
	subq	$8, %rsp
LCFI29:
	movl	%edi, %r14d
	xorl	%edi, %edi
	call	_srand48
	testl	%r14d, %r14d
	jle	L72
	xorl	%r13d, %r13d
	xorl	%r12d, %r12d
	movq	_valB@GOTPCREL(%rip), %r15
	.align 4,0x90
L71:
	movq	%r12, %rbx
	movq	_valA@GOTPCREL(%rip), %rax
	addq	(%rax), %rbx
	call	_mrand48
	movl	%eax, (%rbx)
	movq	%r12, %rbx
	addq	(%r15), %rbx
	call	_mrand48
	movl	%eax, (%rbx)
	incl	%r13d
	addq	$4, %r12
	cmpl	%r14d, %r13d
	jne	L71
L72:
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	leave
	ret
LFE10:
	.cstring
LC6:
	.ascii "Malloc failed\12\0"
	.text
	.align 4,0x90
.globl _allocarrays
_allocarrays:
LFB8:
	pushq	%rbp
LCFI30:
	movq	%rsp, %rbp
LCFI31:
	pushq	%rbx
LCFI32:
	subq	$8, %rsp
LCFI33:
	movslq	%edi,%rdi
	salq	$2, %rdi
	movslq	%edi,%rbx
	movq	%rbx, %rdi
	call	_malloc
	movq	_valA@GOTPCREL(%rip), %rdx
	movq	%rax, (%rdx)
	testq	%rax, %rax
	je	L80
	movq	%rbx, %rdi
	call	_malloc
	movq	_valB@GOTPCREL(%rip), %rdx
	movq	%rax, (%rdx)
	testq	%rax, %rax
	je	L80
	addq	$8, %rsp
	popq	%rbx
	leave
	ret
L80:
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	(%rax), %rcx
	movl	$14, %edx
	movl	$1, %esi
	leaq	LC6(%rip), %rdi
	call	_fwrite
	movl	$-1, %edi
	call	_exit
LFE8:
	.cstring
LC7:
	.ascii "cycle: \0"
LC8:
	.ascii "%u \0"
	.text
	.align 4,0x90
.globl _dumpcycle
_dumpcycle:
LFB6:
	pushq	%rbp
LCFI34:
	movq	%rsp, %rbp
LCFI35:
	pushq	%r13
LCFI36:
	pushq	%r12
LCFI37:
	pushq	%rbx
LCFI38:
	subq	$8, %rsp
LCFI39:
	leaq	LC7(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	movq	_cindex@GOTPCREL(%rip), %r13
	movl	(%r13), %eax
	testl	%eax, %eax
	jle	L82
	xorl	%ebx, %ebx
	movq	_cycle@GOTPCREL(%rip), %r12
	.align 4,0x90
L84:
	movl	(%r12,%rbx,4), %esi
	leaq	LC8(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	leal	1(%rbx), %eax
	incq	%rbx
	cmpl	%eax, (%r13)
	jg	L84
L82:
	movl	$10, %edi
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	leave
	jmp	_putchar
LFE6:
	.cstring
	.align 3
LC9:
	.ascii "Measuring using %d iterations\12\0"
	.align 3
LC10:
	.ascii "Cycles per function call, predictable branches\0"
	.align 3
LC11:
	.ascii "Cycles per function call, unpredictable branches\0"
	.text
	.align 4,0x90
.globl _main
_main:
LFB17:
	pushq	%rbp
LCFI40:
	movq	%rsp, %rbp
LCFI41:
	pushq	%r15
LCFI42:
	pushq	%r14
LCFI43:
	pushq	%r13
LCFI44:
	pushq	%r12
LCFI45:
	pushq	%rbx
LCFI46:
	subq	$8, %rsp
LCFI47:
	movl	$1, %r15d
L88:
	call	_start_counter
	call	_get_counter
	addl	%r15d, %r15d
	cmpl	$999999, %eax
	jbe	L88
	movl	%r15d, %esi
	leaq	LC9(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	movl	%r15d, %edi
	call	_allocarrays
	testl	%r15d, %r15d
	jle	L90
	xorl	%ecx, %ecx
	xorl	%edx, %edx
L92:
	movq	_valA@GOTPCREL(%rip), %rbx
	movq	(%rbx), %rax
	movl	$0, (%rax,%rdx)
	movq	_valB@GOTPCREL(%rip), %rbx
	movq	(%rbx), %rax
	movl	$10, (%rax,%rdx)
	incl	%ecx
	addq	$4, %rdx
	cmpl	%r15d, %ecx
	jne	L92
L90:
	movq	_cindex@GOTPCREL(%rip), %rax
	movl	$0, (%rax)
	xorl	%ebx, %ebx
	movq	_cycle@GOTPCREL(%rip), %r12
L93:
	call	_start_counter
	call	_get_counter
	movq	_cindex@GOTPCREL(%rip), %rdx
	movl	(%rdx), %ecx
	movslq	%ecx,%rdi
	xorl	%edx, %edx
	divl	%r15d
	movl	%eax, (%r12,%rdi,4)
	incl	%ecx
	movq	_cindex@GOTPCREL(%rip), %rax
	movl	%ecx, (%rax)
	cmpl	$20, %ecx
	jg	L119
	movl	$1, %r9d
	movl	$1, %r8d
	.align 4,0x90
L156:
	cmpl	%ecx, %r8d
	jl	L97
L158:
	testl	%r9d, %r9d
	jne	L101
	movb	$1, %r9b
	movl	$1, %r8d
	cmpl	%ecx, %r8d
	jge	L158
L97:
	leal	-1(%r8), %eax
	cltq
	movl	(%r12,%rax,4), %edi
	movslq	%r8d,%rsi
	movl	(%r12,%rsi,4), %edx
	cmpl	%edx, %edi
	jbe	L98
	movl	%edi, (%r12,%rsi,4)
	movl	%edx, (%r12,%rax,4)
	xorl	%r9d, %r9d
L98:
	incl	%r8d
	jmp	L156
L101:
	incl	%ebx
	cmpl	$20, %ebx
	jne	L93
	leaq	LC10(%rip), %rdi
	call	_puts
	movl	(%r12), %esi
	leaq	LC0(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	mov	8(%r12), %eax
	cvtsi2sdq	%rax, %xmm1
	mov	(%r12), %eax
	cvtsi2sdq	%rax, %xmm0
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	divsd	%xmm1, %xmm0
	ucomisd	LC1(%rip), %xmm0
	ja	L159
L106:
	xorl	%edi, %edi
	call	_srand48
	testl	%r15d, %r15d
	jle	L115
	xorl	%r14d, %r14d
	xorl	%r13d, %r13d
L117:
	movq	%r13, %rbx
	movq	_valA@GOTPCREL(%rip), %rax
	addq	(%rax), %rbx
	call	_mrand48
	movl	%eax, (%rbx)
	movq	%r13, %rbx
	movq	_valB@GOTPCREL(%rip), %rdx
	addq	(%rdx), %rbx
	call	_mrand48
	movl	%eax, (%rbx)
	incl	%r14d
	addq	$4, %r13
	cmpl	%r15d, %r14d
	jne	L117
L115:
	movq	_cindex@GOTPCREL(%rip), %rbx
	movl	$0, (%rbx)
	xorl	%ebx, %ebx
L118:
	call	_start_counter
	call	_get_counter
	movq	_cindex@GOTPCREL(%rip), %rdx
	movl	(%rdx), %ecx
	movslq	%ecx,%rdi
	xorl	%edx, %edx
	divl	%r15d
	movl	%eax, (%r12,%rdi,4)
	incl	%ecx
	movq	_cindex@GOTPCREL(%rip), %rax
	movl	%ecx, (%rax)
	cmpl	$20, %ecx
	jg	L119
	movl	$1, %r9d
	movl	$1, %r8d
	.align 4,0x90
L157:
	cmpl	%ecx, %r8d
	jl	L122
L160:
	testl	%r9d, %r9d
	jne	L126
	movb	$1, %r9b
	movl	$1, %r8d
	cmpl	%ecx, %r8d
	jge	L160
L122:
	leal	-1(%r8), %eax
	cltq
	movl	(%r12,%rax,4), %edi
	movslq	%r8d,%rsi
	movl	(%r12,%rsi,4), %edx
	cmpl	%edx, %edi
	jbe	L123
	movl	%edi, (%r12,%rsi,4)
	movl	%edx, (%r12,%rax,4)
	xorl	%r9d, %r9d
L123:
	incl	%r8d
	jmp	L157
L126:
	incl	%ebx
	cmpl	$20, %ebx
	jne	L118
	leaq	LC11(%rip), %rdi
	call	_puts
	movl	(%r12), %esi
	leaq	LC0(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	mov	8(%r12), %eax
	cvtsi2sdq	%rax, %xmm1
	mov	(%r12), %eax
	cvtsi2sdq	%rax, %xmm0
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	divsd	%xmm1, %xmm0
	ucomisd	LC1(%rip), %xmm0
	jbe	L141
	leaq	LC2(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	movl	8(%r12), %esi
	mov	%esi, %eax
	cvtsi2sdq	%rax, %xmm1
	mov	(%r12), %eax
	cvtsi2sdq	%rax, %xmm0
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	mulsd	LC3(%rip), %xmm0
	divsd	%xmm1, %xmm0
	leaq	LC4(%rip), %rdi
	movl	$1, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	leave
	jmp	_printf
L159:
	leaq	LC2(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	movl	8(%r12), %esi
	mov	%esi, %eax
	cvtsi2sdq	%rax, %xmm1
	mov	(%r12), %eax
	cvtsi2sdq	%rax, %xmm0
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	mulsd	LC3(%rip), %xmm0
	divsd	%xmm1, %xmm0
	leaq	LC4(%rip), %rdi
	movl	$1, %eax
	call	_printf
	jmp	L106
L141:
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	leave
	ret
L119:
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	(%rax), %rcx
	movl	$36, %edx
	movl	$1, %esi
	leaq	LC5(%rip), %rdi
	call	_fwrite
	movl	$-1, %edi
	call	_exit
LFE17:
.comm _cycle,80,5
.comm _cindex,4,2
.comm _valA,8,3
.comm _valB,8,3
	.literal8
	.align 3
LC1:
	.long	1202590843
	.long	1064598241
	.align 3
LC3:
	.long	0
	.long	1079574528
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$0,LECIE1-LSCIE1
	.long L$set$0
LSCIE1:
	.long	0x0
	.byte	0x1
	.ascii "zR\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0x1
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE1:
.globl _sortcycle.eh
_sortcycle.eh:
LSFDE1:
	.set L$set$1,LEFDE1-LASFDE1
	.long L$set$1
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB7-.
	.set L$set$2,LFE7-LFB7
	.quad L$set$2
	.byte	0x0
	.byte	0x4
	.set L$set$3,LCFI0-LFB7
	.long L$set$3
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$4,LCFI1-LCFI0
	.long L$set$4
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE1:
.globl _initarrays1.eh
_initarrays1.eh:
LSFDE3:
	.set L$set$5,LEFDE3-LASFDE3
	.long L$set$5
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB9-.
	.set L$set$6,LFE9-LFB9
	.quad L$set$6
	.byte	0x0
	.byte	0x4
	.set L$set$7,LCFI2-LFB9
	.long L$set$7
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$8,LCFI3-LCFI2
	.long L$set$8
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE3:
.globl _clearlist.eh
_clearlist.eh:
LSFDE5:
	.set L$set$9,LEFDE5-LASFDE5
	.long L$set$9
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB11-.
	.set L$set$10,LFE11-LFB11
	.quad L$set$10
	.byte	0x0
	.byte	0x4
	.set L$set$11,LCFI4-LFB11
	.long L$set$11
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$12,LCFI5-LCFI4
	.long L$set$12
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE5:
.globl _absdiff.eh
_absdiff.eh:
LSFDE7:
	.set L$set$13,LEFDE7-LASFDE7
	.long L$set$13
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB14-.
	.set L$set$14,LFE14-LFB14
	.quad L$set$14
	.byte	0x0
	.byte	0x4
	.set L$set$15,LCFI6-LFB14
	.long L$set$15
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$16,LCFI7-LCFI6
	.long L$set$16
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE7:
.globl _testbest.eh
_testbest.eh:
LSFDE9:
	.set L$set$17,LEFDE9-LASFDE9
	.long L$set$17
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB12-.
	.set L$set$18,LFE12-LFB12
	.quad L$set$18
	.byte	0x0
	.byte	0x4
	.set L$set$19,LCFI8-LFB12
	.long L$set$19
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$20,LCFI9-LCFI8
	.long L$set$20
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$21,LCFI11-LCFI9
	.long L$set$21
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE9:
.globl _measure.eh
_measure.eh:
LSFDE11:
	.set L$set$22,LEFDE11-LASFDE11
	.long L$set$22
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB16-.
	.set L$set$23,LFE16-LFB16
	.quad L$set$23
	.byte	0x0
	.byte	0x4
	.set L$set$24,LCFI12-LFB16
	.long L$set$24
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$25,LCFI13-LCFI12
	.long L$set$25
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$26,LCFI15-LCFI13
	.long L$set$26
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE11:
.globl _measurecnt.eh
_measurecnt.eh:
LSFDE13:
	.set L$set$27,LEFDE13-LASFDE13
	.long L$set$27
LASFDE13:
	.long	LASFDE13-EH_frame1
	.quad	LFB15-.
	.set L$set$28,LFE15-LFB15
	.quad L$set$28
	.byte	0x0
	.byte	0x4
	.set L$set$29,LCFI16-LFB15
	.long L$set$29
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$30,LCFI17-LCFI16
	.long L$set$30
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$31,LCFI19-LCFI17
	.long L$set$31
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE13:
.globl _addtolist.eh
_addtolist.eh:
LSFDE15:
	.set L$set$32,LEFDE15-LASFDE15
	.long L$set$32
LASFDE15:
	.long	LASFDE15-EH_frame1
	.quad	LFB13-.
	.set L$set$33,LFE13-LFB13
	.quad L$set$33
	.byte	0x0
	.byte	0x4
	.set L$set$34,LCFI20-LFB13
	.long L$set$34
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$35,LCFI21-LCFI20
	.long L$set$35
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE15:
.globl _initarrays2.eh
_initarrays2.eh:
LSFDE17:
	.set L$set$36,LEFDE17-LASFDE17
	.long L$set$36
LASFDE17:
	.long	LASFDE17-EH_frame1
	.quad	LFB10-.
	.set L$set$37,LFE10-LFB10
	.quad L$set$37
	.byte	0x0
	.byte	0x4
	.set L$set$38,LCFI22-LFB10
	.long L$set$38
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$39,LCFI23-LCFI22
	.long L$set$39
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$40,LCFI29-LCFI23
	.long L$set$40
	.byte	0x83
	.byte	0x7
	.byte	0x8c
	.byte	0x6
	.byte	0x8d
	.byte	0x5
	.byte	0x8e
	.byte	0x4
	.byte	0x8f
	.byte	0x3
	.align 3
LEFDE17:
.globl _allocarrays.eh
_allocarrays.eh:
LSFDE19:
	.set L$set$41,LEFDE19-LASFDE19
	.long L$set$41
LASFDE19:
	.long	LASFDE19-EH_frame1
	.quad	LFB8-.
	.set L$set$42,LFE8-LFB8
	.quad L$set$42
	.byte	0x0
	.byte	0x4
	.set L$set$43,LCFI30-LFB8
	.long L$set$43
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$44,LCFI31-LCFI30
	.long L$set$44
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$45,LCFI33-LCFI31
	.long L$set$45
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE19:
.globl _dumpcycle.eh
_dumpcycle.eh:
LSFDE21:
	.set L$set$46,LEFDE21-LASFDE21
	.long L$set$46
LASFDE21:
	.long	LASFDE21-EH_frame1
	.quad	LFB6-.
	.set L$set$47,LFE6-LFB6
	.quad L$set$47
	.byte	0x0
	.byte	0x4
	.set L$set$48,LCFI34-LFB6
	.long L$set$48
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$49,LCFI35-LCFI34
	.long L$set$49
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$50,LCFI39-LCFI35
	.long L$set$50
	.byte	0x83
	.byte	0x5
	.byte	0x8c
	.byte	0x4
	.byte	0x8d
	.byte	0x3
	.align 3
LEFDE21:
.globl _main.eh
_main.eh:
LSFDE23:
	.set L$set$51,LEFDE23-LASFDE23
	.long L$set$51
LASFDE23:
	.long	LASFDE23-EH_frame1
	.quad	LFB17-.
	.set L$set$52,LFE17-LFB17
	.quad L$set$52
	.byte	0x0
	.byte	0x4
	.set L$set$53,LCFI40-LFB17
	.long L$set$53
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$54,LCFI41-LCFI40
	.long L$set$54
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$55,LCFI47-LCFI41
	.long L$set$55
	.byte	0x83
	.byte	0x7
	.byte	0x8c
	.byte	0x6
	.byte	0x8d
	.byte	0x5
	.byte	0x8e
	.byte	0x4
	.byte	0x8f
	.byte	0x3
	.align 3
LEFDE23:
	.subsections_via_symbols
