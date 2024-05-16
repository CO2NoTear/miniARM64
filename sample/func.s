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

	# label printf
	.text
	.global printf
printf:

	# begin
  add sp, sp, #-16
	stp x29, x30, [sp]
	# end
  ldp x29, x30, [sp]
  add sp, sp, #16
  ret
.size printf, .-printf

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

	# l = 1
	ldr x0,=1

	# m = 2
	ldr x1,=2

	# n = 3
	ldr x2,=3

	# actual L1
	str x0,[sp, 16]
	adr x0,str1

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
	# actual l
	ldr x0,[sp, 16]

	# t0 = call func
  bl func
	# n = t0

	# actual L1
	str x0,[sp, 32]
	adr x0,str1

	# actual i
	adr x1, i
	ldr x1, [x1]

	# actual j
	adr x2, j
	ldr x2, [x2]

	# actual k
	adr x3, k
	ldr x3, [x3]

	# call printf
  bl printf
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
  add sp, sp, #-16
	stp x29, x30, [sp]
	# formal o

	# formal p

	# formal q

	# actual L1
	adr x0,str1

	# actual o
	ldr x1,[sp, 8]

	# actual p
	ldr x2,[sp]

	# actual q
	ldr x3,[sp, -8]

	# call printf
  bl printf
	# i = o
	ldr x0,[sp, 8]

	# j = p
	ldr x1,[sp]

	# k = q
	ldr x2,[sp, -8]

	# actual L1
	ldr x0, i
	str x0,[x0]
	adr x0,str1

	# actual i
	ldr x0, j
	str x1,[x0]
	adr x1, i
	ldr x1, [x1]

	# actual j
	ldr x0, k
	str x2,[x0]
	adr x2, j
	ldr x2, [x2]

	# actual k
	adr x3, k
	ldr x3, [x3]

	# call printf
  bl printf
	# return 999
	ldr x0,=999
  ldp x29, x30, [sp]
  add sp, sp, #16
  ret
.size func, .-func

	# end
  ldp x29, x30, [sp]
  add sp, sp, #16
  ret
.size func, .-func
