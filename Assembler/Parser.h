#pragma once

#include <iostream>
#include <cstdint>
#include <unordered_map>

//TEST DEBUG
#include <bitset>
#include <fstream>

#include "../libsLocal/rang.hpp"
#include "utils/Radix.h"

#include "Lexer.h"


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

#define asmc_CombineMODBits(opcode, modBits) ((uint32_t)opcode | ((uint32_t)modBits << 15))

#define asmc_ShiftAmount_Opcode 24
#define asmc_ShiftAmount_RegA 21
#define asmc_ShiftAmount_RegB 18

namespace asmc
{

enum CommandType
{
	ALU,
	OTHER
};

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
	int ramIndex = -1;
};

struct PacketAdrPReg
{
	uint32_t m_adrPart;
	uint32_t m_regPart;
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

	MemoryLayout parseOperand(uint32_t opcode);

	PacketAdrPReg getAdr_P_RegPart(std::string& operand);

#ifdef PARSER_TEST_FUNCS
	asmc::TokenType toToken(size_t opcode);
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

	//8byte
	using funcPtr = void (asmc::Parser::*)();

	funcPtr m_parserFuncs[32];
	
	std::string m_lastFuncName;

	bool f_error;
	bool fd_PrintHexOutput;//flag debug fd
	

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
	void parseSTR();


	//------------------ALU-------------//
	void parseArithmeticPart();

	//----parse and,or,xor,shl,shr-------//
	void parseLogicPart();
	//----------------------------------//

	void parseNOT();

	void parseCMP();
	//-----------------------------------//

	void parsePUSH();
	void parsePOP();

	
	void parseLabel();

	void parseCALL();
	void parseFUNC();
	void parseRET();

	//----parse jmp,jlz,jgz,jcf....-------//
	void parseJMP();

};

}


