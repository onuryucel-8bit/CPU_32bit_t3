#include "Lexer.h"
#include "Parser.h"

int main()
{
	asmc::Lexer lexer("Source.bc");

	asmc::Parser parser(lexer);

	parser.run();
		
}