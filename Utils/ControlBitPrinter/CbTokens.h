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
namespace cb
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

		PC_enb,
		PC_load,

		MDR_we,
		MDR_out,

		IR_we,
		IR_out,

		TEMP_we,
		TEMP_out,

		Posta_we,
		Posta_out,

		ADR_we,
		ADR_out,

		I3B,
		MAR_load,
		MAR_we,

		ACC_we,
		FR_cls,
		ALU_in,
		ALU_out,

		REG_out,
		REG_in,

		//----------------------//
		LPAREN,
		RPAREN,
		HASH,
		COMMENT,

		ENDOFFILE,
		UNKNOWN
	};

	struct Token
	{
		std::string m_text;
		cb::TokenType m_type;
	};

	

	

}


