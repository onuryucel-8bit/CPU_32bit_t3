#pragma once

#include <iostream>
#include <cstdint>
#include <unordered_map>

//TEST DEBUG
#include <bitset>

#include "../libsLocal/rang.hpp"

#include "Lexer.h"
#include "utils/Radix.h"

#define PARSER_TEST_FUNCS

namespace asmc
{


enum class LabelStatus
{
	Used,
	NotUsed,
	Undefined
};

struct symbolInfo
{
	int m_ramIndex = -1;
	asmc::LabelStatus m_status = asmc::LabelStatus::NotUsed;
};

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
	
	void checkTables();

	void printError(std::string& message);
	void printWarning(std::string& message);

#ifdef PARSER_TEST_FUNCS
	void printBinHex(std::bitset<32> opcode, std::bitset<32> operand);
#endif // PARSER_TEST_FUNCS

	int m_ramLocation;
	int m_lineNumber;

	asmc::Lexer& m_lexer;

	asmc::Token m_currentToken;
	asmc::Token m_peekToken;

	std::unordered_map<std::string, asmc::symbolInfo> m_symbolTable;
	std::unordered_map<std::string, asmc::MemoryLayout> m_jumpTable;


	std::vector<asmc::MemoryLayout> m_output;
	/*
	*	opcode = opcode_HEX_VAL;
	*	
	*	getSecondPart();
	* 
	*	//insert Mod bits
	*	switch()
	*		...
	* 
	*	output.push_back(opcode_val);
	*/
	//-----------parseX()--------------//
	void parseLOAD();
	void parseADD();
	void parseSTR();
	void parseLabel();
	void parseJMP();
};

}


