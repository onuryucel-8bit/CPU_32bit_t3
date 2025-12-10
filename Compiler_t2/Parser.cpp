#include "Parser.h"
namespace asmc
{


	Parser::Parser(asmc::Lexer& lexer)
		:m_lexer(lexer)
	{
		m_tokenList = lexer.getTokenList();

		m_registerIndex = 0;

		m_tokenIndex = 0;
		m_peekToken = m_tokenList[0];
		moveCurrentToken();
	}

	Parser::~Parser()
	{
	}
	/*

	<program> ::= <let_stmt>
	<let_stmt> ::= "LET" <ID> ("=" <number>)?
	<expression> ::= <ID> "=" <ID> + <ID>
	<number> ::= [0-9]+

	*/
	void Parser::run()
	{
		while (m_currentToken.m_type != asmc::TokenType::ENDOFFILE)
		{
			//std::cout << m_currentToken.m_text << "\n";
			program();
		}

		std::cout << "------------------------------\n";

		for (const auto& [key, value] : m_symbolTable)
		{
			std::cout << key.m_text << "|" << value.m_value << " reg " << value.m_registerIndex << "\n";

			if (value.m_status == asmc::symbolStatus::NOT_USED)
			{
				printWarning("variable[" + key.m_text + "] is not used");
			}
		}



	}

	void Parser::printError(std::string message)
	{
		std::cout << rang::fg::red << "ERROR:: " << message << rang::style::reset << "\n";
	}

	void Parser::printWarning(std::string message)
	{
		std::cout << rang::fg::yellow << "WARNING:: "<< message << rang::style::reset << "\n";
	}

	void Parser::asmLOAD(asmc::Token& token, int value)
	{
		if (m_symbolTable[token].m_registerIndex != -1)
		{
			std::cout << "LOAD r " << m_symbolTable[token].m_registerIndex << "," << value <<"\n";
		}
		else
		{
			std::cout << "PUSH " << value << " | " << token.m_text <<"\n";
		}
	}

	void Parser::program()
	{
		while (m_currentToken.m_type == asmc::TokenType::LET ||
			m_currentToken.m_type == asmc::TokenType::ID)

		{
			switch (m_currentToken.m_type)
			{
			case asmc::TokenType::LET:
				LET_STMT();				
				break;

			case asmc::TokenType::ID:
				expression();
				break;
			}
			
		}
		
	}

	void Parser::LET_STMT()
	{
		match(asmc::TokenType::LET);

		asmc::Token ct = m_currentToken;

		m_symbolTable[ct] = { .m_value = -1, .m_registerIndex = -1, .m_status = asmc::symbolStatus::NOT_USED };

		match(asmc::TokenType::ID);
		if (m_currentToken.m_type == asmc::TokenType::ASSIGN)
		{
			match(asmc::TokenType::ASSIGN);


			int value = number();

			if (m_registerIndex < 4)
			{
				m_symbolTable[ct].m_registerIndex = m_registerIndex;
				m_registerIndex++;
			}
			else
			{
				m_symbolTable[ct].m_registerIndex = -1;
			}

			m_symbolTable[ct].m_value = value;
			m_symbolTable[ct].m_status = asmc::symbolStatus::USED;

								

			asmLOAD(ct, value);
		}
	}

	//<expression> ::= <ID> "=" <ID> + <ID>
	void Parser::expression()
	{
		asmc::Token id = m_currentToken;

		match(asmc::TokenType::ID);
		match(asmc::TokenType::ASSIGN);
		
		asmc::Token id_left = m_currentToken;
		match(asmc::TokenType::ID);
		match(asmc::TokenType::PLUS);
		asmc::Token id_right = m_currentToken;

		if (m_symbolTable.contains(id_left) && m_symbolTable.contains(id_right))
		{
			std::cout << "ADD r" << m_symbolTable[id_left].m_registerIndex << ",r" << m_symbolTable[id_right].m_registerIndex << "\n";			
		}
		else
		{
			printError("id_left or id_right is undefined" + std::to_string(id_left.m_lineNumber));			
		}

		match(asmc::TokenType::ID);
	}

	int Parser::number()
	{
		
		int value = 0;
		if (m_currentToken.m_type == asmc::TokenType::NUMBER)
		{
			value = to_int(m_currentToken.m_text);
		}

		match(asmc::TokenType::NUMBER);

		return value;
		
	}

	void Parser::match(asmc::TokenType type)
	{
		if (m_currentToken.m_type != type)
		{
			//printError
			//f_error = true
		}
		moveCurrentToken();
	}

	void Parser::moveCurrentToken()
	{
		m_currentToken = m_peekToken;

		m_tokenIndex++;
		if (m_tokenIndex >= m_tokenList.size())
		{
			m_tokenList = m_lexer.getTokenList();
			m_tokenIndex = 0;
		}

		m_peekToken = m_tokenList[m_tokenIndex];
	}
}