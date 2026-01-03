load r0,0x0		;x
load r1,0x0		;y

;rlx
load r2,@3000	

load r3,0x3000	;i
load r4,0x0		;color
load r5,0x0		;j

loop:

add r3,0x1
load r4,@r3

load r5,0x0
	
	drawloop:
		
		cmp r0,0x81		;if x != 128 jump
		jne endline
			load r0,0x0	;x = 0
			add r1,0x1	;y++
		endline:

		str @fffffffa,r4	;color
		str @fffffff9,r0	;posx
		str @fffffff8,r1	;posy
		load r6,0x1
		str @fffffffb,r6	;com

		add r5, 0x1
		add r0, 0x1

		cmp r5,r2
		jne drawloop

add r3,0x1
load r2,@r3

jmp loop



int x = 0;
int y = 0;
int rlx = arr[0];
int i = 0;
int color = 0;

while(1)
{
	i++;
	color = arr[i];

	int j = 0;
	while(j < rlx)
	{	
		if(x == 129)
		{
			x = 0;
			y++;
		}

		draw(x,y,color);
		j++;

		x++;				
	}

	i++;
	rlx = arr[i];
}



load r1,0x0		;x
load r2,0x0		;y

loopWHILE:
	load r3,@r0		;rlx
	load r4,0x0		;j

	add r0,0x1		;i++
	load r5,@r0		;color

	loop:
		str @fffffffa,r5
		str @fffffff9,r1
		str @fffffff8,r2
		str @fffffffb,0x1

		add r4,0x1	;j++
		cmp r1,0x80
		jne	compx
			add r2,0x1	;y++
			load r1,0x0	;x = 0
		compx:

		add r1,0x1		;x++

		cmp r4,r3
	jne loop


	add r0,0x1		;i++
jmp loopWHILE

;const uint32_t asmc_KMT_adr	= 0xffff'fffb
;const uint32_t asmc_COLOR_adr  = 0xffff'fffa
;const uint32_t asmc_POSX_adr   = 0xffff'fff9
;const uint32_t asmc_POSY_adr   = 0xffff'fff8