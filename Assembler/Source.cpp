#include "LexerPCH.h"

#include "Lexer.h"

#include "Parser.h"


int main(int argc, char* argv[])
{

	/*std::string path = argv[1];
	if (argc < 2)
	{
		std::cout << "Expected file path...\n";
		std::cout << "Loading default main.asm\n";
		
		path = "main.asm";
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

TODO
hata var sayilar 5 3 tanimsiz oldugu icin add (icinde donusum)?
yaparken sonsuz donguye takiliyor

LOAD r0, 5
LOAD r1, 3
ADD r0, r1

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