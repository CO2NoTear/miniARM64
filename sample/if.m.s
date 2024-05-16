  .arch armv8-a
  .text

	.data  .global k
  .type k, %object
  .size k, 4
k:
  .word 0
	.section .rodata
// Read-only data section
str1:
	.asciz "%d != %d"
str2:
	.asciz "%d == %d"
str5:
	.asciz "%d\n"

	# var k

	# label main
	.text
	.global main
main:

	# begin
  add sp, sp, #-32
	stp x29, x30, [sp]
	# var i

	# var j

	# i = 123
	ldr x0,=123

	# j = 222
	ldr x1,=222

	# k = 4
	ldr x2,=4

	# var t0

	# t0 = (i != j)
	str x0,[sp, 16]
	cmp x0,x1
	beq cmp_a0
	mov x0,1
	b cmp_b0
cmp_a0:	mov x0,0
cmp_b0:
	# ifz t0 goto L3
	str x1,[sp, 8]
	ldr x11, k
	str x2,[x11]
	cbz x0, L3

	# actual L1
	adr x0,str1

	# call (null)
  bl (null)
	# actual i
	ldr x0,[sp, 16]

	# call (null)
  bl (null)
	# actual j
	ldr x0,[sp, 8]

	# call (null)
  bl (null)
	# goto L4
	b L4

	# label L3
	.text
	.global L3
L3:

	# actual L2
	adr x0,str2

	# call (null)
  bl (null)
	# actual i
	ldr x0,[sp, 16]

	# call (null)
  bl (null)
	# actual j
	ldr x0,[sp, 8]

	# call (null)
  bl (null)
	# label L4
	.text
	.global L4
L4:

	# i = 999
	ldr x0,=999

	# actual L5
	str x0,[sp, 16]
	adr x0,str5

	# call (null)
  bl (null)
	# actual i
	ldr x0,[sp, 16]

	# call (null)
  bl (null)
	# end
  ldp x29, x30, [sp]
  add sp, sp, #32
