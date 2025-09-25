#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Lexer.h"
#include "Parser.h"


int main(int argc, char* argv[])
{
	/*if (argc < 2)
	{
		std::cout << "Expected file path...\n";
		
		return -1;
	}*/

	/*
	switch(flags)
	{
		case fd:
		parser.fd_Print
		break;
	}
	*/

	asmc::Lexer lexer("main.asm");

	asmc::Parser parser(lexer);

	parser.run();

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