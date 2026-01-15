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
	;print("\nsave system\n")
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	load r0, 0x173	; index
	; char
	load r1, @173
	CALL print
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	;while(1)
	;{
	;	if(input == key_ENTER)	
	;	{
	;		adr = asciiToInt(input())
	;		break;
	;	}
	;}
	;while(1)
	;{
	;	ram[adr] = input()
	;	adr++
	;	if(input == key_ENTER)
	;	{
	;		break;
	;	}
	;}
	
	;kayit noktasi
	adres:
		kwait
		CMP r0, 0x0A 	;lf line feed 'enter'
		JE dataEntry	;break
		CALL asciiToInt
	jmp adres
	
	
	dataEntry:			
		MOV r1,r2	;save adr = r1
	
	saveSystemloop:
		kwait
		CMP r0, 0x20
		JE saveSystemExit
		;ret r2
		CALL asciiToInt
	jmp saveSystemloop
	
	saveSystemExit:
		;print("exit")
		load r0, 0x17F	; index
		; char
		load r1, @17F
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

	load r1,0x0a
	str @ffffffff,r1
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
.db "DENIZ ANASI OS v01"

;0x173 save system
, "SAVE SYSTEM"

;0X17F
, "EXITING SAVE SYSTEM"