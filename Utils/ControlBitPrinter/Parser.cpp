#include "Parser.h"

namespace asmc
{

Parser::Parser(asmc::Lexer& lexer)
	:m_lexer(lexer)
{	
	m_controlBitAdr = 0;	
}

Parser::~Parser()
{
}

void Parser::run()
{
	asmc::Token t = m_lexer.getToken();
	while (t.m_type != asmc::TokenType::ENDOFFILE)
	{
		if (t.m_type != asmc::TokenType::NEWLINE)
		{
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

				case asmc::TokenType::UNKNOWN:
					std::cout << "UNKNOWN token detected\n";
					break;

				default:
					std::cout << "ERROR:: Undefined token[" << (t.m_text) << "]\n";
					break;
			}
		}
		t = m_lexer.getToken();

	}

	writeToFile();
}

void Parser::calculateControlBits()
{ 

	m_controlBitAdr++;

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
			m_addressROM.push_back(m_controlBitAdr);

			
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
		addressRom << rdx::decToHex(m_addressROM[i]) << " ";

		if ((i + 1) % 16 == 0)
		{
			addressRom << "\n";
		}
	}

	addressRom.close();
}

}