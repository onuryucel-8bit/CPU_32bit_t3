#include "Parser.h"

namespace asmc
{

	Parser::Parser(asmc::Lexer& lexer)
		:m_lexer(lexer)
	{

	}

	Parser::~Parser()
	{
	}

	void Parser::run()
	{
		/*
		asmc::TokenList* list = m_lexer.getTokenList();
		std::cout << list->m_size << "\n";

		for (size_t i = 0; i < list->m_size; i++)
		{
			std::cout << list->m_list[i].m_text<< "\n";
		}
		*/
		asmc::Token t = m_lexer.getToken();
		while (t.m_type != asmc::TokenType::ENDOFFILE)
		{
			if (t.m_type != asmc::TokenType::NEWLINE)
			{
				std::cout << t.m_text << "\n";				
			}
			t = m_lexer.getToken();

		}
	}

}