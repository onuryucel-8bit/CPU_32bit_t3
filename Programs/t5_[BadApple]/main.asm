CLOSEDEB

;x
load r0,0x0		
;y
load r1,0x0	
;color
load r2,@40

;index = 0x40
load r3,0x40

;rlx
load r4,@40

;i = 0
load r5,0x0

;frame_i
load r6,0x0

;frame_no
load r7,0x0

loop:
	
	load r5,0x0 ;i = arr[rlx]
	add r3,0x1	;index++
	load r2,@r3 ;color = arr[index]

	drawLoop:				
	
		str @fffffff9,r0	;posx
		str @fffffff8,r1	;posy
		str @fffffffa,r2	;color
	
		add r0,0x1		;x++
		cmp r0,0x20		;x,32dec
	
		jne hrz			;if(x != 32) jump
			load r0,0x0	;x = 0
			add r1,0x1	;y++
		hrz:
		
		add r6,0x1	;frame_i++

		add r5,0x1	;i++
		cmp r5,r4
		;if (i != rlx) jump
	jne drawLoop

	add r3,0x1 ;index++
	load r4,@r3
	
	;rlx = arr[index]

	cmp r6,0x400
	jne framex
		add r7,0x1
		load r6,0x0
	framex:


jmp loop


.origin 0x40
.db 0x400,0x0,0x400,0x0,0x400,0x0,0x400,0x0,0x400,0x0,0x400,0x0,0x400,0x0,0x1F,0x0,
