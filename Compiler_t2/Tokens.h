#pragma once
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
		IDENT,
		PRINT,
		FUNCTION,

		STRING,			//"abc"
		NUMBER,			//125
		
		PLUS,			// +
		MINUS,			// -
		ASTERISK,		// *
		SLASH,			// /

		SEMICOLON, 		// ;
		RPAREN,			// (
		LPAREN,			// )
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

		LABEL,

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
}