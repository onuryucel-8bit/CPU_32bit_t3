;---------------------------------
;---------------------------------
;---------------------------------

;char a = ascii[index];
;int index = 0;
;while(1)
;{
;	print(a);
;	index++;
;   a = ascii[index];
;}

;r0 = index
;r1 = char(a)
;---------------------------------
;---------------------------------
;---------------------------------

load r1,@20	 ;
load r0,0x20 ;index

;while(1)
drawLoop:
	str @10000000, r1 ;print(a[index])
	add r0,0x1  ;index++
	load r1,@r0 ;a ascii[index]

	jmp drawLoop: 

;data 'ascii[]'

.origin 0x20
#include "test.asm"
