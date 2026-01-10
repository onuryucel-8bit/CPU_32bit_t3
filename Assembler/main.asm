CLOSEDEB

load r7,0x500

os:
	;print os str
	
	load r0, 0x160	; index
	; char
	load r1, @160	

	CALL print

	;check "R" run
	kwait
	CMP r0, 0x52
	JNE os

	adr:
		;if( key == enter) break;
		kwait		
		CMP r0, 0x0A	;line feed "enter" lf
		JE runprog		;break
		
		;convert r1 to ascii
		;r1 ascii => r2 int
		MOV r1, r0		;r1 = key
		CALL asciiToInt		
	jmp adr

	;r2 jmp adr
	runprog:
		CALL @r2
	
	load r2,0x0
	load r1,0x0
	load r0,0x0
	
jmp os

.origin 0x30
;save system[]
func saveSystem
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;73 61 76 65 20 73 79 73 74 65 6d
	;print("\nsave system\n")
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	load r0, 0x177	; index
	; char
	load r1, @177
	CALL print
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	;while(1)
	;{
	;	if(input == key_ENTER)	
	;		break
	;
	;	adr = asciiToInt(input)
	;} 
	;kayit noktasi
	adres:
		kwait
		CMP r0, 0x0A 	;lf line feed 'enter'
		JE prt	;break
		CALL asciiToInt
	jmp adres
	
	prt:
			
	load r0, 0x182	; index
	; char
	load r1, @182
	CALL print						
		
ret


;asciiToInt(ascii r1) => int r2
func asciiToInt
	SUB r1, 0x30 ;char c - '0' => INT
	;sayi = 10 * sayi + rakam
	MUL r2, 0x0a
	ADD r2, r1
ret


FUNC print
	printloop:
		str @ffffffff,r1
		;index++
		add r0,0x1	
		;char = ram[index]
		load r1, @r0 

		;if char != \0 jump
		cmp r1,0x0	
	jne printloop
	
RET

;i
.origin 0x150

;ISR-keyboard key => ascii r0
FUNC readchar
	
	load r0,@fffffff7
	str @ffffffff,r0

	RET


.origin 0x160
;os str
.db 0x44 ,0x45 ,0x4E ,0x49 ,0x5A ,0x20 ,0x41 ,0x4E ,0x41 ,0x53 ,0x49 ,0x20 ,0x21 ,0x20 ,0x4F ,0x53 ,0x20 ,0x76 ,0x30 ,0x2E,0x31,0x0a,0x0

;0x177 save system
,0x73 ,0x61 ,0x76 ,0x65 ,0x20 ,0x73 ,0x79 ,0x73 ,0x74 ,0x65 ,0x6d,0x0

;0X182
,0x45 ,0x58 ,0x49 ,0x54,0x0