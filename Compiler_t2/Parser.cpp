#include "Parser.h"
namespace asmc
{


	Parser::Parser(asmc::Lexer& lexer)
		:m_lexer(lexer)
	{
		m_tokenList = lexer.getTokenList();

		m_registerIndex = 0;
		f_error = false;

		m_tokenIndex = 0;
		m_peekToken = m_tokenList[0];
		moveCurrentToken();
	}

	Parser::~Parser()
	{
	}

	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//

	void Parser::printError(std::string message)
	{
		std::cout << rang::fg::red 
			<< "ERROR:: line number["<< m_currentToken.m_lineNumber << "] "
			<< message
			<< " ["+ m_currentToken.m_text +"]" 
			<< " type ["+ std::string(magic_enum::enum_name(m_currentToken.m_type)) + "] "			
			<< rang::style::reset << "\n";

		f_error = true;
	}

	void Parser::printErrorExt(std::string message, asmc::Token token)
	{
		std::cout << rang::fg::red
			<< "ERROR:: line number[" << token.m_lineNumber << "] "
			<< message
			<< " [" + token.m_text + "]"
			<< " type [" + std::string(magic_enum::enum_name(token.m_type)) + "] "
			<< rang::style::reset << "\n";

		f_error = true;
	}

	void Parser::printCurrentToken()
	{
		std::cout << m_currentToken << "\n_____________\n";		
	}

	void Parser::emit(std::string str)
	{
		if (!f_error)
		{
			std::cout << str << "\n";
		}	
	}

	void Parser::printWarning(std::string message)
	{
		std::cout << rang::fg::yellow << "WARNING:: " << message << rang::style::reset << "\n";
	}

	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//


	
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

	void Parser::asmLOAD(asmc::Token& token, int value)
	{
		if (m_symbolTable[token].m_registerIndex != -1)
		{
			//std::cout << "LOAD r " << m_symbolTable[token].m_registerIndex << "," << value <<"\n";
		}
		else
		{
			//std::cout << "PUSH " << value << " | " << token.m_text <<"\n";
		}
	}
	/*
	* 
	
	<program> ::= (<let_stmt> | <assing_stmt>)*
	<let_stmt> ::= "LET" <id> ("=" <number> | <id>)?
	<assing_stmt> ::= <id> "=" <add_expr>

	<add_expr> ::= <mult_expr> ((+ | -) <mult_expr>)*
	<mult_expr> ::= (<id> | <number>) ((* | /) (<id> | <number>))*

	<id> ::= [a-zA-Z]+
	<number> ::= [0-9]+
	*/
	void Parser::program()
	{
		while (checkType(asmc::TokenType::LET) ||
			checkType(asmc::TokenType::ID))

		{			

			switch (m_currentToken.m_type)
			{
			case asmc::TokenType::LET:
				let_stmt();				
				break;

			case asmc::TokenType::ID:
				assing_stmt();
				break;
			}
			
		}
		
	}

	void Parser::let_stmt()
	{
		match(asmc::TokenType::LET);

		asmc::Token idName = id();
		
		m_symbolTable[idName] = { .m_value = -1, .m_registerIndex = -1, .m_status = asmc::symbolStatus::NOT_USED };

		if (checkType(asmc::TokenType::ASSIGN))
		{
			match(asmc::TokenType::ASSIGN);

			asmc::Token value = number();
			//emit("LOAD r"+ std::to_string(m_registerIndex) + "," + idName.m_text + " ," + value.m_text);
			emit("LOAD r" + std::to_string(m_registerIndex) + ",0x" + rdx::decToHex(std::stoi(value.m_text)));
			
			m_symbolTable[idName].m_value = std::stoi(value.m_text);
			m_symbolTable[idName].m_registerIndex = m_registerIndex;
			m_symbolTable[idName].m_status = asmc::symbolStatus::NOT_USED;

			m_registerIndex++;
		};
			
		
	}

	void Parser::assing_stmt()
	{
		asmc::Token idRes = id();

		if (!m_symbolTable.contains(idRes))
		{
			printErrorExt("undefined variable used ", idRes);
		}
		else
		{
			m_symbolTable[idRes].m_status = asmc::symbolStatus::USED;
		}
		//emit("assign_stmt VAR " + idRes.m_text + " = ");

		if (!(checkType(asmc::TokenType::ASSIGN)))
		{
			printError("assign_stmt() Expected '=' found, ");
		}
		
		moveCurrentToken();

		add_expr();
	}
	//<add_expr> ::= <mult_expr> ((+ | -) <mult_expr>)*
	void Parser::add_expr()
	{
		asmc::Token idLeft = mult_expr();
		
		asmc::Token idRight;

		while (checkType(asmc::TokenType::PLUS) || checkType(asmc::TokenType::MINUS))
		{
			asmc::Token op = m_currentToken;
			//emit("OPERATOR," + op.m_text);

			moveCurrentToken();

			if (checkType(asmc::TokenType::ID))
			{
				idRight = mult_expr();
			}
			else if (checkType(asmc::TokenType::NUMBER))
			{				
				idRight = mult_expr();

				switch (op.m_type)
				{
					case asmc::TokenType::PLUS:
						emit("ADD r" + std::to_string(m_symbolTable[idLeft].m_registerIndex) + ", 0x" + rdx::decToHex(std::stoi(idRight.m_text)));
						break;
					case asmc::TokenType::MINUS:
						emit("SUB r" + std::to_string(m_symbolTable[idLeft].m_registerIndex) + ", 0x" + rdx::decToHex(std::stoi(idRight.m_text)));
						break;

				}
			}
			else
			{
				printError("expression() while + - ");
			}
		}
	}

	//<mult_expr> ::= (<id> | <number>) ((* | /) (<id> | <number>))*
	asmc::Token Parser::mult_expr()
	{
		asmc::Token idLeft;

		switch (m_currentToken.m_type)
		{
		case asmc::TokenType::ID:
			idLeft = id();
			if (!m_symbolTable.contains(idLeft))
			{
				printErrorExt("undefined variable used ", idLeft);
			}
			else
			{
				m_symbolTable[idLeft].m_status = asmc::symbolStatus::USED;
			}

			//emit("VAR " + idLeft.m_text);
			break;

		case asmc::TokenType::NUMBER:
			idLeft = number();
			//emit("NUMBER ," + idLeft.m_text);
			break;

		default:
			printError("expected <ID> or <NUMBER> found,");
			break;
		}		
		
		//TODO fix idRight

		asmc::Token idRight;

		while (checkType(asmc::TokenType::ASTERISK) || checkType(asmc::TokenType::SLASH))
		{

			asmc::Token op = m_currentToken;
			

			moveCurrentToken();

			if (checkType(asmc::TokenType::ID))
			{
				idRight = id();
				emit("OPERATOR," + op.m_text + "," + idRight.m_text);
			}
			else if (checkType(asmc::TokenType::NUMBER))
			{
				idRight = number();
				switch (op.m_type)
				{
				case asmc::TokenType::ASTERISK:
					emit("MUL r" + std::to_string(m_symbolTable[idLeft].m_registerIndex) + ", 0x" + rdx::decToHex(std::stoi(idRight.m_text)));
					break;
				case asmc::TokenType::DIV:
					emit("DIV r" + std::to_string(m_symbolTable[idLeft].m_registerIndex) + ", 0x" + rdx::decToHex(std::stoi(idRight.m_text)));
					break;

				}
			}
			else
			{
				printError("expected <ID> or <NUMBER> found,");
			}
		}

		

		return idLeft;
	}

	//<expression> :: = <id> "=" (<id> | <number>) ((+| -) (<id> | < number>)) *
	void Parser::expression()
	{

		

	}

	asmc::Token Parser::id()
	{
		asmc::Token retval;
		
		if (checkType(asmc::TokenType::ID))
		{
			retval = m_currentToken;
		}
		else
		{
			printError("expected <ID>, found");
		}
		
		moveCurrentToken();

		return retval;

	}

	asmc::Token Parser::number()
	{
		
		asmc::Token token;
		if (checkType(asmc::TokenType::NUMBER))
		{
			token = m_currentToken;
		}
		else
		{
			printError("number() error");
		}
		
		moveCurrentToken();

		return token;
		
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
		//printCurrentToken();


		m_tokenIndex++;
		if (m_tokenIndex >= m_tokenList.size())
		{
			m_tokenList = m_lexer.getTokenList();
			m_tokenIndex = 0;
		}

		m_peekToken = m_tokenList[m_tokenIndex];
	}
}