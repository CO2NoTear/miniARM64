  .arch armv8-a
  .text

	.data
  .global k
  .type k, %object
  .size k, 4
k:
  .word 0
  .global j
  .type j, %object
  .size j, 4
j:
  .word 0
  .global i
  .type i, %object
  .size i, 4
i:
  .word 0
	.section .rodata
// Read-only data section
str1:
	.asciz "%d, %d ,%d\n"

	# var i

	# var j

	# var k

	# label main
	.text
	.global main
main:

	# begin
  add sp, sp, #-48
	stp x29, x30, [sp]
	# var l

	# var m

	# var n

	# actual L1
	adrp x0, str1
	add x0, x0, :lo12:str1

	# actual i
	adrp x1, i
	add x1, x1, :lo12:i
  ldr x1, [x1]
	# actual j
	adrp x2, j
	add x2, x2, :lo12:j
  ldr x2, [x2]
	# actual k
	adrp x3, k
	add x3, x3, :lo12:k
  ldr x3, [x3]
	# call printf
  bl printf
	# l = 1
	ldr x0,=1

	# m = 2
	ldr x1,=2

	# n = 3
	ldr x2,=3

	# actual L1
	str x0,[sp, 16]
	adrp x0, str1
	add x0, x0, :lo12:str1

	# actual l
	str x1,[sp, 24]
	ldr x1,[sp, 16]

	# actual m
	str x2,[sp, 32]
	ldr x2,[sp, 24]

	# actual n
	ldr x3,[sp, 32]

	# call printf
  bl printf
	# var t0

	# actual l
	ldr x0,[sp, 16]

	# actual m
	ldr x1,[sp, 24]

	# actual n
	ldr x2,[sp, 32]

	# t0 = call func
  bl func
	# n = t0

	# actual L1
	str x0,[sp, 32]
	adrp x0, str1
	add x0, x0, :lo12:str1

	# actual l
	ldr x1,[sp, 16]

	# actual m
	ldr x2,[sp, 24]

	# actual n
	ldr x3,[sp, 32]

	# call printf
  bl printf
	# actual L1
	adrp x0, str1
	add x0, x0, :lo12:str1

	# actual i
	adrp x1, i
	add x1, x1, :lo12:i
  ldr x1, [x1]
	# actual j
	adrp x2, j
	add x2, x2, :lo12:j
  ldr x2, [x2]
	# actual k
	adrp x3, k
	add x3, x3, :lo12:k
  ldr x3, [x3]
	# call printf
  bl printf
	# return 0
	ldr x0,=0

	# end
  ldp x29, x30, [sp]
  add sp, sp, #48
  ret
.size main, .-main

	# label func
	.text
	.global func
func:

	# begin
  add sp, sp, #-48
	stp x29, x30, [sp]
	# formal o
  # formal p
  # formal q
  # actual L1

	# actual L1
	str x0,[sp, 16]
	adrp x0, str1
	add x0, x0, :lo12:str1

	# actual o
	str x1,[sp, 24]
	ldr x1,[sp, 16]

	# actual p
	str x2,[sp, 32]
	ldr x2,[sp, 24]

	# actual q
	ldr x3,[sp, 32]

	# call printf
  bl printf
	# i = o
	ldr x0,[sp, 16]

	# j = p
	ldr x1,[sp, 24]

	# k = q
	ldr x2,[sp, 32]

	# o = 4
	ldr x3,=4

	# p = 5
	ldr x4,=5

	# q = 6
	ldr x5,=6

	# actual L1
	ldr x0, i
	adrp x0, str1
	add x0, x0, :lo12:str1

	# actual i
	ldr x0, j
	adrp x1, i
	add x1, x1, :lo12:i
  ldr x1, [x1]
	# actual j
	ldr x0, k
	adrp x2, j
	add x2, x2, :lo12:j
  ldr x2, [x2]
	# actual k
	str x3,[sp, 16]
	adrp x3, k
	add x3, x3, :lo12:k
  ldr x3, [x3]
	# call printf
	str x4,[sp, 24]
	str x5,[sp, 32]
  bl printf
	# return 999
	ldr x0,=999

	# end
  ldp x29, x30, [sp]
  add sp, sp, #48
  ret
.size func, .-func
