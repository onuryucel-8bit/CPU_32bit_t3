#pragma once

#include <unordered_map>

#define to_int(str) std::stoi(str)

#include "../libsLocal/rang.hpp"
#include "../libsLocal/utils/Radix.h"

#include "Lexer.h"

#define checkType(type) (m_currentToken.m_type == (type))

namespace asmc
{
	enum class symbolStatus
	{
		USED,
		NOT_USED
	};

	typedef struct 
	{
		int m_value;
		int m_registerIndex;
		symbolStatus m_status;

	}variableVec;


	class Parser
	{
	public:
		Parser(asmc::Lexer& lexer);
		~Parser();

		void run();

	private:

		void printWarning(std::string message);
		void printError(std::string message);
		void printErrorExt(std::string message, asmc::Token token);
		void printCurrentToken();

		void emit(std::string str);

		void asmLOAD(asmc::Token& token, int value);

		void program();
		void let_stmt();
		void assing_stmt();
		void add_expr();
		asmc::Token mult_expr();
		void expression();

		asmc::Token id();		
		asmc::Token number();

		void match(asmc::TokenType type);

		void moveCurrentToken();

		asmc::Lexer& m_lexer;

		asmc::Token m_currentToken;
		asmc::Token m_peekToken;

		std::array<asmc::Token, MAX_TOKEN_LIST_SIZE> m_tokenList;
		size_t m_tokenIndex;


		std::unordered_map<asmc::Token, variableVec> m_symbolTable;

		int m_registerIndex;

		bool f_error;
	};
}