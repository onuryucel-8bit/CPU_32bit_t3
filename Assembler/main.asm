/*
	32BitCpu_t3::2025
	AssemblerV4
*/

;LOAD r1,0xf
;LOAD r1,@ff

;hata var
;LOAD r1,@r2
;LOAD r1,@ff+r1

;???
;STR @ff,r1

;hata var
;STR @r2,r1
;STR @ff+r2,r1

;hata var
;add r1,r2

;add r1,0x5
;add r2,@ff

;hata var
;add r2,@r1

;hata var
;add r1,@ff+r1

;hata var
;and r1,r2

;hata var
;or r1,r2

;calisiyor
;push r1
;push @r1
;push @ff

;hata var
;push @ff+4

;calisiyor
;pop

