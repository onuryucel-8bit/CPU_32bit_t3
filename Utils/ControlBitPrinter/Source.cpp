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

std::string readFile()
{
	std::fstream file("controlBits.cbp");

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
	std::cout << "Running...\n";

	asmc::Lexer lex(readFile());

	asmc::Parser parser(lex);

	parser.run();

	std::cout << "Closing...\n";

	return 0;
}

//TODO adr_rom ile output arasinda uyusmazlik var 
//NOT 49 gosteriyor kontorol bitleri ise 4A? adresinde