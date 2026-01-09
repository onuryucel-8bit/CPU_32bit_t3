CLOSEDEB

;stack = 0x200
load r7,0x200


kwait
;r1 = input
mov r1,r0	

kwait	
;r1 = r1 + input
add r1,r0	

;while(1)
loop:
jmp loop


;i
.origin 0x100

;ISR-keyboard
FUNC readchar
	
	;r0 = klavye
	load r0,@fffffff7
	;tty = r0
	str @ffffffff,r0

	RET


