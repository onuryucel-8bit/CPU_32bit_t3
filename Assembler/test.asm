#define ASCIISTART 0x10

load r1,ASCIISTART ;a = ascii[0x10]
load r0,0x10	   ;index = 0x10

;while(1)
drawLoop:
	str @10000000, r1 ;print(a[index])
	add r0,0x1  ;index++
	load r1,@r0 ;a = ascii[index]

	jmp drawLoop: 