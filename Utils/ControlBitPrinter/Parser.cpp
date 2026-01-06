#include "Parser.h"

namespace asmc
{

Parser::Parser(asmc::Lexer& lexer)
	:m_lexer(lexer)
{	
	m_controlRomIndex = 0;	


	m_addressVec.m_selfAdr = 0;
	m_addressVec.m_controlBitStartPos = 0;

	//TODO make them hex
	m_locationTable["LOAD"]= 0x01;
	m_locationTable["STR"] = 0x02;
	m_locationTable["MOV"] = 0x08;

	m_locationTable["ADD"] = 0x10;
	m_locationTable["SUB"] = 0x11;
	m_locationTable["MUL"] = 0x12;
	m_locationTable["DIV"] = 0x13;
	m_locationTable["AND"] = 0x14;
	m_locationTable["OR"] =  0x15;
	m_locationTable["XOR"] = 0x16;
	m_locationTable["NOT"] = 0x17;
	m_locationTable["CMP"] = 0x18;
	m_locationTable["SHL"] = 0x19;
	m_locationTable["SHR"] = 0x1a;
										
	m_locationTable["CALL"]= 0x03;
	m_locationTable["RET"] = 0x04;
	m_locationTable["PUSH"]= 0x05;
	m_locationTable["POP"] = 0x06;
										 
	m_locationTable["JMP"] = 0x1b;	
	m_locationTable["JNE"] = 0x1c;
	m_locationTable["JE"] =  0x1d;
	m_locationTable["JGT"] = 0x1e;
	m_locationTable["JLT"] = 0x1f;
}

Parser::~Parser()
{
}

void Parser::run()
{
	asmc::Token t;
	std::array<asmc::Token, MAX_TOKEN_LIST_SIZE> tokenList = m_lexer.getTokenList();
	
	std::string opcode;

	size_t location = 0;

	uint32_t offset = 0;
	int i = 0;	
	while (tokenList[i].m_type != asmc::TokenType::ENDOFFILE)
	{
		
		t = tokenList[i];

		switch (t.m_type)
		{

#pragma region CASE_T0
		case asmc::TokenType::PC_toDataBus:
			m_sector.push_back(asmc::TokenType::PC_toDataBus);
			break;

		case asmc::TokenType::SP_inc:
			m_sector.push_back(asmc::TokenType::SP_inc);
			break;

		case asmc::TokenType::SP_updown:
			m_sector.push_back(asmc::TokenType::SP_updown);
			break;

		case asmc::TokenType::TEMP_out:
			m_sector.push_back(asmc::TokenType::TEMP_out);
			break;

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

		case asmc::TokenType::ADR_mux:
			m_sector.push_back(asmc::TokenType::ADR_mux);
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

			// Register file
		case asmc::TokenType::REG_out:
			m_sector.push_back(asmc::TokenType::REG_out);
			break;

		case asmc::TokenType::REG_we:
			m_sector.push_back(asmc::TokenType::REG_we);
			break;

		case asmc::TokenType::REG_path_in:
			m_sector.push_back(asmc::TokenType::REG_path_in);
			break;

		case asmc::TokenType::REG_data_out:
			m_sector.push_back(asmc::TokenType::REG_data_out);
			break;

		case asmc::TokenType::DATA_to_adr:
			m_sector.push_back(asmc::TokenType::DATA_to_adr);
			break;
#pragma endregion
		
		case asmc::TokenType::NOP:
			m_sector.push_back(asmc::TokenType::NOP);
			calculateControlBits();
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

			//substr [COMMAND modbit]
			//[LOAD 1]

			//LOAD
			opcode = t.m_text.substr(0, t.m_text.find(' '));
			//1
			offset = (uint32_t)std::stoi(t.m_text.substr(t.m_text.find(' ') + 1, t.m_text.length() - 1));


			if (m_locationTable.contains(opcode))
			{				
				//start of control bit location adr
				location = m_locationTable[opcode];

				location <<= 3;

				//command type value
				//[LOAD mod]
				//0000_0001 + 011 => 1011
				location += offset;
				/*				 
								[opcode]   [mod]
				load rx,0xff => 0000_0001 + 001 => 0000_1001 control bits pointer location
				load rx,@adr => 0000_0001 + 010 => 0000_1010
				load rx,@ry  => 0000_0001 + 011 => 0000_1011
				
				add rx,ry    => 0001_0000 + 101 => 1000_0101
				*/
				
				asmc::adrVec tempVec;
				//control bits pointer value
				 
				//ptr_bits = LOAD_type_rx_0xff_location, add_type_rx_ry_location... etc
				tempVec.m_controlBitStartPos = m_controlRomIndex;
				tempVec.m_selfAdr = location;
				
				m_addressROM.push_back(tempVec);
			}
			else
			{
				std::cout << "ERROR:: invalid CONTROL_BIT_LOCATION found\n";
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
	
	size_t res = 0;
	for (size_t i = 0; i < m_sector.size(); i++)
	{
		if (m_sector[i] == asmc::TokenType::NOP)
		{
			//res = 0;
		}
		else if (m_sector[i] != asmc::TokenType::END)
		{
			res += std::pow(2, m_sector[i]);
		}
		//push END control bit
		else
		{			
			//TODO find a better solution...
			m_output.push_back(m_sector[i]);
			m_sector.clear();
			
			//std::cout << "romADR: " << std::hex << m_controlRomIndex << "|" << std::dec << m_controlRomIndex << "\n";
			m_controlRomIndex++;
			return;
		}
	}
	//std::cout << "control bits hex : " << std::hex << res << "\n";		
	
	m_output.push_back(res);

	m_sector.clear();

	m_controlRomIndex++;
}

void Parser::writeToFile()
{
	/*for (size_t i = 0; i < m_output.size(); i++)
	{
		std::cout << std::hex << m_output[i] << "\n";
	}*/

	std::ofstream file("output.txt");

	file << "v3.0 hex words plain\n";

	file << std::hex;
	for (size_t i = 0; i < m_output.size(); i++)
	{
		file << m_output[i] << " ";

		if (m_output[i] == asmc::TokenType::END)
		{
			file << "\n";
		}

		/*if ((i + 1) % 16 == 0)
		{
			file << "\n";
		}*/
	}

	file.close();

	/*std::cout << "m_addressRom[i].m_selfAdr\n";
	for (const auto& elem : m_addressROM)
	{
		std::cout << elem.m_selfAdr << "\n";
	}*/

	file.open("AdrROM_output.txt");
	

	file << "v3.0 hex words plain\n";

	file << "\n";
	
	//sort std::vector
	std::sort(m_addressROM.begin(), m_addressROM.end(),
		[](adrVec& a, adrVec& b)
		{
			return a.m_selfAdr < b.m_selfAdr;
		}
	);

	int romIndex = 0;
	for (size_t i = 0; i < m_addressROM.size(); i++)
	{
		size_t target = m_addressROM[i].m_selfAdr;

		//Fill gap with zeros
		while (romIndex < target)
		{
			file << "0 ";			

			romIndex++;
			if (romIndex % 9 == 0)
			{
				file << "\n";				
			}
		}
		
		file << m_addressROM[i].m_controlBitStartPos << " ";
		
		romIndex++;
		if (romIndex % 9 == 0)
		{
			file << "\n";			
		}
	}

	file.close();
}

}