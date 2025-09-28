;1+2+3+4+5+ ... +n = n(n+1)/2
mov r1,r2
FUNC lda
	load r0,0xfff
	load r0,0xfff
	load r0,0xfff
RET
;call mult => checkTables() => undefined
;ret => parseRET() => printError()