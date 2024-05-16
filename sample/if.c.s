	.arch armv8-a
	.file	"if.c"
	.text

	.global	test
	.data
	.align	2
	.type	test, %object
	.size	test, 4
test:
	.word	1

	.global	past
	.align	2
	.type	past, %object
	.size	past, 4
past:
	.word	2
	
	.section	.rodata
	.align	3
.LC0:
	.string	"%d != %d\n"
	.align	3
.LC1:
	.string	"%d == %d\n"
	.align	3
.LC2:
	.string	"%d\n"

	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	mov	w0, 123
	str	w0, [sp, 28]
	mov	w0, 222
	str	w0, [sp, 24]
	ldr	w1, [sp, 28]
	ldr	w0, [sp, 24]
	cmp	w1, w0
	beq	.L2
	ldr	w2, [sp, 24]
	ldr	w1, [sp, 28]
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	printf
	b	.L3
.L2:
	ldr	w2, [sp, 24]
	ldr	w1, [sp, 28]
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	printf
.L3:
	mov	w0, 999
	str	w0, [sp, 28]
	ldr	w1, [sp, 28]
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	printf
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
