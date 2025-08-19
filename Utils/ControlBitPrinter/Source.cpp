/*
*	controlBits.txt =>[...] => ROM.txt
*   controlBits.txt => [string => vector[ CB[Read, MDR_we,...], CB[IR_we, MDR_out,....] ) => LogisimPrinter ] => ROM.txt
* 
*	\ => new Control bit start
*
*	.ORIGIN 0x000f
*
*	Read
*	MDR_we
*	\
*	IR_we
*
*	 |
*	 v
*
*	0x10000 => vector[ controlBit[Read, MDR_we, ....], CB[IR_we, MDR_out, ....] )
*	0xa0000
*
*	vector => LogisimPrinter.print(std::vector) => ROM
* 
*  
*/


/*
	{
		Read
		PC_enb
		MAR_enb
		#
		Read
		PC_load
		...
		#
		...
		..
		#
	}
	{
		Read
		...
		..
		#
		..
		..
		#
		...
		...
		#
	}

	bnf

	<program> ::= "{" <controlBit> "#" "}"
	<controlBit> ::= "Read" | "PC_load" | "PC_enb" | ...

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "Lexer.h"


std::vector<int> controlBits;

std::vector<int> result;

void calcControlBits()
{
	int res = 0;
	for (size_t i = 0; i < controlBits.size(); i++)
	{
		res += std::pow(2, controlBits[i]);
	}
	std::cout << "control bits hex : " << std::hex << res << "\n";
	std::cout << std::dec;
	result.push_back(res);
}

void parser(std::string input)
{
	reflex::Input file(input);

	Lexer lx(file);

	cb::Token currentToken = lx.lex();

	
	while (currentToken.m_type != cb::TokenType::ENDOFFILE && 
		currentToken.m_type != cb::TokenType::UNKNOWN)
	{
		std::cout << currentToken.m_text << "\n";

		if (currentToken.m_type != cb::TokenType::LPAREN && 
			currentToken.m_type != cb::TokenType::RPAREN &&
			currentToken.m_type != cb::TokenType::HASH)
		{
			controlBits.push_back(currentToken.m_type);
		}

		if (currentToken.m_type == cb::TokenType::HASH)
		{
			calcControlBits();
			controlBits.clear();
		}

		currentToken = lx.lex();
	}

	
}

std::string readFile()
{
	std::fstream file("controlBits.txt");

	if (!file.is_open())
	{
		std::cout << "ERROR:: couldnt open the file\n";
		return "";
	}

	std::stringstream ss;

	ss << file.rdbuf();

	return ss.str();
}

#include "DyLogisimPrinter.h"

int main()
{	        
	parser(readFile());

	DyLogisimPrinter dlp;
	dlp.printROM(20, result);

	
	
	return 0;
}
