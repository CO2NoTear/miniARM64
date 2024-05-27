  .arch armv8-a
  .text

	.data
	.section .rodata
// Read-only data section
str1:
	.asciz "%d\n"

	# label main
	.text
	.global main
main:

	# begin
  add sp, sp, #-32
	stp x29, x30, [sp]
	# var i

	# i = 1
	ldr x0,=1

	# label L2
	str x0,[sp, 16]
	.text
	.global L2
L2:

	# var t0

	# t0 = (i < 10)
	ldr x0,[sp, 16]
	ldr x1,=10
	cmp x0,x1
	blt cmp_a0
	mov x0,0
	b cmp_b0
cmp_a0:	mov x0,1
cmp_b0:
	# ifz t0 goto L3
	cbz x0, L3

	# actual L1
	adrp x0, str1
	add x0, x0, :lo12:str1

	# actual i
	ldr x1,[sp, 16]

	# call printf
  bl printf
	# var t1

	# t1 = i + 1
	ldr x0,[sp, 16]
	ldr x1,=1
	add x0,x0,x1

	# i = t1

	# goto L2
	str x0,[sp, 16]
	b L2

	# label L3
	.text
	.global L3
L3:

	# end
  ldp x29, x30, [sp]
  add sp, sp, #32
  ret
.size main, .-main
