#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <cctype>
#include <cstdint>

#include "../LibsLocal/magic_enum/magic_enum.hpp"
#include "../libsLocal/rang.hpp"

#include "Tokens.h"

namespace asmc
{



struct Token
{
	std::string m_text = "";
	TokenType m_type = TokenType::EMPTY;
};

//gereksiz ?
#define EMPTY_TOKEN { "", asmc::TokenType::EMPTY }

class Lexer
{
public:
	
	Lexer(std::string program);
	
	[[nodiscard]] Token getToken();

	bool getErrorFlag();

	size_t m_lineNumber;

private:

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


	//returns sub part of m_program
	std::string getSubStr(int startPos, int length ,int (*cmpFunc)(int));

	//str token enum icerisinde tanimlimi
	bool checkIfKeyword(std::string token);

	//.ORIGIN .DB	
	[[nodiscard]] asmc::Token lexDotPart();
	//rx	
	[[nodiscard]] asmc::Token lexRegPart();
	//0xff
	[[nodiscard]] asmc::Token lexHexNumberPart();
	//+,-,@,\n,EOF.... single char
	[[nodiscard]] asmc::Token lexSingleChar();
	//keyword, label, jumploc
	[[nodiscard]] asmc::Token lexWord();
	//#define, #include
	[[nodiscard]] asmc::Token lexMacro();


	
	int m_position;
	char m_currentChar;
	bool f_error;
	bool f_newline;

	std::string m_program;
	asmc::Token m_lastToken;


	//label check
};

}