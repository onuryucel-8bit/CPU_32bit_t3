#pragma once

#include <string>
#include "../libsLocal/magic_enum/magic_enum.hpp"

namespace asmc
{
	enum TokenType
	{

		ENDOFFILE = -1,

		LET,
		IF,
		THEN,
		ELSE,
		END,
		WHILE,		
		PRINT,
		FUNCTION,
		GOTO,
		LABEL,
		BUZZ,
		DRAW,

		STRING,			//"abc"
		CHAR,			//'a'
		NUMBER,			//125
		
		PLUS,			// +
		MINUS,			// -
		ASTERISK,		// *
		SLASH,			// /

		SEMICOLON, 		// ;
		RPAREN,			// )
		LPAREN,			// (
		RBRACE,			// ]
		LBRACE,			// [
		RCPAREN,		// }
		LCPAREN,		// {

		ASSIGN,			// =
		LESS_THAN,		// <
		LESS_EQ,		// <=
		GREATER_THAN,	// >
		GREATER_EQ,		// >=
		NOT_EQ,			// !=
		EQEQ,			// ==

		SHL,			// <<
		SHR,			// >>
		NOT,			// !
		AND,			// &
		OR,				// |


		
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
		ADD,
		SUB,
		MUL,
		DIV,
		XOR,

		CMP,


		//JUMP
		JMP,
		JAZ,
		JLZ,

		JGZ,
		JSC,
		JUC,

		JCT,
		JCF,

		DEFINE,
		ORIGIN,
		INCLUDE,
		DB,	//define byte

		

		EMPTY,
		NEWLINE,
		ID,

		//Operands

		DECNUMBER,	
		DIRECTORY,

		REGISTER,
		HEXNUMBER,
		ADDRESS,
		REGADR,
		ADR_P_REG,		
		
		
	};


	struct Token
	{
		std::string m_text = "";
		asmc::TokenType m_type = TokenType::EMPTY;
		size_t m_lineNumber = 0;

		bool operator==(const Token& other) const
		{
			return m_text == other.m_text && m_type == other.m_type;
		}

		//DEBUG
		friend std::ostream& operator<<(std::ostream& os, const asmc::Token& token)
		{
			return os << "lineNumber: [" << token.m_lineNumber << "] text[" + token.m_text + "], token[" + std::string(magic_enum::enum_name(token.m_type)) << +"]";
		}
	};
}