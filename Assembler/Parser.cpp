#include "Parser.h"
namespace asmc
{

Parser::Parser(asmc::Lexer& lexer)
	:m_lexer(lexer)
{
	m_peekToken = m_lexer.getToken();
	moveCurrentToken();
}

Parser::~Parser()
{
}

void Parser::moveCurrentToken()
{
	m_currentToken = m_peekToken;
	m_peekToken = m_lexer.getToken();
}

void Parser::run()
{
	

	while (m_currentToken.m_type != asmc::TokenType::ENDOFLINE &&
		m_currentToken.m_type != asmc::TokenType::EMPTY)
	{
		if (m_currentToken.m_type != asmc::TokenType::NEWLINE)
		{	
			std::cout << "-------\n";
			std::cout << m_currentToken.m_text <<" " << magic_enum::enum_name(m_currentToken.m_type) << "\n";
			program();
		}

		moveCurrentToken();

	}
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
		break;
	}

	std::bitset<32> output{ memlay.m_opcode };
	std::bitset<32> output2{ memlay.m_secondPart };

	std::cout << output << "\n"
		<< output2 <<"\n";

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
		break;
	}
	
	std::bitset<32> output{ memlay.m_opcode };
	
	std::cout << output << "\n";

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
		break;
	}

	std::bitset<32> output{ memlay.m_opcode };
	std::bitset<32> adr{ memlay.m_secondPart };

	std::cout << output << "\n" << adr << "\n";
}

}
