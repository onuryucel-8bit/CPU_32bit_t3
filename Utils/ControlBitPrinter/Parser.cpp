#include "Parser.h"

namespace asmc
{

Parser::Parser(asmc::Lexer& lexer)
	:m_lexer(lexer)
{	
	m_romAdrIndex = 0;	


	addressVec.m_romIndex = 0;
	addressVec.m_value = 0;

	m_locationTable["LOAD"] = 0b0000'0001'001;
	m_locationTable["STR"] = 0b0000'0010'010;
	m_locationTable["MOV"] = 0b0000'1000'000;

	m_locationTable["ADD"] = 0b0001'0000'001;
	//TODO rx,sayi |rx,ry|rx,@adr|rx@ry|rx,@adr+ry
	m_locationTable["SUB"] = 0b0001'0001'001;
	m_locationTable["MUL"] = 0b0001'0010'001;
	m_locationTable["DIV"] = 0b0001'0011'001;
	m_locationTable["AND"] = 0b0001'0100'101;
	m_locationTable["OR"] =  0b0001'0101'101;
	m_locationTable["XOR"] = 0b0001'0110'101;
	m_locationTable["NOT"] = 0b0001'0111'101;
	m_locationTable["CMP"] = 0b0001'0111'010;
	m_locationTable["SHL"] = 0b0001'1001'000;
	m_locationTable["SHR"] = 0b0001'1010'000;

	m_locationTable["CALL"]= 0b0000'0011'000;
	m_locationTable["RET"] = 0b0000'0100'000;
	m_locationTable["PUSH"]= 0b0000'0101'001;
	m_locationTable["POP"] = 0b0000'0110'000;

	m_locationTable["JMP"] = 0b0001'1011'000;
	m_locationTable["JAZ"] = 0b0001'1100'000;
	m_locationTable["JLZ"] = 0b0001'1101'000;
	m_locationTable["JGZ"] = 0b0001'1110'000;

	m_locationTable["JSC"] = 0b0001'1111'000;
	m_locationTable["JUC"] = 0b0010'0000'000;
	m_locationTable["JCT"] = 0b0010'0001'000;
	m_locationTable["JCF"] = 0b0010'0010'000;
}

Parser::~Parser()
{
}

void Parser::run()
{
	asmc::Token t;
	std::array<asmc::Token, MAX_TOKEN_LIST_SIZE> tokenList = m_lexer.getTokenList();

	int i = 0;	
	while (tokenList[i].m_type != asmc::TokenType::ENDOFFILE)
	{
		
		t = tokenList[i];

		switch (t.m_type)
		{
		case asmc::TokenType::Read:
			m_sector.push_back(asmc::TokenType::Read);
			break;

		case asmc::TokenType::Write:
			m_sector.push_back(asmc::TokenType::Write);
			break;

			// PC
		case asmc::TokenType::PC_mux:
			m_sector.push_back(asmc::TokenType::PC_mux);
			break;

		case asmc::TokenType::PC_enb:
			m_sector.push_back(asmc::TokenType::PC_enb);
			break;

		case asmc::TokenType::PC_load:
			m_sector.push_back(asmc::TokenType::PC_load);
			break;

			// MDR
		case asmc::TokenType::MDR_we:
			m_sector.push_back(asmc::TokenType::MDR_we);
			break;

		case asmc::TokenType::MDR_out:
			m_sector.push_back(asmc::TokenType::MDR_out);
			break;

			// Instruction Register
		case asmc::TokenType::IR_we:
			m_sector.push_back(asmc::TokenType::IR_we);
			break;

		case asmc::TokenType::IR_out:
			m_sector.push_back(asmc::TokenType::IR_out);
			break;

			// TEMP
		case asmc::TokenType::TEMP_we:
			m_sector.push_back(asmc::TokenType::TEMP_we);
			break;

			// POSTA A/B
		case asmc::TokenType::POSTA_A_we:
			m_sector.push_back(asmc::TokenType::POSTA_A_we);
			break;

		case asmc::TokenType::POSTA_A_out:
			m_sector.push_back(asmc::TokenType::POSTA_A_out);
			break;

		case asmc::TokenType::POSTA_B_we:
			m_sector.push_back(asmc::TokenType::POSTA_B_we);
			break;

		case asmc::TokenType::POSTA_B_out:
			m_sector.push_back(asmc::TokenType::POSTA_B_out);
			break;

			// ADR
		case asmc::TokenType::ADR_we:
			m_sector.push_back(asmc::TokenType::ADR_we);
			break;

		case asmc::TokenType::ADR_out:
			m_sector.push_back(asmc::TokenType::ADR_out);
			break;

			// ALU
		case asmc::TokenType::I3B:
			m_sector.push_back(asmc::TokenType::I3B);
			break;

		case asmc::TokenType::ALU_in:
			m_sector.push_back(asmc::TokenType::ALU_in);
			break;

		case asmc::TokenType::ALU_out:
			m_sector.push_back(asmc::TokenType::ALU_out);
			break;

		case asmc::TokenType::ALU_mux:
			m_sector.push_back(asmc::TokenType::ALU_mux);
			break;

		case asmc::TokenType::ACC_we:
			m_sector.push_back(asmc::TokenType::ACC_we);
			break;

			// MAR
		case asmc::TokenType::MAR_load:
			m_sector.push_back(asmc::TokenType::MAR_load);
			break;

		case asmc::TokenType::MAR_we:
			m_sector.push_back(asmc::TokenType::MAR_we);
			break;

			// Flag register
		case asmc::TokenType::FR_cls:
			m_sector.push_back(asmc::TokenType::FR_cls);
			break;

			// Register file
		case asmc::TokenType::REG_out:
			m_sector.push_back(asmc::TokenType::REG_out);
			break;

		case asmc::TokenType::REG_we:
			m_sector.push_back(asmc::TokenType::REG_we);
			break;

		case asmc::TokenType::HASH:
			calculateControlBits();
			break;

		case asmc::TokenType::RPAREN:
			m_sector.push_back(asmc::TokenType::END);
			calculateControlBits();
			break;

		case asmc::TokenType::LPAREN:
		case asmc::TokenType::COMMENT:
		case asmc::TokenType::ENDOFFILE:
		case asmc::TokenType::NEWLINE:
			break;
	
		case asmc::TokenType::CONTROL_BIT_LOCATION:

			if (m_locationTable.contains(t.m_text))
			{				
				addressVec.m_value = m_locationTable[t.m_text];
			}
			break;

		case asmc::TokenType::UNKNOWN:
			std::cout << "UNKNOWN token detected\n";
			break;

		default:
			std::cout << "ERROR:: Parser() switch-case Undefined token[" << (t.m_text) << "]\n";
			break;

		}
				
		i++;

		if (i >= tokenList.size())
		{
			tokenList = m_lexer.getTokenList();
			i = 0;
		}
	}

	writeToFile();
}

void Parser::calculateControlBits()
{ 

	m_romAdrIndex++;

	int res = 0;
	for (size_t i = 0; i < m_sector.size(); i++)
	{
		if (m_sector[i] != asmc::TokenType::END)
		{
			res += std::pow(2, m_sector[i]);
		}
		//push END control bit
		else
		{			
			//TODO find a better solution...
			m_output.push_back(m_sector[i]);
			m_sector.clear();

			addressVec.m_romIndex = m_romAdrIndex;			

			m_addressROM.push_back(addressVec);

			
			return;
		}
	}
	//std::cout << "control bits hex : " << std::hex << res << "\n";		
	
	m_output.push_back(res);

	m_sector.clear();
}

void Parser::writeToFile()
{
	/*for (size_t i = 0; i < m_output.size(); i++)
	{
		std::cout << std::hex << m_output[i] << "\n";
	}*/

	std::ofstream file("output.txt");

	file << "v3.0 hex words plain\n";

	for (size_t i = 0; i < m_output.size(); i++)
	{
		file << rdx::decToHex(m_output[i]) << " ";

		if ((i + 1) % 16 == 0)
		{
			file << "\n";
		}
	}

	file.close();



	std::ofstream addressRom("AdrROM_output.txt");

	addressRom << "v3.0 hex words plain\n";

	for (int i = 0; i < 9; i++)
	{
		addressRom << "0 ";
	}

	for (size_t i = 0; i < m_addressROM.size(); i++)
	{
		addressRom << rdx::decToHex(m_addressROM[i].m_value) << " ";

		if ((i + 1) % 16 == 0)
		{
			addressRom << "\n";
		}
	}

	addressRom.close();
}

}