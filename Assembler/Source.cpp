#include "Lexer.h"

std::string readFile()
{
	return "";
}

int main()
{
	asmc::Lexer lexer(readFile());
	
}

/*

.include "math.asm"

.origin 0xff

.text "test_txt"

.ascii 'a','b','c'

.dword 0xff,0xaa,0x24

#define ARRAY_START_ADDRESS 0x0f

<opcode> ::= (<adr> | <hex> | <reg> | <regadr>) | (<adr> | <hex> | <reg> | <regadr>)?

*/