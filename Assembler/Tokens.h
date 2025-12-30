#pragma once
namespace asmc
{
	
//address = @ff
//regAdr = @r4
//register = r4
//adr_p_reg = "@ff+r3"

//hexnumber = 0xfa
//decimal  = $255
//ascii = 'a'
//string = "abcde"
//directory = "abcdef.asm" | "abc.cpp" | "abc.c" ... "abc.*"

	enum TokenType
	{

		ENDOFFILE = -1,
		
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
		JNE,
		JE,

		JGT,
		JLT,		

		JUMP_INST,

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
		STRING,
		DIRECTORY,

		REGISTER,
		HEXNUMBER,
		ADDRESS,
		REGADR,
		ADR_P_REG,

		PLUS,
	};
}