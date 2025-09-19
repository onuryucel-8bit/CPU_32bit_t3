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

	m_parserFuncs[0x1] = &asmc::Parser::parseLOAD;
	m_parserFuncs[0x2] = &asmc::Parser::parseSTR;
}

Parser::~Parser()
{
}

void Parser::moveCurrentToken()
{
	m_currentToken = m_peekToken;
	m_peekToken = m_lexer.getToken();
}


#ifdef PARSER_TEST_FUNCS
void Parser::printBinHex(std::bitset<32> opcode, std::bitset<32> operand)
{
	std::cout 
		<< opcode << " : " << std::hex << opcode.to_ulong() << "\n"
		<< operand << " : " << operand.to_ulong() << "\n"
		<< "m_currentToken.text[" << m_currentToken.m_text <<"]"
	<< "\n";
		
	std::cout << std::dec;
}

void Parser::printCurrentPeekToken()
{
	std::cout 
		<< "m_currentToken.text[" << m_currentToken.m_text << "]"
		<< "m_currentToken.type[" << magic_enum::enum_name(m_currentToken.m_type) << "]"
		<<"\n"
		<< "m_peekToken.text[" << m_peekToken.m_text << "]"
		<< "m_peekToken.type[" << magic_enum::enum_name(m_peekToken.m_type)<< "]"
	<< "\n";

	

	std::cout << std::dec;
}

#endif // PARSER_TEST_FUNCS

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

void Parser::printError(std::string message)
{
	std::cout << rang::fg::red
			  << "ERROR::Parser:: " << message 
			  << " line number [" << m_lexer.m_lineNumber << "]"
			  << " currentToken[" << m_currentToken.m_text <<"]"
			  << " type	[" << magic_enum::enum_name(m_currentToken.m_type) << "]"
			  << rang::style::reset
			  << "\n";
}

void Parser::printWarning(std::string message)
{
}

void Parser::program()
{

	m_parserFuncs[m_currentToken.m_type];

	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::LOAD:
		parseLOAD();
		break;

	case asmc::TokenType::ADD:
		parseADD();
		break;

	case asmc::TokenType::STR:
		parseSTR();
		break;

	case asmc::TokenType::LABEL:
		parseLabel();
		break;

	case asmc::TokenType::JMP:
		parseJMP();
		break;

	case asmc::TokenType::CALL:
		parseCALL();
		break;

	case asmc::TokenType::FUNC:
		parseFUNC();
		break;

	case asmc::TokenType::RET:
		parseRET();
		break;

	case asmc::TokenType::EMPTY:
		break;

	default:
		printError("Undefined token");
		break;
	}
}

void Parser::parseLOAD()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		std::cout << "HATA\n";
	}

	uint32_t opcode = 0x01;

	opcode <<= 24;

	moveCurrentToken();
	uint32_t registerPart = rdx::hexToDec(m_currentToken.m_text);

	registerPart <<= 18;
	opcode |= registerPart;

	moveCurrentToken();

	MemoryLayout memlay;

	memlay = { opcode, 0};

	uint32_t rx;

	switch (m_currentToken.m_type)
	{
		case asmc::TokenType::HEXNUMBER:
			
				opcode = asmc_CombineMODBits(opcode, asmc_MOD_Number);

				memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

				m_ramLocation += 2;
			break;

		case asmc::TokenType::ADDRESS:

				opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr);
			
				memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

				m_ramLocation += 2;
			break;

		case asmc::TokenType::REGADR:
				
				rx = opcode & 0x00ff'0000;//get rx part
				opcode = opcode & 0xff00'0000;//clear regx part

				opcode = opcode | (rx << 3);//shift rx part to left

				opcode = opcode | (rdx::hexToDec(m_currentToken.m_text) << 18);//get ry part shift left

				opcode = asmc_CombineMODBits(opcode, asmc_MOD_RegAdr);//insert mod bits

				memlay.m_opcode = opcode;

				m_ramLocation += 1;
			break;

		case asmc::TokenType::ADR_P_REG:
				
				//TODO substr
				
				/*
					adrPart = substrFunc();
					regPart = substrFunc();


				*/
				
				opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr_P_Reg);

				memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

				m_ramLocation += 2;
			break;
	}

	

	printBinHex(memlay.m_opcode, memlay.m_secondPart);

}

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

	registerPart <<= 21;
	opcode |= registerPart;


	moveCurrentToken();

	MemoryLayout memlay;

	switch (m_currentToken.m_type)
	{
		case asmc::TokenType::REGISTER:

			registerPart = rdx::hexToDec(m_currentToken.m_text);
			opcode = opcode | (registerPart << 18);
			
			opcode = opcode | (5 << 15);

			memlay = { opcode, 0};
			
			m_ramLocation += 1;
		break;
	}
	
	//printBinHex(memlay.m_opcode, 0);

}

void Parser::parseSTR()
{
	if (m_peekToken.m_type != asmc::TokenType::ADDRESS)
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

			secondPart <<= 18;
			opcode |= secondPart;

			opcode = opcode | (2 << 15);

			memlay = { opcode, address};
		
			m_ramLocation += 2;
		break;
	}

	//printBinHex(memlay.m_opcode, memlay.m_secondPart);

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

}
