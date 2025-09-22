#include "Parser.h"
namespace asmc
{

Parser::Parser(asmc::Lexer& lexer)
	:m_lexer(lexer)
{
	m_peekToken = m_lexer.getToken();
	moveCurrentToken();

	m_ramLocation = 0;

	m_lineNumber = 0;

	//REG - RAM
	m_parserFuncs[asmc::TokenType::LOAD] = &asmc::Parser::parseLOAD;
	m_parserFuncs[asmc::TokenType::STR] = &asmc::Parser::parseSTR;
	m_parserFuncs[asmc::TokenType::MOV] = &asmc::Parser::parseMOV;

	//STACK
	m_parserFuncs[asmc::TokenType::CALL] = &asmc::Parser::parseCALL;
	m_parserFuncs[asmc::TokenType::RET] = &asmc::Parser::parseRET;
	m_parserFuncs[asmc::TokenType::PUSH] = &asmc::Parser::parsePUSH;
	m_parserFuncs[asmc::TokenType::POP] = &asmc::Parser::parsePOP;
	m_parserFuncs[asmc::TokenType::FUNC] = &asmc::Parser::parseFUNC;

	//ALU
	m_parserFuncs[asmc::TokenType::ADD] = &asmc::Parser::parseADD;
	m_parserFuncs[asmc::TokenType::SUB] = &asmc::Parser::parseSUB;
	m_parserFuncs[asmc::TokenType::MUL] = &asmc::Parser::parseMUL;
	m_parserFuncs[asmc::TokenType::DIV] = &asmc::Parser::parseDIV;
	m_parserFuncs[asmc::TokenType::SHL] = &asmc::Parser::parseSHL;
	m_parserFuncs[asmc::TokenType::SHR] = &asmc::Parser::parseSHR;

	m_parserFuncs[asmc::TokenType::AND] = &asmc::Parser::parseAND;
	m_parserFuncs[asmc::TokenType::OR] = &asmc::Parser::parseOR;
	m_parserFuncs[asmc::TokenType::NOT] = &asmc::Parser::parseNOT;
	m_parserFuncs[asmc::TokenType::XOR] = &asmc::Parser::parseXOR;

	m_parserFuncs[asmc::TokenType::CMP] = &asmc::Parser::parseCMP;

	//JUMP
	m_parserFuncs[asmc::TokenType::JMP] = &asmc::Parser::parseJMP;
	m_parserFuncs[asmc::TokenType::JAZ] = &asmc::Parser::parseJAZ;
	m_parserFuncs[asmc::TokenType::JLZ] = &asmc::Parser::parseJLZ;
	m_parserFuncs[asmc::TokenType::JGZ] = &asmc::Parser::parseJGZ;
	m_parserFuncs[asmc::TokenType::JSC] = &asmc::Parser::parseJSC;
	m_parserFuncs[asmc::TokenType::JUC] = &asmc::Parser::parseJUC;
	m_parserFuncs[asmc::TokenType::JCT] = &asmc::Parser::parseJCT;
	m_parserFuncs[asmc::TokenType::JCF] = &asmc::Parser::parseJCF;

}

Parser::~Parser()
{
}

//-------------------------------------------------//
//-------------------------------------------------//
//-------------------------------------------------//

void Parser::run()
{
	

	while (m_currentToken.m_type != asmc::TokenType::ENDOFLINE)
	{
		if (m_currentToken.m_type != asmc::TokenType::NEWLINE)
		{	
			//std::cout << "-------\n";
			//std::cout << m_currentToken.m_text <<" " << magic_enum::enum_name(m_currentToken.m_type) << "\n";
			program();

			
		}
		
		moveCurrentToken();

	}

	std::cout << rang::fg::blue
		<< "printing m_symbolTable"
		<< rang::style::reset
		<< "\n";

	for (const auto& [key, value] : m_symbolTable)
	{
		std::cout << '[' << key << "] status [" << magic_enum::enum_name(value.m_status) << "]" << " address[" << value.m_ramIndex << "]\n";
	}


	checkTables();

	for (const auto& [key, value] : m_symbolTable)
	{
		std::cout << '[' << key << "] status [" << magic_enum::enum_name(value.m_status) << "]" << " address[" << value.m_ramIndex << "]\n";
	}

}

void Parser::checkTables()
{
	std::cout << rang::bg::blue << "Starting second pass..." << rang::style::reset << "\n";

	for (const auto& [key, value] : m_symbolTable)
	{
		if (value.m_status == asmc::LabelStatus::Undefined)
		{
			//printError("Undefined label [" + key + "] line number [test]");
			//return;

			std::cout << "undefined label/func\n";
		}
		else if (value.m_status == asmc::LabelStatus::Called_Noc)
		{
			std::cout << "hata RET yok\n";
		}
		else if (value.m_status == asmc::LabelStatus::NotUsed)
		{
			//printWarning("Not used label[" + key + "]");

			//check jump table if label used
			for (const auto& [labelName, memLayout] : m_jumpTable)
			{
				if (labelName == key)
				{
					//combine label address with jmp instructions
					//std::cout << "ramIndex " << value.m_ramIndex << "\n";
					m_jumpTable[labelName].m_secondPart = value.m_ramIndex;
					m_symbolTable[key].m_status = asmc::LabelStatus::Used;

					//m_output.push_back(m_jumpTable[labelName]);
				}
			}



			//for warning
			if (value.m_status == asmc::LabelStatus::NotUsed)
			{
				//printWarning("Label not used[" + key + "]");
			}
		}
	}
}

void Parser::program()
{
	if (m_currentToken.m_type >= asmc::TokenType::LABEL || m_currentToken.m_type < 0)
	{
		printError("Undefined token");
	}
	else
	{
		(this->*m_parserFuncs[m_currentToken.m_type])();
	}
	for (size_t i = 0; i < m_output.size(); i++)
	{
		printBinHex(m_output[i].m_opcode, m_output[i].m_secondPart);
	}
	
}

//-------------------------------------------------//
//------------------PRINT_X()----------------------//
//-------------------------------------------------//

#ifdef PARSER_TEST_FUNCS
void Parser::printBinHex(std::bitset<32> opcode, std::bitset<32> operand)
{
	std::string str = opcode.to_string();

	std::cout
		<< rang::bg::green
		<< "printBinHex()\n"
		<< rang::style::reset
			
			<<"oooooooo rrr rrr mmm\n"
			
			//opcode bits
			<< rang::fg::blue			
			<< str.substr(0,8) << " "
			<< rang::style::reset

			//reg bits
			<< rang::fg::yellow
			<< str.substr(8,3) << " "
			<< rang::style::reset

			<< rang::fg::yellow
			<< str.substr(11, 3) << " "
			<< rang::style::reset

			//mod bits
			<< rang::fg::cyan
			<<  str.substr(14,3) << " "
			<< rang::style::reset

			//empty bits
			<< str.substr(17, str.length())

		<< " : " << std::hex << opcode.to_ulong() << "\n"
		<< operand << " : " << operand.to_ulong() << "\n"
		<< "m_currentToken.text[" << m_currentToken.m_text << "]\n"
		<< "----------------"
		<< "\n";

	std::cout << std::dec;
}

void Parser::printCurrentPeekToken()
{
	std::cout		
		<< "printCurrentPeekToken()\n"		
		<< "m_currentToken.text[" << m_currentToken.m_text << "]"
		<< "m_currentToken.type[" << magic_enum::enum_name(m_currentToken.m_type) << "]"
		<< "\n"
		<< "m_peekToken.text[" << m_peekToken.m_text << "]"
		<< "m_peekToken.type[" << magic_enum::enum_name(m_peekToken.m_type) << "]"		
		<< "\n";

}

#endif // PARSER_TEST_FUNCS

void Parser::printError(std::string message)
{
	std::cout << rang::fg::red
		<< "ERROR::Parser:: " << message
		<< " line number [" << m_lexer.m_lineNumber << "]"
		<< " currentToken[" << m_currentToken.m_text << "]"
		<< " type	[" << magic_enum::enum_name(m_currentToken.m_type) << "]"
		<< rang::style::reset
		<< "\n";
}

void Parser::printWarning(std::string message)
{
	std::cout << rang::fg::yellow
		<< "WARNING::Parser:: " << message
		<< " line number [" << m_lexer.m_lineNumber << "]"
		<< " currentToken[" << m_currentToken.m_text << "]"
		<< " type	[" << magic_enum::enum_name(m_currentToken.m_type) << "]"
		<< rang::style::reset
		<< "\n";
}

//-------------------------------------------------//
//-------------------------------------------------//
//-------------------------------------------------//

void Parser::moveCurrentToken()
{
	m_currentToken = m_peekToken;
	m_peekToken = m_lexer.getToken();
}

MemoryLayout Parser::parseOperand(uint32_t opcode)
{
	MemoryLayout memlay{ opcode, 0 };	

	uint32_t rx;

	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::HEXNUMBER:
			
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Number);

			memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

			m_ramLocation += 2;
		break;

	case asmc::TokenType::REGISTER:

			rx = rdx::hexToDec(m_currentToken.m_text);
			opcode = opcode | (rx << 18);

			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Rx_Ry);

			m_ramLocation += 1;
		break;

	case asmc::TokenType::ADDRESS:

			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr);

			memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

			m_ramLocation += 2;
		break;

	case asmc::TokenType::REGADR:		

		m_ramLocation += 1;
		break;

	case asmc::TokenType::ADR_P_REG:

		opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr_P_Reg);

		PacketAdrPReg packet = getAdr_P_RegPart(m_currentToken.m_text);

		//insert reg ry
		rx = opcode & 0x00ff'0000;//get rx part
		opcode = opcode & 0xff00'0000;//clear regx part

		opcode = opcode | (rx << 3);//shift rx part to left

		opcode = opcode | (packet.m_regPart << 18);//shift ry part to left 
		
		memlay.m_secondPart = packet.m_adrPart;

		m_ramLocation += 2;

		break;
	}

	memlay.m_opcode = opcode;

	return memlay;
}

PacketAdrPReg Parser::getAdr_P_RegPart(std::string& operand)
{
	//~~same thing as Lexer::getSubStr(int,int,int (*)(int))
	int index = 0;

	int startPos = 0;
	int length = 1;

	while (index + 1 < operand.length() && std::isxdigit(operand[index + 1]) )
	{
		index++;
		length++;
	}

	std::string addressPart = operand.substr(startPos, length);	
	

	startPos = index + 2;//cursor points the register number	
	length = 1;

	while (index + 1 < operand.length() && std::isdigit(operand[index + 1]))
	{
		index++;
		length++;
	}

	std::string regPart = operand.substr(startPos, length);

	PacketAdrPReg retVal;
	if (addressPart.length() <= 8)
	{
		retVal =
		{
			.m_adrPart = rdx::hexToDec(addressPart),
			.m_regPart = rdx::hexToDec(regPart)
		};
	}
	else
	{
		retVal = { 0,0 };
		printError("address exceeds 32bit limit");
	}
	

	return retVal;
}
 
//-------------REG/RAM---------------------//
void Parser::parseLOAD()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		std::cout << "HATA\n";
	}

	uint32_t opcode = 0x01 << asmc_ShiftAmount_Opcode;

	moveCurrentToken();
	uint32_t registerPart = rdx::hexToDec(m_currentToken.m_text);

	registerPart <<= asmc_ShiftAmount_RegB;
	opcode |= registerPart;

	moveCurrentToken();

	MemoryLayout memlay;
	if (m_currentToken.m_type == asmc::TokenType::REGISTER)
	{
		printError("unexpected operand type");
	}
	else
	{
		memlay = parseOperand(opcode);
	}
	

	//switch (m_currentToken.m_type)
	//{
	//	case asmc::TokenType::HEXNUMBER:
	//		
	//			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Number);

	//			memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

	//			m_ramLocation += 2;
	//		break;

	//	case asmc::TokenType::ADDRESS:

	//			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr);
	//		
	//			memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

	//			m_ramLocation += 2;
	//		break;

	//	case asmc::TokenType::REGADR:
	//			
	//			rx = opcode & 0x00ff'0000;//get rx part
	//			opcode = opcode & 0xff00'0000;//clear regx part

	//			opcode = opcode | (rx << 3);//shift rx part to left

	//			opcode = opcode | (rdx::hexToDec(m_currentToken.m_text) << 18);//get ry part shift left

	//			opcode = asmc_CombineMODBits(opcode, asmc_MOD_RegAdr);//insert mod bits

	//			memlay.m_opcode = opcode;

	//			m_ramLocation += 1;
	//		break;

	//	case asmc::TokenType::ADR_P_REG:
	//					
	//			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr_P_Reg);

	//			PacketAdrPReg packet = getAdr_P_RegPart(m_currentToken.m_text);
	//			
	//			//insert reg ry
	//			rx = opcode & 0x00ff'0000;//get rx part
	//			opcode = opcode & 0xff00'0000;//clear regx part

	//			opcode = opcode | (rx << 3);//shift rx part to left

	//			opcode = opcode | (packet.m_regPart << 18);//shift ry part to left 


	//			memlay.m_opcode = opcode;
	//			memlay.m_secondPart = packet.m_adrPart;

	//			m_ramLocation += 2;
	//		break;
	//}

	printBinHex(memlay.m_opcode, memlay.m_secondPart);

}

void Parser::parseSTR()
{
	if (m_peekToken.m_type != asmc::TokenType::ADDRESS && 
		m_peekToken.m_type != asmc::TokenType::ADR_P_REG)
	{
		std::cout << "HATA\n";
	}


	uint32_t opcode = 0x2;

	opcode <<= 24;

	moveCurrentToken();
	uint32_t address = rdx::hexToDec(m_currentToken.m_text);

	moveCurrentToken();

	MemoryLayout memlay;

	uint32_t secondPart = 0;

	switch (m_currentToken.m_type)
	{
		case asmc::TokenType::REGISTER:
				secondPart = rdx::hexToDec(m_currentToken.m_text);

				opcode = opcode | (secondPart << 18);

				opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr);
				
				memlay = { opcode, address};
		
				m_ramLocation += 2;
			break;

		case asmc::TokenType::REGADR:
				secondPart = rdx::hexToDec(m_currentToken.m_text);

				

				m_ramLocation += 1;
			break;

		case asmc::TokenType::ADR_P_REG:
				secondPart = rdx::hexToDec(m_currentToken.m_text);

				opcode = opcode | (secondPart << 18);

				opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr_P_Reg);

				memlay = { opcode, address };
				m_ramLocation += 2;
			break;
	}

	printBinHex(memlay.m_opcode, memlay.m_secondPart);

}

void Parser::parseMOV()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("expected register for first operand");
	}

	uint32_t opcode = 0xff << asmc_ShiftAmount_Opcode;
	
	moveCurrentToken();
	uint32_t rx = rdx::hexToDec(m_currentToken.m_text);

	moveCurrentToken();
	if (m_currentToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("expected register for second operand");
		
	}
	else
	{
		uint32_t ry = rdx::hexToDec(m_currentToken.m_text);

		rx <<= asmc_ShiftAmount_RegA;
		ry <<= asmc_ShiftAmount_RegB;

		opcode = (opcode | rx) | ry;

		printBinHex(opcode, 0);
	}

	
	
}

//-----------------ALU---------------------//

void Parser::parseADD()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		std::cout << "HATA\n";
	}

	uint32_t opcode = 0x10;

	opcode <<= 24;

	moveCurrentToken();
	uint32_t registerPart = rdx::hexToDec(m_currentToken.m_text);
	
	registerPart <<= asmc_ShiftAmount_RegB;
	opcode |= registerPart;

	moveCurrentToken();

	MemoryLayout memlay;

	memlay = parseOperand(opcode);

	/*switch (m_currentToken.m_type)
	{
		case asmc::TokenType::REGISTER:

			registerPart = rdx::hexToDec(m_currentToken.m_text);
			opcode = opcode | (registerPart << 18);
			
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Rx_Ry);

			memlay = { opcode, 0};
			
			m_ramLocation += 1;
		break;
	}*/
	
	m_output.push_back(memlay);	

}

void Parser::parseSUB()
{


}

void Parser::parseDIV()
{
}

void Parser::parseMUL()
{
}

void Parser::parseSHL()
{
}

void Parser::parseSHR()
{
}

void Parser::parseAND()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("Expected register as first operand");
	}


	uint32_t opcode = 0x01 << asmc_ShiftAmount_Opcode;

	moveCurrentToken();
	uint32_t registerPart = rdx::hexToDec(m_currentToken.m_text);

	registerPart <<= asmc_ShiftAmount_RegB;
	opcode |= registerPart;

	moveCurrentToken();

	MemoryLayout memlay;
	if (m_currentToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("unexpected operand type");
	}
	else
	{
		memlay = parseOperand(opcode);
		m_output.push_back(memlay);
	}
}

void Parser::parseOR()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("Expected register as first operand");
	}

	uint32_t opcode = 0x01 << asmc_ShiftAmount_Opcode;

	moveCurrentToken();
	uint32_t registerPart = rdx::hexToDec(m_currentToken.m_text);

	registerPart <<= asmc_ShiftAmount_RegB;
	opcode |= registerPart;

	moveCurrentToken();

	MemoryLayout memlay;
	if (m_currentToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("unexpected operand type");
	}
	else
	{
		memlay = parseOperand(opcode);
		m_output.push_back(memlay);

	}
}

void Parser::parseXOR()
{
}

void Parser::parseNOT()
{
}

void Parser::parseCMP()
{
}

//-----------------------------------//

void Parser::parsePUSH()
{
	uint32_t opcode = 0x01 << asmc_ShiftAmount_Opcode;

	moveCurrentToken();
	
	uint32_t rx;
	MemoryLayout memlay{ opcode, 0 };

	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::REGISTER:

			rx = rdx::hexToDec(m_currentToken.m_text);

			opcode = opcode | (rx << asmc_ShiftAmount_RegB);
			
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Rx_Ry);

			memlay.m_opcode = opcode;

			m_output.push_back(memlay);
			m_ramLocation += 1;			
		break;

	case asmc::TokenType::REGADR:
			rx = rdx::hexToDec(m_currentToken.m_text);

			opcode = opcode | (rx << asmc_ShiftAmount_RegB);

			opcode = asmc_CombineMODBits(opcode, asmc_MOD_RegAdr);

			memlay.m_opcode = opcode;

			m_output.push_back(memlay);
			m_ramLocation += 1;
		break;

	case asmc::TokenType::ADDRESS:
			//address
			rx = rdx::hexToDec(m_currentToken.m_text);
			
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr);

			memlay.m_opcode = opcode;
			memlay.m_secondPart = rx;
			
			m_output.push_back(memlay);
			m_ramLocation += 2;
		break;

	default:
			printError("unexpected operand");
		break;
	}


}

void Parser::parsePOP()
{
	uint32_t opcode = 0x01 << asmc_ShiftAmount_Opcode;

	MemoryLayout memlay{ opcode, 0};

	m_output.push_back(memlay);
	
}

void Parser::parseLabel()
{
	if (m_symbolTable.contains(m_currentToken.m_text) && 
		(m_symbolTable[m_currentToken.m_text].m_status == LabelStatus::NotUsed || 
			m_symbolTable[m_currentToken.m_text].m_status == LabelStatus::Used))
	{
		//m_lineNumber++;
		//printError("statement():: Label [" + m_currentToken.m_text + "] defined twice or more \n");
		
	}
	else
	{
		m_symbolTable[m_currentToken.m_text] = { m_ramLocation, LabelStatus::NotUsed };
	}
}

void Parser::parseCALL()
{
	if (m_peekToken.m_type != asmc::TokenType::ID)
	{
		std::cout << "parse CALL hata\n";
	}

	uint32_t opcode = 0xff;

	opcode <<= 24;

	moveCurrentToken();

	if (!m_symbolTable.contains(m_currentToken.m_text))
	{
		m_symbolTable[m_currentToken.m_text] = { -1, LabelStatus::Undefined };

		m_jumpTable[m_currentToken.m_text] =
		{
			.m_opcode = opcode,
			.m_secondPart = 0

		};

		
	}

	m_ramLocation += 2;

}

void Parser::parseFUNC()
{
	//printCurrentPeekToken();

	if (m_peekToken.m_type != asmc::TokenType::FUNC_NAME)
	{
		std::cout << "parse FUNC hata\n";
	}

	moveCurrentToken();

	if (m_symbolTable.contains(m_currentToken.m_text))
	{
		m_symbolTable[m_currentToken.m_text].m_ramIndex = m_ramLocation;
		m_symbolTable[m_currentToken.m_text].m_status = asmc::LabelStatus::Called_Noc;
		
	}

	m_lastFuncName = m_currentToken.m_text;

	//TODO FUNC mult donus RET komutu yok ise hata versin

}

void Parser::parseRET()
{

	if (m_lastFuncName == "")
	{
		std::cout << "RET qwrqwrqw hata\n";
	}

	if (m_symbolTable.contains(m_lastFuncName))
	{
		m_symbolTable[m_lastFuncName].m_status = asmc::LabelStatus::Called;

		m_lastFuncName = "";
	}

	

}


//----------------JUMP----------------//

void Parser::parseJMP()
{
	if (m_peekToken.m_type != asmc::TokenType::LABEL)
	{
		std::cout << "hata\n";
	}

	uint32_t opcode = 0xff;

	opcode <<= 24;

	moveCurrentToken();

	if (!m_symbolTable.contains(m_currentToken.m_text))
	{
		m_symbolTable[m_currentToken.m_text] = { -1, LabelStatus::Undefined };


		m_jumpTable[m_currentToken.m_text] =
		{
			.m_opcode = opcode,
			.m_secondPart = 0

		};


	}
	else
	{
		m_symbolTable[m_currentToken.m_text].m_status = asmc::LabelStatus::Used;

		MemoryLayout memlay;

		memlay.m_opcode = opcode;
		memlay.m_secondPart = m_symbolTable[m_currentToken.m_text].m_ramIndex;

		printBinHex(memlay.m_opcode, 0);


	}

	m_ramLocation += 2;
}

void Parser::parseJAZ()
{
}

void Parser::parseJLZ()
{
}

void Parser::parseJGZ()
{
}

void Parser::parseJSC()
{
}

void Parser::parseJUC()
{
}

void Parser::parseJCT()
{
}

void Parser::parseJCF()
{
}

//------------------------------------//

}
