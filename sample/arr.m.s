  .arch armv8-a
  .text

	.data
	.section .rodata
// Read-only data section
str2:
	.asciz "a = %d\n"
str3:
	.asciz "b = %d\n"
str4:
	.asciz "c = %d\n"
str5:
	.asciz "d = %d\n"
str6:
	.asciz "arr[0] = %d\n"
str7:
	.asciz "arr[1] = %d\n"
str8:
	.asciz "arr[2] = %d\n"
str9:
	.asciz "arr[3] = %d\n"

	# label func
	.text
	.global func
func:

	# begin
  add sp, sp, #-32
	stp x29, x30, [sp]
	# formal a
  # formal b
  # var t0

	# var t0

	# t0 = a + b
	str x0,[sp, 16]
	add x0,x0,x1

	# return t0

	# end
  ldp x29, x30, [sp]
  add sp, sp, #32
  ret
.size func, .-func

	# label main
	str x1,[sp, 24]
	.text
	.global main
main:

	# begin
  add sp, sp, #-96
	stp x29, x30, [sp]
	# var arr[5]
  add x0, sp, 16

	# var a

	# var b

	# var c

	# var d

	# var z

	# arr[0]=0
	mov x10,x0
	ldr x0,=0
  str x0, [x10, #0]
	# arr[1]=1
	ldr x1,=1
  str x1, [x10, #8]
	# arr[2]=2
	ldr x2,=2
  str x2, [x10, #16]
	# arr[3]=3
	ldr x3,=3
  str x3, [x10, #24]
	# arr[4]=4
	ldr x4,=4
  str x4, [x10, #32]
	# a = 20
	ldr x5,=20

	# var t1

	# t1 = a + 2
	str x5,[sp, 56]
	ldr x6,=2
	add x5,x5,x6

	# b = t1

	# z = 'z'

	# c = 2
	ldr x8,=2

	# var t2

	# t2=arr[0]
	ldr x9,[sp, 16]
  ldr x9, [x10, #0]
	# var t3

	# t3=arr[1]
	ldr x11,[sp, 24]
  ldr x11, [x10, #8]
	# var t4

	# t4 = t2 + t3
	add x9,x9,x11

	# var t5

	# t5=arr[2]
	ldr x12,[sp, 32]
  ldr x12, [x10, #16]
	# var t6

	# t6 = t4 + t5
	add x9,x9,x12

	# var t7

	# t7 = t6 + c
	add x9,x9,x8

	# arr[3]=t7
  str x9, [x10, #24]
	# arr[4]=6
	ldr x13,=6
  str x13, [x10, #32]
	# var t8

	# t8 = b + 5
	str x5,[sp, 64]
	ldr x14,=5
	add x5,x5,x14

	# arr[1]=t8
  str x5, [x10, #8]
	# var t11

	# var t10

	# t10=arr[2]
	ldr x15,[sp, 32]
  ldr x15, [x10, #16]
	# var t9

	# t9=arr[1]
	ldr x16,[sp, 24]
  ldr x16, [x10, #8]
	# actual t9
	mov x0,x16

	# actual t10
	mov x1,x15

	# t11 = call func
	str x7,[sp, 88]
	str x8,[sp, 72]
  bl func
	# arr[2]=t11
  add x10, sp, 16
  str x0, [x10, #16]
	# var t12

	# t12 = b + c
	ldr x1,[sp, 64]
	ldr x2,[sp, 72]
	add x1,x1,x2

	# d = t12

	# var t13

	# t13 = - b
	ldr x3,=0
	ldr x4,[sp, 64]
	sub x3,x3,x4

	# var t14

	# t14 = t13 * c
	mul x3,x3,x2

	# var t15

	# t15 = t14 + d
	add x3,x3,x1

	# a = t15

	# var t16

	# t16 = (a < 0)
	str x3,[sp, 56]
	ldr x5,=0
	cmp x3,x5
	blt cmp_a0
	mov x3,0
	b cmp_b0
cmp_a0:	mov x3,1
cmp_b0:
	# ifz t16 goto L1
	str x1,[sp, 80]
	cbz x3, L1

	# var t17

	# t17 = - b
	ldr x6,=0
	sub x6,x6,x4

	# var t18

	# t18 = t17 * c
	mul x6,x6,x2

	# a = t18

	# label L1
	str x6,[sp, 56]
	.text
	.global L1
L1:

	# d = 999
	ldr x0,=999

	# actual L2
	str x0,[sp, 80]
	adrp x0, str2
	add x0, x0, :lo12:str2

	# actual a
	ldr x1,[sp, 56]

	# call printf
  bl printf
	# actual L3
	adrp x0, str3
	add x0, x0, :lo12:str3

	# actual b
	ldr x1,[sp, 64]

	# call printf
  bl printf
	# actual L4
	adrp x0, str4
	add x0, x0, :lo12:str4

	# actual c
	ldr x1,[sp, 72]

	# call printf
  bl printf
	# actual L5
	adrp x0, str5
	add x0, x0, :lo12:str5

	# actual d
	ldr x1,[sp, 80]

	# call printf
  bl printf
	# var t19

	# t19=arr[0]
  add x10, sp, 16
	ldr x0,[sp, 16]
  ldr x0, [x10, #0]
	# actual L6
	adrp x0, str6
	add x0, x0, :lo12:str6

	# actual t19
	ldr x1,[sp, 16]

	# call printf
  bl printf
	# var t20

	# t20=arr[1]
  add x10, sp, 16
	ldr x0,[sp, 24]
  ldr x0, [x10, #8]
	# actual L7
	adrp x0, str7
	add x0, x0, :lo12:str7

	# actual t20
	ldr x1,[sp, 24]

	# call printf
  bl printf
	# var t21

	# t21=arr[2]
  add x10, sp, 16
	ldr x0,[sp, 32]
  ldr x0, [x10, #16]
	# actual L8
	adrp x0, str8
	add x0, x0, :lo12:str8

	# actual t21
	ldr x1,[sp, 32]

	# call printf
  bl printf
	# var t22

	# t22=arr[3]
  add x10, sp, 16
	ldr x0,[sp, 40]
  ldr x0, [x10, #24]
	# actual L9
	adrp x0, str9
	add x0, x0, :lo12:str9

	# actual t22
	ldr x1,[sp, 40]

	# call printf
  bl printf
	# return 0
	ldr x0,=0

	# end
  ldp x29, x30, [sp]
  add sp, sp, #96
  ret
.size main, .-main
