
LOAD r0,0x2a	
;r0 = '*'
LOAD r1,0x0		;i = 0
LOAD r3,0x0A	;r3 = '\n'

loop:
		
	LOAD r2,0x0		;j = 0
	loopj:		
		STR @FFFFFFFF,r0	;print r0

		ADD r2,0x1		;j++
		CMP r2,0x5		;if(j != 0x5)jump
	JNE loopj
	
	STR @FFFFFFFF,r3	;print \n

	ADD r1,0x1			;i++
	CMP r1,0x05		;if(i != 0x20) jump
JNE loop
