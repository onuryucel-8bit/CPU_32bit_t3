.origin 0xfe

.db 0x2, 0x5

.origin 0x0

LOAD r0, @fe
LOAD r1, @ff

ADD r0,r1

STR @a0,r0