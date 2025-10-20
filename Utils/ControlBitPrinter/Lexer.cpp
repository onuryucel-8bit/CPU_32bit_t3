#include "Lexer.h"

namespace asmc
{

Lexer::Lexer(std::string file)
{
	m_program = file;


	m_position = -1;
	m_currentChar = 0;
	m_lineNumber = 0;


	f_error = false;

	nextChar();

	m_tokenArr.m_list = new asmc::Token[MAX_TOKEN_LIST_SIZE];

}

Lexer::~Lexer()
{
	delete[] m_tokenArr.m_list;
}

asmc::TokenList* Lexer::getTokenList()
{
	for (size_t i = 0; i < MAX_TOKEN_LIST_SIZE; i++)
	{
		asmc::Token token = getToken();

		if (token.m_type == asmc::TokenType::ENDOFFILE)
		{
			m_tokenArr.m_list[i] = { "EOF", asmc::TokenType::ENDOFFILE };
			m_tokenArr.m_size++;
		}

		m_tokenArr.m_list[i] = token;
		m_tokenArr.m_size++;
	}

	return &m_tokenArr;
}

Token Lexer::getToken()
{
	skipWhiteSpace();
	skipComments();
	skipNonEssential();
	
	Token token;

	//std::cout << m_currentChar;

	//Register,[keyword,label,jumploc] check
	if (std::isalpha(m_currentChar))
	{		
		std::string tokenStr = getSubStr(m_position, 1, std::isalpha, false);

		if (checkIfKeyword(tokenStr))
		{
			std::optional<TokenType> enumVal = magic_enum::enum_cast<TokenType>(tokenStr);
			token.m_type = enumVal.value();
			token.m_text = tokenStr;
		}
		else
		{
			printError("Undefined token[" + tokenStr + "]");
		}

	}
	//number check
	else if (std::isdigit(m_currentChar))
	{
		
	}
	else
	{
		switch (m_currentChar)
		{	
			case '\n':
				token.m_text = '\n';
				token.m_type = asmc::TokenType::NEWLINE;
			break;

			case '#':
				token.m_text = '#';
				token.m_type = asmc::TokenType::HASH;
			break;

			case '{':
				token.m_text = '{';
				token.m_type = asmc::TokenType::LPAREN;
			break;

			case '}':
				token.m_text = '}';
				token.m_type = asmc::TokenType::RPAREN;
			break;

			case asmc::ENDOFFILE:
				token.m_text = 'EOF';
				token.m_type = asmc::TokenType::ENDOFFILE;
			break;
		}
	}

	nextChar();

	return token;
}

char Lexer::peek()
{
	if (m_position + 1 >= m_program.length())
	{
		return asmc::TokenType::ENDOFFILE;
	}
	return m_program[m_position + 1];
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void Lexer::skipComments()
{
	if (m_currentChar == '/' && peek() == '/')
	{
		while (m_currentChar != '\n' && m_currentChar != asmc::TokenType::ENDOFFILE)
		{
			nextChar();
		}
		
	}

	//TODO aciklama satiri icerisinde / olunca hata veriyor duzelt bunu 
	if (m_currentChar == '/')
	{
		if (peek() == '*')
		{
			nextChar();//*
			nextChar();// \n
			while (m_currentChar != asmc::TokenType::ENDOFFILE && m_currentChar != '*' && peek() != '/')
			{
				nextChar();
			}
			nextChar();// /
			nextChar();// \n
		}
		else
		{
			printError("LEXER::Comment is not defined ::error ");

		}

	}
}

void Lexer::skipWhiteSpace()
{
	while (m_currentChar == ' ' || m_currentChar == '\t')
	{
		nextChar();
	}
}

//skip ',' 
void Lexer::skipNonEssential()
{
	while (m_currentChar == ',')
	{
		nextChar();
	}
}


//returns next char if pos >= program.length returns enum::EOF
void Lexer::nextChar()
{
	m_position++;
	if (m_position >= m_program.length())
	{
		m_currentChar = asmc::TokenType::ENDOFFILE;
	}
	else
	{
		m_currentChar = m_program[m_position];
	}

	//for printError
	if (m_currentChar == '\n')
	{
		m_lineNumber++;
	}
}

std::string Lexer::getSubStr(int startPos, int length, int (*cmpFunc)(int), bool upper)
{
	while (cmpFunc(peek()) || peek() == '_')
	{
		nextChar();
		length++;
	}

	std::string tokenStr = m_program.substr(startPos, length);

	if (upper)
	{
		//toUpper(tokenStr);
	}


	return tokenStr;
}

bool Lexer::checkIfKeyword(std::string token)
{
	std::optional<TokenType> tempToken = magic_enum::enum_cast<TokenType>(token);
	if (tempToken.has_value())
	{
		return true;
	}

	return false;
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//

void Lexer::printError(std::string message)
{
	std::cout << rang::fg::cyan
			<< "*****************************\n"
			<< "ERROR::Lexer::" << message			
			<< "\n*****************************\n"
			<< rang::style::reset;

	
}

}