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
	enum TokenType
	{

		ENDOFLINE = -1,

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
		JAZ,
		JLZ,

		JGZ,
		JSC,
		JUC,

		JCT,
		JCF,

		EMPTY,
		LABEL,

		ORIGIN,
		DB,	//define byte
		NEWLINE,

		INCLUDE,
		DEFINE,
		ID,
		
		//Operands
		
		DECNUMBER,
		ASCII,
		STRING,

		REGISTER,
		HEXNUMBER,
		ADDRESS,
		REGADR,
		ADR_P_REG,

		PLUS,
	};
}