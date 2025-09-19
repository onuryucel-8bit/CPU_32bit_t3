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

//MOD bits
//ozel bitler

#define asmc_MOD_Empty 0
#define asmc_MOD_Number 1
//@adr
#define asmc_MOD_Adr 2
//@ry
#define asmc_MOD_RegAdr 3
//@adr + ry
#define asmc_MOD_Adr_P_Reg 4
//rx, ry
#define asmc_MOD_Rx_Ry 5

#define asmc_CombineMODBits(opcode, modBits) (opcode | (modBits << 15)) 

#define asmc_ShiftAmount_Opcode 24
#define asmc_ShiftAmount_RegA 22
#define asmc_ShiftAmount_RegB 18

namespace asmc
{


enum class LabelStatus
{
	Used,
	NotUsed,
	Undefined,
	Called_Noc,
	Called
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

	void printError(std::string message);
	void printWarning(std::string message);

#ifdef PARSER_TEST_FUNCS
	void printBinHex(std::bitset<32> opcode, std::bitset<32> operand);
	void printCurrentPeekToken();
#endif // PARSER_TEST_FUNCS

	int m_ramLocation;
	int m_lineNumber;

	asmc::Lexer& m_lexer;

	asmc::Token m_currentToken;
	asmc::Token m_peekToken;

	std::unordered_map<std::string, asmc::symbolInfo> m_symbolTable;

	std::unordered_map<std::string, asmc::MemoryLayout> m_jumpTable;


	std::vector<asmc::MemoryLayout> m_output;

	using funcPtr = void (asmc::Parser::*)();

	funcPtr m_parserFuncs[32];
	
	std::string m_lastFuncName;

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
	void parseMOV();


	void parseADD();
	void parseSUB();
	void parseDIV();
	void parseMUL();
	void parseSHL();
	void parseSHR();
	void parseAND();
	void parseOR();
	void parseXOR();
	void parseNOT();
	void parseCMP();

	void parsePUSH();
	void parsePOP();



	void parseSTR();
	void parseLabel();
	void parseJMP();
	void parseCALL();
	void parseFUNC();
	void parseRET();


	void parseJAZ();
	void parseJLZ();
	void parseJGZ();
	void parseJSC();
	void parseJUC();
	void parseJCT();
	void parseJCF();

};

}


