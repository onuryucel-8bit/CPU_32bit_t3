#pragma once

#include <unordered_map>


#define to_int(str) std::stoi(str)

#include "../libsLocal/rang.hpp"
#include "../libsLocal/utils/Radix.h"

#include "Lexer.h"

#include "MemoryManager.h"

#define getAdr_asString(exprVal)(std::to_string(m_symbolTable[exprVal.m_token].m_ramIndex))

#define asmc_TTY_adr 10000000


//#define PRINT_RL 1

namespace asmc
{
	/*
	*	holds variable initial values and addresses
	*	LET A = 5;
	*	LET B = 3;
	*	used for writing var section
	*
	*
	*	.origin VAR_SECTION
	*	.db 5, 3
	*/
	struct variableVec
	{
		uint32_t m_value;
		uint32_t m_ramIndex;
	};

	enum class Location
	{		
		None,
		Register,
		Stack
	};

	struct ExprVal
	{				
		asmc::Token m_token;
		asmc::Location m_location = Location::None;
		char m_registerIndex = -1;
		//rhs right hand side
		bool m_rhsComputed = false;
		bool m_recRet = false;
		uint32_t m_value;
	};


	enum class symbolStatus
	{
		USED,
		NOT_USED
	};

	enum class variableType
	{
		Int32,
		Array,
		String
	};

	struct SymbolEntry
	{
		size_t m_ramIndex = 0;
		uint32_t m_value = 0;		
		symbolStatus m_status = symbolStatus::NOT_USED;		
		variableType m_type = variableType::Int32;
	};

	class Parser
	{
	public:
		Parser(asmc::Lexer& lexer);
		~Parser();

		void run();

	private:
						
		//======PARSE=============================================//
		void program();
		void while_stmt();
		void if_stmt();
		void let_stmt();
		void print_stmt();

		//---------------------//
		void assing_stmt();
		asmc::ExprVal expression();
		asmc::ExprVal add_expr();
		asmc::ExprVal mult_expr();
		
		//---------------------//
		asmc::ExprVal primary();		
		asmc::ExprVal id();
		asmc::ExprVal number();

		//========================================================//


		//======PRINT=============================================//
		void printWarning(std::string message);
		void printError(std::string message);
		void printErrorExt(std::string message, asmc::Token token);
		//========================================================//


		//=====OUTPUT=============================================//
		void emit(std::string str);
		void loadToRegister(asmc::ExprVal& expval);
		void instruction(asmc::TokenType operation, int registerIndex, int opcodeType, int value);
		//========================================================//

		//=================Lexer==================================//
		asmc::Lexer& m_lexer;
		asmc::Token m_currentToken;
		asmc::Token m_peekToken;

		std::array<asmc::Token, MAX_TOKEN_LIST_SIZE> m_tokenList;
		size_t m_tokenIndex;
		//========================================================//

		void checkTable(asmc::ExprVal& expval);

		void match(asmc::TokenType type);

		void moveCurrentToken();


		asmc::MemoryManager m_memManager;

		std::vector<variableVec> m_varlist;
		
		std::unordered_map<asmc::Token, SymbolEntry> m_symbolTable;

		uint32_t m_labelIndex;

		bool f_error;
	};
}