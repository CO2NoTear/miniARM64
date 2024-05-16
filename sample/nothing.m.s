  .arch armv8-a
  .text
  .global main
main:
	.data	.section .rodata // Read-only data section

	# label main
main:

	# begin

	# var i

	# i = 2
	ldr x0,=2

	# end
	LOD R3,(R2+8)
	LOD R2,(R2)
	JMP R3
