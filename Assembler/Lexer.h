#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <cctype>
#include <cstdint>

#include "../LibsLocal/magic_enum/magic_enum.hpp"
#include "../libsLocal/rang.hpp"


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

	INCLUDE,
	DEFINE,
	ID,
	LABEL ,

	//Operands
	REGISTER,
	HEXNUMBER,
	DECNUMBER,
	ASCII,
	STRING,
	
	ADDRESS,
	REGADR,
	ADR_P_REG,


	PLUS,
	

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

	FUNC,
	FUNC_NAME,
			
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
	JCF ,
	
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


	size_t m_lineNumber;
	int m_position;
	std::string m_program;
	char m_currentChar;

	asmc::Token m_lastToken;

	bool f_error;

	//label check
	bool f_newline;
};

}