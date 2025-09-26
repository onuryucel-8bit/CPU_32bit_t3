#include "Lexer.h"
namespace asmc 
{

Lexer::Lexer(std::string path)
{

	FileData filedData;

	filedData.m_path = path;

	m_inputStream.push_back(filedData);

	m_program = readFile(path);


	m_position = -1;
	m_currentChar = 0;
	m_lineNumber = 0;
	

	f_error = false;
	f_newline = true;

	nextChar();
}

std::string Lexer::readFile(std::string path)
{
	std::fstream file(path);

	if (!file.is_open())
	{
		std::cout << "ERROR:: couldnt open the file\n";
		return "";
	}

	std::stringstream ss;

	ss << file.rdbuf();

	return ss.str();
}

Token Lexer::getToken()
{
	skipWhiteSpace();
	skipComments();
	skipNonEssential();
	//TODO bosluklari es gecme mekanizmasi ekle
	skipWhiteSpace();


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

//--------------------------------------------//
//-------------lexPartX()---------------------//
//--------------------------------------------//

asmc::Token Lexer::lexDotPart()
{
	asmc::Token token;

	std::string tokenStr = getSubStr(m_position + 1, 0, std::isalpha);

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
	asmc::Token token;

	nextChar();
	if (isxdigit(static_cast<uint8_t>(peek())))
	{		
		printError("invalid reg operand it should be r[0-7]\n");
		//empty token

		token = { std::string(1,m_currentChar), asmc::TokenType::EMPTY};

		return token;
	}

	
	return token = { std::string(1,m_currentChar), asmc::TokenType::REGISTER };
}

asmc::Token Lexer::lexHexNumberPart()
{
	asmc::Token token;

	nextChar();//skip 0
	nextChar();//skip x
	
	std::string tokenStr = getSubStr(m_position, 1, std::isxdigit);

	return token = {tokenStr, asmc::TokenType::HEXNUMBER};
}

asmc::Token Lexer::lexSingleChar()
{
	asmc::Token token;

	std::string tokenStr;

	switch (m_currentChar)
	{
	
	case '\n':
		//std::cout << "LEXER newline detected\n";
		token = { std::string(1,m_currentChar), asmc::TokenType::NEWLINE };
		f_newline = true;		
		
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
			
			nextChar();//move cursor to '+' or ' '
			
			skipWhiteSpace();

			//peek() == '+' || '-'
			if (m_currentChar == '+' || m_currentChar == '-')
			{				
				tokenStr += m_currentChar;
				
				nextChar();//move cursor to 'r' or ' '
				skipWhiteSpace();
				
				if (isOperand())
				{
					nextChar();//get register hex value
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
		
		tokenStr = getSubStr(m_position, 1, std::isalnum, false);

		nextChar(); 

		//is it "abcd.asm" path?
		if (m_currentChar == '.')
		{
			tokenStr += m_currentChar;

			nextChar();
			while (m_currentChar != '"')
			{
				tokenStr += m_currentChar;
				nextChar();
			}
			nextChar();//skip "

			token = { tokenStr, asmc::TokenType::DIRECTORY};
		}
		else
		{
			token = { tokenStr, asmc::TokenType::STRING };
		}		
		
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
		//f_error = true;
		break;
	}

	return token;
}

asmc::Token Lexer::lexWord()
{
	asmc::Token token;
	
	std::string tokenStr = getSubStr(m_position, 1, std::isalpha);

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
			if (m_lastToken.m_type >= asmc::TokenType::JMP &&
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
	asmc::Token token;

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
	char peekoverX;

	if (m_position + 2 >= m_program.length())
	{
		peekoverX = ENDOFLINE;
	}
	else
	{
		peekoverX = m_program[m_position + 2];
	}
	

	if (m_currentChar == '0' && peek() == 'x' && std::isxdigit(static_cast<uint8_t>(peekoverX)))
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

	//TODO aciklama satiri icerisinde / olunca hata veriyor duzelt bunu 
	if (m_currentChar == '/')
	{
		if (peek() == '*')
		{
			nextChar();//*
			nextChar();// \n
			while (m_currentChar != asmc::TokenType::ENDOFLINE && m_currentChar != '*' && peek() != '/')
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
		if (m_currentChar == '\n')
		{
			f_newline = true;
		}
		
		nextChar();
	}
}

void Lexer::pushFile(std::string path)
{	
	for (size_t i = 0; i < m_inputStream.size(); i++)
	{
		if (m_inputStream[i].m_path == path)
		{
			std::cout << rang::fg::red 
					  << "ERROR::Same file name detected in inputStream"
					  << rang::style::reset
					  <<"\n";
			return;
		}
	}

	asmc::FileData& filed = m_inputStream.back();

	filed.m_currentChar = m_currentChar;
	filed.m_lastPosition = m_position;
	filed.m_lineNumber = m_lineNumber;

	m_inputStream.push_back({ path });
	

	m_program = readFile(path);
	m_currentChar = 0;
	m_lineNumber = 0;
	m_position = -1;

	nextChar();

}

bool Lexer::popFile()
{		
	m_inputStream.pop_back();

	if (m_inputStream.empty())
	{
		return true;
	}

	asmc::FileData filed;
	filed = m_inputStream.back();

	//open file
	m_program = readFile(filed.m_path);
	m_currentChar = filed.m_currentChar;
	m_position = filed.m_lastPosition;
	m_lineNumber = filed.m_lineNumber;	

	return false;
}

std::string Lexer::getCurrentFileName()
{
	return m_inputStream.back().m_path;
}

std::string Lexer::getSubStr(int startPos, int length, int (*cmpFunc)(int), bool upper)
{
	while (cmpFunc(peek()))
	{
		nextChar();
		length++;
	}

	std::string tokenStr = m_program.substr(startPos, length);

	if (upper)
	{
		toUpper(tokenStr);
	}
	

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

	//for printError
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
	std::cout << rang::fg::cyan
		<< "*****************************\n"
		<< "ERROR::Lexer::" << message
		<< "lastToken [" << m_lastToken.m_text <<"] "
		<< "current char[" << std::string(1, m_currentChar) <<"] "
		<< "current pos [" << std::to_string(m_position) << "] "
		<< "line number [" << m_lineNumber <<"]\n"
		<< "*****************************\n"
	<< rang::style::reset;

	f_error = true;
}

bool Lexer::getErrorFlag()
{
	return f_error;
}

bool Lexer::isInputStreamEmpty()
{
	return m_inputStream.empty();
}

}