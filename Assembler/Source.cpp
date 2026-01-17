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
