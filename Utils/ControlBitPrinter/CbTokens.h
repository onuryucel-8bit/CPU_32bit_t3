/*
{
	@aciklama satiri
	Read
	Write
	PC_enb
	PC_load
	#
	Write
	PC_enb
	PC_load
}
*/



//cb control bit
namespace asmc
{
	/*std::string TokenArr[] =
	{
		"Read","Write","PC_enb","PC_load",
		"MDR_we","MDR_out","IR_we","IR_out",
		"TEMP_we","TEMP_out","Posta_we","Posta_out",
		"ADR_we","ADR_out","I3B","MAR_load",

		"MAR_we","ACC_we","FR_cls","ALU_in",
		"ALU_out","REG_out","REG_in","}",
		"{","#","@"
	};*/

	enum TokenType
	{
		Read,
		Write,
		
		//PC
		PC_mux,
		PC_enb,
		PC_load,

		//MDR
		MDR_we,
		MDR_out,

		//Instruction Reg
		IR_we,
		IR_out,

		//TEMP
		TEMP_we,
		TEMP_out,

		//POSTA a/b
		POSTA_A_we,
		POSTA_A_out,

		POSTA_B_we,
		POSTA_B_out,

		//ADR reg
		ADR_we,
		ADR_out,

		//ALU
		I3B,
		ALU_in,
		ALU_out,
		ALU_mux,
		ACC_we,

		//MAR
		MAR_load,
		MAR_we,
		
		//Flag reg
		FR_cls,

		//Register file
		REG_out,
		REG_in,
		REG_inSelect,
		REG_outSelect,
		
		
		//----------------------//
		LPAREN,
		RPAREN,
		HASH,
		COMMENT,

		//WARNING lexer m_currentChar is char type if you get weird error, its caused by char overflow
		ENDOFFILE,
		UNKNOWN,
		NEWLINE,
		//TODO define end bits in logisim/cpu
		END = 0xff,

	};
}


