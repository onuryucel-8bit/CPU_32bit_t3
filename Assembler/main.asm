CLOSEDEB

load r7,0x200

loop:
jmp loop


;i
.origin 0x100


FUNC readchar
	load r1,0x0
	readchar:

	load r0,@fffffff7
	str @ffffffff,r0

	add r1,0x1
	
	cmp r1,0x7
	jne readchar
	RET
