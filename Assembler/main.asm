;CLOSEDEB

load r7,0x200

load r2,0x20

CALL @r2

.origin 0x20

FUNC adl
	load r0,0x2
	add r0,0x2
RET




;i
.origin 0x100

;ISR-keyboard
FUNC readchar
	
	load r0,@fffffff7
	str @ffffffff,r0

	RET


