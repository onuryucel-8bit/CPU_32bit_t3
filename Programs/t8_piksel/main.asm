
;posx
load r1, 0x0
;posy
load r2, 0x5
;color
load r3, 0x3

load r7,0x200

loop:
	nop
	mwe
	nop
	nop
	;clear
	str @fffffff6,r0
		
	;posx
	str @fffffff9,r1
	;posy
	str @fffffff8,r2
	;color
	str @fffffffa,r3


jmp loop

.origin 0x150

;ISR-keyboard key => ascii r0
FUNC readchar
	
	load r0,@fffffff7
	str @ffffffff,r0

	;enter?
	cmp r0,0x0A
	jne continue
		add r1,0x1
		;r5 = 0 => r0
		mov r0,r5
	continue:

	RET