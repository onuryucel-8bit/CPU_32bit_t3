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
#include <sstream>

#include "Lexer.h"
#include "Parser.h"

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

int main()
{	        
	asmc::Lexer lex(readFile());

	asmc::Parser parser(lex);

	parser.run();

	return 0;
}
