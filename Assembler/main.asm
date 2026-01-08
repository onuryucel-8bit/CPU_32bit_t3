CLOSEDEB

load r7,0x200

loop:
jmp loop


;i
.origin 0x100


FUNC readchar
	
	load r0,@fffffff7
	str @ffffffff,r0

	RET
