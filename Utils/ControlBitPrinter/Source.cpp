#include <iostream>
#include <vector>
#include <cmath> 



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




enum ControlBits
{
	space,

	Read,
	Write,

	PC_enb,
	PC_load,

	MDR_we,
	MDR_out,

	IR_we,
	IR_out,

	TEMP_we,
	TEMP_out,

	Posta_we,
	Posta_out,
	
	ADR_Reg_we,
	ADR_Reg_out,
};

std::vector<int> input
{
	Read,
	Write,

	space,

	Posta_we,
	Posta_out

};

struct romLayout
{
	int x;
};

std::vector<romLayout> output;

void readFile()
{

}

//CALL dynamic printer .exe
void callLogisimPrinter()
{

}

//Lexer
void lexer()
{

}
//Parser

void parser()
{
	lexer();
}

void calcControlBits()
{

	int res = 0;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == ControlBits::space)
		{

		}

		res += std::pow(2, input[i]);
	}
	std::cout << "hex : " << std::hex << res << "\n";
	//std::cout << std::dec;

}

#include "Lexer.h"

int main()
{
	std::string txtFile;

	readFile();

	parser();
	callLogisimPrinter();
	
	return 0;
}
