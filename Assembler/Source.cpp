#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Lexer.h"
#include "Parser.h"


std::string readFile(std::string path)
{
	std::fstream file(path);

	if (!file.is_open())
	{
		std::cout << "ERROR:: couldnt open the file\n";
		return "";
	}

	std::stringstream ss;

	ss << file.rdbuf();

	return ss.str();
}

//std::vector<FileData> inputStream;
//
//struct FileData
//{
//	int id;
//	std::string path;
//	int hashcode;
//};

int main(int argc, char* argv[])
{
	/*if (argc < 2)
	{
		std::cout << "Expected file path...\n";
		
		return -1;
	}*/

	asmc::Lexer lexer(readFile("main.asm"));

	asmc::Parser parser(lexer);

	parser.run();

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