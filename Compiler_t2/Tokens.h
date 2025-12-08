#pragma once
namespace asmc
{
	enum TokenType
	{

		ENDOFFILE = -1,

		LET,
		IF,
		THEN,
		END,
		WHILE,
		IDENT,
		STRING,


		NUMBER,
		
		PLUS,			// +
		MINUS,			// -


		ASTERISK,		// *
		SLASH,			// /
		EQUAL, 			// =
		SEMICOLON, 		// ;
		
		
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
		SHL,
		SHR,

		AND,
		OR,
		NOT,
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
		ASCII,
		DIRECTORY,

		REGISTER,
		HEXNUMBER,
		ADDRESS,
		REGADR,
		ADR_P_REG,		
		
		
	};
}