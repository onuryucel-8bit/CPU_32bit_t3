/*
	32BitCpu_t3::2025
	AssemblerV4
*/

main:
	LOAD r1, 0x5
	LOAD r2, 0x2
	ADD r1, r2
	STR @ff, r1

	JMP main

