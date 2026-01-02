#pragma once

#include "LexerPCH.h"

#include "../LibsLocal/magic_enum/magic_enum.hpp"
#include "../libsLocal/rang.hpp"

#include "Tokens.h"

#define MAX_TOKEN_LIST_SIZE 30
#define asmc_CLOSE_DEBUG_WORD "CLOSEDEB"

namespace asmc
{

struct Token
{
	std::string m_text = "";
	TokenType m_type = TokenType::EMPTY;

	bool operator==(const Token& other) const
	{
		return m_text == other.m_text && m_type == other.m_type;
	}
};

struct FileData
{
	std::string m_path;
	int m_lastPosition = 0;
	char m_currentChar = 0;
	size_t m_lineNumber = 0;
};

}

namespace std
{
	template<>
	struct hash<asmc::Token>
	{
		size_t operator() (const asmc::Token& key) const
		{
			return std::hash<std::string>()(key.m_text);
		}
	};
}

namespace asmc
{

class Lexer
{
public:
	
	Lexer(std::string path);
	
	
	[[nodiscard]] std::array<asmc::Token, MAX_TOKEN_LIST_SIZE> getTokenList();


	bool getErrorFlag();
	bool getDebugFlag();

	size_t m_lineNumber;

	bool isInputStreamEmpty();

	void pushFile(std::string path);

	//if input stream is empty return s true
	bool popFile();

	std::string getCurrentFileName();

private:

	[[nodiscard]] Token getToken();
	void toUpper(std::string& str);

	//sonraki karakteri isaret eder
	void nextChar();

	char peek();

	//is 0xfa
	bool isNumberHex();
	bool isOperand();

	void printError(std::string message);

	//bosluklari es gecer
	void skipWhiteSpace();
	//aciklama satiri ; sonrasindaki karakterleri es gecer
	void skipComments();
	//skip ',' '\n'
	void skipNonEssential();

	
	std::string readFile(std::string path);

	//returns sub part of m_program
	std::string getSubStr(int startPos, int length ,int (*cmpFunc)(int), bool upper = true);

	//str token enum icerisinde tanimlimi
	bool checkIfKeyword(std::string token);

	//------------------------------------------------------//
	//---------------------lex[X]()-------------------------//
	//------------------------------------------------------//

	//returns token starting with '.' .ORIGIN .DB
	[[nodiscard]] asmc::Token lexDotPart();

	//returns register hex part r0,r2,r3, .. => 0,2,3, ...	
	[[nodiscard]] asmc::Token lexRegPart();

	//0xff
	[[nodiscard]] asmc::Token lexHexNumberPart();

	//+,-,@,\n,EOF.... single char
	[[nodiscard]] asmc::Token lexSingleChar();

	//returns keywords LOAD,STR,ADD, LABEL or ID
	[[nodiscard]] asmc::Token lexWord();

	//returns token starting with '#' #define, #include
	[[nodiscard]] asmc::Token lexMacro();

	//------------------------------------------------------//
	//------------------------------------------------------//
	//------------------------------------------------------//

	
	int m_position;
	char m_currentChar;
	bool f_error;
	bool f_newline;
	bool fd_debug;

	std::array<asmc::Token, MAX_TOKEN_LIST_SIZE> m_tokenArr;

	std::string m_program;
	std::string m_currentFileName;
	asmc::Token m_lastToken;
	

	std::vector<FileData> m_inputStream;

	//label check
};

}