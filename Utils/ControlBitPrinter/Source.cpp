#include <iostream>
#include <vector>

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

int main()
{
	std::string txtFile;

	readFile();

	parser();
	callLogisimPrinter();
	
	return 0;
}