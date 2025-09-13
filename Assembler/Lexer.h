#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <cctype>
#include <cstdint>

#include "../LibsLocal/magic_enum/magic_enum.hpp"
#include "../libsLocal/rang.hpp"

#undef OUT // ..rang.hpp/windows.h  name collision

namespace asmc
{

//address = @ff
//regAdr = @r4
//register = r4
//hexnumber = 0xfa
//decimal  = 255
enum TokenType
{
	
	ENDOFLINE = -1,

	ORIGIN = 0x52,	
	DB,	//define byte
	NEWLINE,
	DEFINE,


	LABEL ,
	JUMPLOC ,

	//Operands
	REGISTER,
	HEXNUMBER,
	DECNUMBER,
	ADDRESS,
	REGADR,

	//Opcodes

	//REG - RAM
	LOAD,
	STR,
	MOV,

	//STACK
	CALL,
	RET,
	PUSH,
	POP,

			
	//ALU
	ADD ,
	SUB ,
	MUL ,
	DIV ,
	SHL ,
	SHR ,
		
	AND ,
	OR  ,
	NOT ,
	XOR ,

	CMP ,
		
	
	//JUMP
	JMP ,
	JAZ ,
	JLZ ,

	JGZ ,
	JSC ,
	JUC ,

	JCT ,
	JMT ,
	
	EMPTY
};

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


private:

	void toUpper(std::string& str);

	//sonraki karakteri isaret eder
	void nextChar();

	char peek();
	//0x4f peeks over 'x' returns 4
	char peekOverX();

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


	int m_position;
	std::string m_program;
	char m_currentChar;

	bool f_error;

	//label check
	bool f_newline;
};

}