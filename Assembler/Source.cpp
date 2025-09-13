#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Lexer.h"

std::string readFile()
{
	std::fstream file("program.txt");

	if (!file.is_open())
	{
		std::cout << "ERROR:: couldnt open the file\n";
		return "";
	}

	std::stringstream ss;

	ss << file.rdbuf();

	return ss.str();
}

int main()
{
	asmc::Lexer lexer(readFile());
	
	asmc::Token token = lexer.getToken();

	while (token.m_type != asmc::TokenType::ENDOFLINE &&
		token.m_type != asmc::TokenType::EMPTY)
	{
		if (token.m_type != asmc::TokenType::NEWLINE)
		{
			std::cout << token.m_text << " | " << magic_enum::enum_name(token.m_type) << "\n";
		}

		token = lexer.getToken();
		
	}

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