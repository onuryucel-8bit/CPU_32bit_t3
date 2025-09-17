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
	/*std::cout << opcode << " : " << std::hex << opcode.to_ulong() << "\n"
			  << operand << " : " << operand.to_ulong() << "\n";
		
	std::cout << std::dec;*/
}

#endif // PARSER_TEST_FUNCS

void Parser::run()
{
	

	while (m_currentToken.m_type != asmc::TokenType::ENDOFLINE &&
		m_currentToken.m_type != asmc::TokenType::EMPTY)
	{
		if (m_currentToken.m_type != asmc::TokenType::NEWLINE)
		{	
			//std::cout << "-------\n";
			//std::cout << m_currentToken.m_text <<" " << magic_enum::enum_name(m_currentToken.m_type) << "\n";
			program();

			m_lineNumber;
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
		if (value.m_status == LabelStatus::Undefined)
		{
			//printError("Undefined label [" + key + "] line number [test]");
			//return;
		}
		else if (value.m_status == LabelStatus::NotUsed)
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
					m_symbolTable[key].m_status = LabelStatus::Used;

					//m_output.push_back(m_jumpTable[labelName]);
				}
			}

			//for warning
			if (value.m_status == LabelStatus::NotUsed)
			{
				//printWarning("Label not used[" + key + "]");
			}
		}
	}
}

void Parser::printError(std::string& message)
{
}

void Parser::printWarning(std::string& message)
{
}

void Parser::program()
{
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

	default:
		std::cout << rang::fg::red << "undefined token"<< rang::style::reset <<"\n";
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

	switch (m_currentToken.m_type)
	{
		case asmc::TokenType::HEXNUMBER:

			opcode = opcode | (1 << 15);
			memlay = { opcode, rdx::hexToDec(m_currentToken.m_text)};

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
	
	printBinHex(memlay.m_opcode, 0);

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

	printBinHex(memlay.m_opcode, memlay.m_secondPart);

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

}
