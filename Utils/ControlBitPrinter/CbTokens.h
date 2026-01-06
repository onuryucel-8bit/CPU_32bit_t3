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

		//POSTA a/b
		POSTA_A_we,
		POSTA_A_out,

		POSTA_B_we,
		POSTA_B_out,

		//MAR
		MAR_load,
		MAR_we,

		//ADR reg
		ADR_we,
		ADR_out,
		ADR_mux,

		//ALU
		I3B,
		ALU_in,
		ALU_out,
		ALU_mux,
		ACC_we,
				
		//Sp++
		SP_inc,
		
		
		//Register file
		REG_out,			
		REG_we,
		REG_data_out,
		REG_path_in,				
		DATA_to_adr,

		TEMP_out,
		SP_updown,
		

		END,		
		//----------------------//
		PC_toDataBus,


		NOP,
		CONTROL_BIT_LOCATION,
		LPAREN,
		RPAREN,
		HASH,
		COMMENT,
						
		UNKNOWN,
		NEWLINE,		
		
		//WARNING lexer m_currentChar is char type if you get weird error, its caused by char overflow
		//?? 05.01.2025 need a time machine
		ENDOFFILE
	};
}


