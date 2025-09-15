#include "Lexer.h"
namespace asmc 
{

Lexer::Lexer(std::string program)
{
	m_program = program;
	m_position = -1;
	m_currentChar = 0;
	m_lineNumber = 0;

	f_error = false;
	f_newline = true;

	nextChar();
}

Token Lexer::getToken()
{
	skipWhiteSpace();
	skipComments();
	skipNonEssential();
	//TODO bosluklari es gecme mekanizmasi ekle
	skipWhiteSpace();


	//TODO aciklama satiri icerisinde / olunca hata veriyor duzelt bunu 
	if (m_currentChar == '/')
	{
		if (peek() == '*')
		{
			nextChar();//*
			nextChar();// \n
			while (m_currentChar != asmc::TokenType::ENDOFLINE && m_currentChar != '/')
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


	Token token;

	//.origin .db
	if (m_currentChar == '.')
	{
		token = lexDotPart();

		f_newline = false;
	}
	//#define, #include
	else if (m_currentChar == '#')
	{
		token = lexMacro();

		f_newline = false;
	}
	//Register,[keyword,label,jumploc] check
	else if (std::isalpha(m_currentChar))
	{
		//Register?
		if (isOperand())
		{
			token = lexRegPart();
		}
		//keyword,label,jumploc
		else
		{
			token = lexWord();
		}
		f_newline = false;
	}
	//number check
	else if (std::isdigit(m_currentChar))
	{
		//HEX NUMBER
		if (isOperand())
		{			
			token = lexHexNumberPart();
		}			
		else
		{			
			//FIXME ???
			f_error = true;

		}
		f_newline = false;

	}
	else
	{
		token = lexSingleChar();
		f_newline = false;
	}

	nextChar();

	m_lastToken = token;

	return token;

}

char Lexer::peek()
{
	if (m_position + 1 >= m_program.length())
	{
		return ENDOFLINE;
	}
	return m_program[m_position + 1];
}

char Lexer::peekOverX()
{
	if (m_position + 2 >= m_program.length())
	{
		return ENDOFLINE;
	}
	return m_program[m_position + 2];
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//

asmc::Token Lexer::lexDotPart()
{
	Token token;

	//get token str
	int startPos = m_position + 1;
	int length = 0;
	while (std::isalpha(peek()))
	{
		nextChar();
		length++;
	}

	std::string tokenStr = m_program.substr(startPos, length);

	toUpper(tokenStr);

	if (checkIfKeyword(tokenStr))
	{
		//returns ORIGIN or DB token
		std::optional<asmc::TokenType> enumVal = magic_enum::enum_cast<asmc::TokenType>(tokenStr);
		token = { tokenStr, enumVal.value() };
	}
	else
	{
		f_error = true;
	}
	

	return token;
}

asmc::Token Lexer::lexRegPart()
{	
	nextChar();
	if (isxdigit(static_cast<uint8_t>(peek())))
	{
		std::cout << "ERROR invalid reg operand it should be r[0-7]\n";
		return EMPTY_TOKEN;
	}

	
	return { std::string(1,m_currentChar), asmc::TokenType::REGISTER };
}

asmc::Token Lexer::lexHexNumberPart()
{
	nextChar();//skip 0
	nextChar();//skip x
	
	std::string tokenStr = getSubStr(m_position, 1, std::isxdigit);

	return {tokenStr, asmc::TokenType::HEXNUMBER};
}

asmc::Token Lexer::lexSingleChar()
{
	asmc::Token token;

	int startPos;
	int length;
	std::string tokenStr;

	switch (m_currentChar)
	{
	
	case '\n':
		//std::cout << "LEXER newline detected\n";
		token = { std::string(1,m_currentChar), asmc::TokenType::NEWLINE };
		f_newline = true;		
		m_lineNumber++;
		break;

		//ADDRESS
	case '@':		

		nextChar();//move cursor to number OR r

		
		if (m_currentChar == 'r')
		{
			nextChar();
			token = { std::string(1, m_currentChar), asmc::TokenType::REGADR };
		}
		else
		{
			
			tokenStr = getSubStr(m_position, 1, std::isxdigit);
			
			//TODO + - arasi bosluklari kabul eden kod yaz

			//peek() == '+' || '-'
			if (peek() == '+' || peek() == '-')
			{
				nextChar();//+
				tokenStr += m_currentChar;
				
				nextChar();//r
				if (isOperand())
				{
					nextChar();//3
					tokenStr += m_currentChar;

					token = { tokenStr, asmc::TokenType::ADR_P_REG};
				}
			}
			else
			{
				token = { tokenStr, asmc::TokenType::ADDRESS };
			}

			
		}



		break;

	case '+':
		token = { std::string(1,m_currentChar), asmc::TokenType::PLUS};
		break;

	case '$':
		nextChar();//skip $

		tokenStr = getSubStr(m_position, 1, std::isxdigit);

		token = {tokenStr, asmc::TokenType::DECNUMBER };

		break;

	case '"':
		nextChar();
		
		tokenStr = getSubStr(m_position, 1, std::isalnum);

		nextChar(); 

		token = { tokenStr, asmc::TokenType::STRING};
		
		break;

	case '\'':
		nextChar();
		token = { std::string(1, m_currentChar), asmc::TokenType::ASCII};
		nextChar();

		break;

	case ENDOFLINE:
		token = { std::string(1,m_currentChar), asmc::TokenType::ENDOFLINE };
		break;
	default:
		printError("LEXER Default CASE! ");
		f_error = true;
		return EMPTY_TOKEN;	
	}

	return token;
}

asmc::Token Lexer::lexWord()
{
	asmc::Token token;

	int startPos = m_position;
	int length = 1;
	while (std::isalpha(peek()))
	{
		nextChar();
		length++;
	}

	std::string tokenStr = m_program.substr(startPos, length);

	toUpper(tokenStr);

	if (checkIfKeyword(tokenStr))
	{
		std::optional<TokenType> enumVal = magic_enum::enum_cast<TokenType>(tokenStr);
		token = { tokenStr, enumVal.value() };
	}
	else
	{
		//label
		if (f_newline == true && peek() != ':')
		{
			printError("LEXER:: label must end with ':' ");
			
		}
		else if (peek() == ':')
		{
			token = { tokenStr, TokenType::LABEL };
			nextChar();

		}
		//variable
		else
		{
			if (m_lastToken.m_type >= asmc::TokenType::JAZ &&
				m_lastToken.m_type <= asmc::TokenType::JCF)
			{
				token = { tokenStr, TokenType::LABEL};
			}
			else if (m_lastToken.m_type == asmc::TokenType::FUNC)
			{
				token = { tokenStr, asmc::TokenType::FUNC_NAME };
			}
			else
			{
				token = { tokenStr, TokenType::ID};
			}
				
			//nextChar();

		}

	}

	return token;
}

asmc::Token Lexer::lexMacro()
{
	Token token;

	nextChar();

	std::string tokenStr = getSubStr(m_position, 1, std::isalpha);

	toUpper(tokenStr);

	if (checkIfKeyword(tokenStr))
	{
		std::optional<TokenType> enumVal = magic_enum::enum_cast<TokenType>(tokenStr);
		token = { tokenStr, enumVal.value() };
	}
	else
	{
		printError("macro is not in list");
	}

	return token;
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//

bool Lexer::checkIfKeyword(std::string token)
{
	std::optional<TokenType> tempToken = magic_enum::enum_cast<TokenType>(token);
	if (tempToken.has_value())
	{
		return true;
	}

	return false;
}

//is 0xfa
bool Lexer::isNumberHex()
{
	if (m_currentChar == '0' && peek() == 'x' && std::isxdigit(static_cast<uint8_t>(peekOverX())))
	{
		return true;
	}

	return false;
}

bool Lexer::isOperand()
{
	//isxdigit() checks next char is it hex?
	//register ?
	if (m_currentChar == 'r' && std::isdigit(static_cast<uint8_t>(peek())))
	{
		return true;
	}
	//is 0xfa
	else if (isNumberHex())
	{
		return true;
	}

	return false;
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void Lexer::skipComments()
{
	if (m_currentChar == ';')
	{
		while (m_currentChar != '\n' && m_currentChar != ENDOFLINE)
		{
			nextChar();
		}
		f_newline = true;
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
		if (m_currentChar == '\n')
		{
			f_newline = true;
		}
		
		nextChar();
	}
}

std::string Lexer::getSubStr(int startPos, int length, int (*cmpFunc)(int))
{

	//startPos = m_position;
	//length = 1;
	while (cmpFunc(peek()))
	{
		nextChar();
		length++;
	}

	std::string tokenStr = m_program.substr(startPos, length);

	toUpper(tokenStr);

	return tokenStr;
}

void Lexer::toUpper(std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = std::toupper(str[i]);
	}
}

//returns next char if pos >= program.length returns enum::EOF
void Lexer::nextChar()
{	
	m_position++;
	if (m_position >= m_program.length())
	{
		m_currentChar = ENDOFLINE;
	}
	else
	{
		m_currentChar = m_program[m_position];
	}

	if (m_currentChar == '\n')
	{
		m_lineNumber++;
	}
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//

void Lexer::printError(std::string message)
{
	std::cout << rang::fg::red
		<< "ERROR::" << message
		<< "current char[" << std::string(1, m_currentChar) <<"] "
		<< "current pos [" << std::to_string(m_position) << "] "
		<< "line number [" << m_lineNumber <<"]"
		<< rang::style::reset << "\n";

	f_error = true;
}

bool Lexer::getErrorFlag()
{
	return f_error;
}

}