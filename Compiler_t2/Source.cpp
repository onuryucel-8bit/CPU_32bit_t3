#include "Lexer.h"
#include "Parser.h"

//TODO use _DEBUG? msvc macro
#define DEBUG_FILE

int main(int argc, char* argv[])
{

	//TODO add filesystem

	std::string path;
	if (argc < 2)
	{
		std::cout << "Expected file path...\n";

#ifdef DEBUG_FILE		
		std::cout << "Loading default Source.bc\n";
		path = "Source.bc";
#else
		return 0;
#endif // DEBUG_FILE
	}
	else
	{
		path = argv[1];
	}

	asmc::Lexer lexer(path);

	asmc::Parser parser(lexer);

	parser.run();		
}