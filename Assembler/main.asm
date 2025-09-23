/*
	32BitCpu_t3::2025
	AssemblerV4
*/

;---------tamamlandi---------LOAD
;LOAD r0,0xf
;LOAD r1,@ff
;LOAD r1,@r2
;LOAD r1,@ff+r1

;---------tamamlandi---------
;STR @ff,r1
;STR @r2,r1
;STR @ff+r2,r1

;---------tamamlandi---------MOV
;MOV r4,r4

;---------tamamlandi---------
;add r1,0x5
;add r2,@ff+r2
;add r1,r2
;add r2,@r1
;add r1,@ff+r1
;sub,mul,div

;---------tamamlandi--------- AND
;and r1,r2
;or r1,r2
;xor r1,r2
;shl r1,r2
;shr r1,r2
;cmp r1,r2
;cmp r1,@r2
;cmp r1,@ff



;---------tamamlandi--------- PUSH
;push r1
;push @r1
;push @ff
;push @ff+r2

;---------tamamlandi---------POP
;pop

;---------tamamlandi---------JMP
/*
load r0,0xff

loop:
	load r0,0xff

	jmp loop
	jaz loop
	jlz loop
	jgz loop
	jsc loop
	juc loop
	jct loop
	jcf loop
*/
