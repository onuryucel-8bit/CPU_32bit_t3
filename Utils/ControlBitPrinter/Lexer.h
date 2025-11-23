#pragma once


#include <iostream>
#include <optional>
#include <string>
#include <cstdint>
#include <array>

#include "../../libsLocal/magic_enum/magic_enum.hpp"
#include "../../libsLocal/rang.hpp"

#include "CbTokens.h"

#define MAX_TOKEN_LIST_SIZE 30

namespace asmc
{
	struct Token
	{
		std::string m_text = "";
		asmc::TokenType m_type = asmc::TokenType::UNKNOWN;

		bool operator==(const Token& other) const
		{
			return m_text == other.m_text && m_type == other.m_type;
		}
	};

	struct TokenList
	{
		asmc::Token* m_list;
		size_t m_size = 0;
	};

	class Lexer
	{
	public:



		Lexer(std::string file);
		~Lexer();
		[[nodiscard]] std::array<asmc::Token, MAX_TOKEN_LIST_SIZE> getTokenList();
		[[nodiscard]] Token getToken();

		size_t m_lineNumber;

		

	private:		
		
		bool checkIfKeyword(std::string token);
		//sonraki karakteri isaret eder
		void nextChar();

		char peek();

		void printError(std::string message);

		//bosluklari es gecer
		void skipWhiteSpace();
		//aciklama satiri ; sonrasindaki karakterleri es gecer
		void skipComments();
		//skip ',' '\n'
		void skipNonEssential();

		std::string getSubStr(int startPos, int length, int (*cmpFunc)(int), bool upper);

		int m_position;
		char m_currentChar;
		bool f_error;

		std::string m_program;

		std::array<asmc::Token, MAX_TOKEN_LIST_SIZE> m_tokenArr;
		

	};
}