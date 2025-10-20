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


//TODO
//.db ciktisinda hata var 32 bit formatina cevir
//Cagrilmayan fonksiyonlari std::vector cikar
// 

//load ra,0x5 hata veriyor isdigit fonksiyonu hex kontrolu yapmiyor 
//if (m_currentChar == 'r' && std::isdigit(static_cast<uint8_t>(peek())))
/*bool Lexer::isOperand()
{
	//isxdigit() checks next char is it hex?
	//register ?
	if (m_currentChar == 'r' && std::isdigit(static_cast<uint8_t>(peek())))
	{
		return true;
	}
	//is 0xfa
	else if (isNumberHex())
	{
		return true;
	}

	return false;
}
*/