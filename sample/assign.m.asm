label main
begin
var i
var j
var k
var l
var m
i = 8
var t0
t0 = i + 2
j = t0
var t1
t1 = i - 3
k = t1
var t2
t2 = i * 2
l = t2
var t3
t3 = i / 2
m = t3
actual L1
call PRINTS
actual i
call PRINTN
actual L2
call PRINTS
actual L3
call PRINTS
actual j
call PRINTN
actual L2
call PRINTS
actual L4
call PRINTS
actual k
call PRINTN
actual L2
call PRINTS
actual L5
call PRINTS
actual l
call PRINTN
actual L2
call PRINTS
actual L6
call PRINTS
actual m
call PRINTN
actual L2
call PRINTS
end
label main
begin
var i
var j
var k
var l
var m
i = 8
j = 10
k = 5
l = 16
m = 4
actual L1
call PRINTS
actual i
call PRINTN
actual L2
call PRINTS
actual L3
call PRINTS
actual j
call PRINTN
actual L2
call PRINTS
actual L4
call PRINTS
actual k
call PRINTN
actual L2
call PRINTS
actual L5
call PRINTS
actual l
call PRINTN
actual L2
call PRINTS
actual L6
call PRINTS
actual m
call PRINTN
actual L2
call PRINTS
end
	# head
	LOD R2,STACK
	STO (R2),0
	LOD R4,EXIT
	STO (R2+4),R4

	# label main
main:

	# begin

	# var i

	# var j

	# var k

	# var l

	# var m

	# i = 8
	LOD R5,8

	# j = 10
	LOD R6,10

	# k = 5
	LOD R7,5

	# l = 16
	LOD R8,16

	# m = 4
	LOD R9,4

	# actual L1
	LOD R10,L1
	STO (R2+28),R10

	# call PRINTS
	STO (R2+8),R5
	STO (R2+12),R6
	STO (R2+16),R7
	STO (R2+20),R8
	STO (R2+24),R9
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual i
	LOD R5,(R2+8)
	STO (R2+28),R5

	# call PRINTN
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTN

	# actual L2
	LOD R5,L2
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual L3
	LOD R5,L3
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual j
	LOD R5,(R2+12)
	STO (R2+28),R5

	# call PRINTN
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTN

	# actual L2
	LOD R5,L2
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual L4
	LOD R5,L4
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual k
	LOD R5,(R2+16)
	STO (R2+28),R5

	# call PRINTN
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTN

	# actual L2
	LOD R5,L2
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual L5
	LOD R5,L5
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual l
	LOD R5,(R2+20)
	STO (R2+28),R5

	# call PRINTN
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTN

	# actual L2
	LOD R5,L2
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual L6
	LOD R5,L6
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# actual m
	LOD R5,(R2+24)
	STO (R2+28),R5

	# call PRINTN
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTN

	# actual L2
	LOD R5,L2
	STO (R2+28),R5

	# call PRINTS
	STO (R2+32),R2
	LOD R4,R1+32
	STO (R2+36),R4
	LOD R2,R2+32
	JMP PRINTS

	# end
	LOD R3,(R2+4)
	LOD R2,(R2)
	JMP R3

PRINTN:
	LOD R7,(R2-4) # 789
	LOD R15,R7 # 789 
	DIV R7,10 # 78
	TST R7
	JEZ PRINTDIGIT
	LOD R8,R7 # 78
	MUL R8,10 # 780
	SUB R15,R8 # 9
	STO (R2+8),R15 # local 9 store

	# out 78
	STO (R2+12),R7 # actual 78 push

	# call PRINTN
	STO (R2+16),R2
	LOD R4,R1+32
	STO (R2+20),R4
	LOD R2,R2+16
	JMP PRINTN

	# out 9
	LOD R15,(R2+8) # local 9 

PRINTDIGIT:
	ADD  R15,48
	OUT

	# ret
	LOD R3,(R2+4)
	LOD R2,(R2)
	JMP R3

PRINTS:
	LOD R7,(R2-4)

PRINTC:
	LOD R15,(R7)
	DIV R15,16777216
	TST R15
	JEZ PRINTSEND
	OUT
	ADD R7,1
	JMP PRINTC

PRINTSEND:
	# ret
	LOD R3,(R2+4)
	LOD R2,(R2)
	JMP R3

EXIT:
	END

L6:
	DBS 109,61,0
L5:
	DBS 108,61,0
L4:
	DBS 107,61,0
L3:
	DBS 106,61,0
L2:
	DBS 10,0
L1:
	DBS 105,61,0
STATIC:
	DBN 0,0
STACK:
