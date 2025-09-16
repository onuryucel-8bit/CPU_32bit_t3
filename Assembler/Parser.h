#pragma once

#include <iostream>
#include <cstdint>

//TEST DEBUG
#include <bitset>

#include "Lexer.h"
#include "utils/Radix.h"


namespace asmc
{


struct MemoryLayout
{
	uint32_t m_opcode = 0;
	uint32_t m_secondPart = 0;
};

class Parser
{
public:
	Parser(asmc::Lexer& lexer);
	~Parser();

	void run();
private:
	
	void program();

	void moveCurrentToken();

	asmc::Lexer& m_lexer;

	asmc::Token m_currentToken;
	asmc::Token m_peekToken;


	//-----------parseX()--------------//
	void parseLOAD();
	void parseADD();
	void parseSTR();
};

}


