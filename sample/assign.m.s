  .arch armv8-a
  .text

	.data
	.section .rodata
// Read-only data section
str1:
	.asciz "i=%d\n"
str2:
	.asciz "j=%d\n"
str3:
	.asciz "k=%d\n"
str4:
	.asciz "l=%d\n"
str5:
	.asciz "m=%d\n"

	# label main
	.text
	.global main
main:

	# begin
  add sp, sp, #-64
	stp x29, x30, [sp]
	# var i

	# var j

	# var k

	# var l

	# var m

	# i = 8
	ldr x0,=8

	# var t0

	# t0 = i + 2
	str x0,[sp, 16]
	ldr x1,=2
	add x0,x0,x1

	# j = t0

	# var t1

	# t1 = i - 3
	ldr x2,[sp, 16]
	ldr x3,=3
	sub x2,x2,x3

	# k = t1

	# var t2

	# t2 = i * 2
	ldr x4,[sp, 16]
	ldr x5,=2
	mul x4,x4,x5

	# l = t2

	# var t3

	# t3 = i / 2
	ldr x6,[sp, 16]
	ldr x7,=2
	sdiv x6,x6,x7

	# m = t3

	# actual L1
	str x0,[sp, 24]
	adrp x0, str1
	add x0, x0, :lo12:str1

	# actual i
	ldr x1,[sp, 16]

	# call printf
	str x2,[sp, 32]
	str x4,[sp, 40]
	str x6,[sp, 48]
  bl printf
	# actual L2
	adrp x0, str2
	add x0, x0, :lo12:str2

	# actual j
	ldr x1,[sp, 24]

	# call printf
  bl printf
	# actual L3
	adrp x0, str3
	add x0, x0, :lo12:str3

	# actual k
	ldr x1,[sp, 32]

	# call printf
  bl printf
	# actual L4
	adrp x0, str4
	add x0, x0, :lo12:str4

	# actual l
	ldr x1,[sp, 40]

	# call printf
  bl printf
	# actual L5
	adrp x0, str5
	add x0, x0, :lo12:str5

	# actual m
	ldr x1,[sp, 48]

	# call printf
  bl printf
	# return 0
	ldr x0,=0

	# end
  ldp x29, x30, [sp]
  add sp, sp, #64
  ret
.size main, .-main
