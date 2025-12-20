#include "Parser.h"
namespace asmc
{

Parser::Parser(asmc::Lexer& lexer)
	:m_lexer(lexer)
{	
	m_tokenIndex = 0;
	m_tokenList = m_lexer.getTokenList();

	m_peekToken = m_tokenList[0];

	moveCurrentToken();

	//asmc::TokenElement* elem = m_lexer.getTokenList();

	m_ramLocation = 0;
	m_lineNumber = 0;

	f_error = false;
	fd_printHexOutput = true;
	fd_scanTables = false;

	//29 used

	m_parserFuncs[asmc::TokenType::INCLUDE] = &asmc::Parser::parseINCLUDE;
	m_parserFuncs[asmc::TokenType::ORIGIN] = &asmc::Parser::parseORIGIN;
	m_parserFuncs[asmc::TokenType::DB] = &asmc::Parser::parseDB;
	m_parserFuncs[asmc::TokenType::DEFINE] = &asmc::Parser::parseDEFINE;

	//REG - RAM
	m_parserFuncs[asmc::TokenType::LOAD] = &asmc::Parser::parseLOAD;
	m_parserFuncs[asmc::TokenType::STR] = &asmc::Parser::parseSTR;
	m_parserFuncs[asmc::TokenType::MOV] = &asmc::Parser::parseMOV;

	//STACK
	m_parserFuncs[asmc::TokenType::CALL] = &asmc::Parser::parseCALL;
	m_parserFuncs[asmc::TokenType::RET]  = &asmc::Parser::parseRET;
	m_parserFuncs[asmc::TokenType::PUSH] = &asmc::Parser::parsePUSH;
	m_parserFuncs[asmc::TokenType::POP]  = &asmc::Parser::parsePOP;
	m_parserFuncs[asmc::TokenType::FUNC] = &asmc::Parser::parseFUNC;

	//ALU
	m_parserFuncs[asmc::TokenType::ADD] = &asmc::Parser::parseArithmeticPart;
	m_parserFuncs[asmc::TokenType::SUB] = &asmc::Parser::parseArithmeticPart;
	m_parserFuncs[asmc::TokenType::MUL] = &asmc::Parser::parseArithmeticPart;
	m_parserFuncs[asmc::TokenType::DIV] = &asmc::Parser::parseArithmeticPart;

	m_parserFuncs[asmc::TokenType::SHL] = &asmc::Parser::parseLogicPart;
	m_parserFuncs[asmc::TokenType::SHR] = &asmc::Parser::parseLogicPart;
	m_parserFuncs[asmc::TokenType::AND] = &asmc::Parser::parseLogicPart;
	m_parserFuncs[asmc::TokenType::OR]  = &asmc::Parser::parseLogicPart;
	m_parserFuncs[asmc::TokenType::XOR] = &asmc::Parser::parseLogicPart;

	m_parserFuncs[asmc::TokenType::NOT] = &asmc::Parser::parseNOT;
	m_parserFuncs[asmc::TokenType::CMP] = &asmc::Parser::parseCMP;

	//JUMP
	m_parserFuncs[asmc::TokenType::JMP] = &asmc::Parser::parseJMP;
	m_parserFuncs[asmc::TokenType::JAZ] = &asmc::Parser::parseJMP;
	m_parserFuncs[asmc::TokenType::JLZ] = &asmc::Parser::parseJMP;
	m_parserFuncs[asmc::TokenType::JGZ] = &asmc::Parser::parseJMP;
	m_parserFuncs[asmc::TokenType::JSC] = &asmc::Parser::parseJMP;
	m_parserFuncs[asmc::TokenType::JUC] = &asmc::Parser::parseJMP;
	m_parserFuncs[asmc::TokenType::JCT] = &asmc::Parser::parseJMP;
	m_parserFuncs[asmc::TokenType::JCF] = &asmc::Parser::parseJMP;

	m_parserFuncs[asmc::TokenType::LABEL] = &asmc::Parser::parseLabel;

	//-------------------------------------------------//
	//-------------------------------------------------//
	//-------------------------------------------------//
	
	//REG-RAM
	m_opcodeHexTable[asmc::TokenType::LOAD] = 0x01;
	m_opcodeHexTable[asmc::TokenType::STR] = 0x02;
	m_opcodeHexTable[asmc::TokenType::MOV] = 0x08;

	//STACK 
	m_opcodeHexTable[asmc::TokenType::CALL] = 0x03;
	m_opcodeHexTable[asmc::TokenType::RET] = 0x04;
	m_opcodeHexTable[asmc::TokenType::PUSH] = 0x5;
	m_opcodeHexTable[asmc::TokenType::POP] = 0x06;

	//ALU
	m_opcodeHexTable[asmc::TokenType::ADD] = 0x10;
	m_opcodeHexTable[asmc::TokenType::SUB] = 0x11;
	m_opcodeHexTable[asmc::TokenType::MUL] = 0x12;
	m_opcodeHexTable[asmc::TokenType::DIV] = 0x13;

	m_opcodeHexTable[asmc::TokenType::AND] = 0x14;
	m_opcodeHexTable[asmc::TokenType::OR] = 0x15;
	m_opcodeHexTable[asmc::TokenType::XOR] = 0x16;
	m_opcodeHexTable[asmc::TokenType::NOT] = 0x17;

	m_opcodeHexTable[asmc::TokenType::SHL] = 0x19;
	m_opcodeHexTable[asmc::TokenType::SHR] = 0x1A;
			
	m_opcodeHexTable[asmc::TokenType::CMP] = 0x18;

	//JUMP
	m_opcodeHexTable[asmc::TokenType::JMP] = 0x1B;
	m_opcodeHexTable[asmc::TokenType::JAZ] = 0x1C;
	m_opcodeHexTable[asmc::TokenType::JLZ] = 0x1D;
	m_opcodeHexTable[asmc::TokenType::JGZ] = 0x1E;
	m_opcodeHexTable[asmc::TokenType::JSC] = 0x1F;
	m_opcodeHexTable[asmc::TokenType::JUC] = 0x20;
	m_opcodeHexTable[asmc::TokenType::JCT] = 0x21;
	m_opcodeHexTable[asmc::TokenType::JCF] = 0x22;

}

Parser::~Parser()
{
}

//-------------------------------------------------//
//-------------------------------------------------//
//-------------------------------------------------//

void Parser::run()
{	
	//--------------------------------------------------------------//
	//parser
	//--------------------------------------------------------------//
	uti::Timer timer;

	std::cout << rang::bg::blue << "Running parser..." << rang::style::reset << "\n";

	while (m_tokenList[m_tokenIndex].m_type != asmc::TokenType::ENDOFFILE)
	{							
		program();
				
		if (m_peekToken.m_type == asmc::TokenType::ENDOFFILE)
		{
			//if input stream is empty close the loop
			if (m_lexer.popFile())
			{
				break;
			}
			else
			{
				moveCurrentToken();
			}						
		}
		
		moveCurrentToken();		
	}

	if (m_output.size() > 0)
	{

		std::cout << rang::fg::blue
			<< "printing m_symbolTable"
			<< rang::style::reset
			<< "\n";

		for (const auto& [key, value] : m_symbolTable)
		{
			//std::cout << '[' << key.m_text << "] status [" << magic_enum::enum_name(value.m_info) << "]" << " address[" << value.m_ramIndex << "]\n";
		}
	}

	//check for parser or lexer error
	if (f_error)
	{
		std::cout << rang::fg::red
			<< "Parser or lexer error detected"
			<< rang::style::reset
			<< "\n";		
		return;
	}

	//--------------------------------------------------------------//
	//check tables
	//--------------------------------------------------------------//


	checkTables();

	for (const auto& [key, value] : m_symbolTable)
	{
		//std::cout << '[' << key.m_text << "] status [" << magic_enum::enum_name(value.m_status) << "]" << " address[" << value.m_ramIndex << "]\n";
	}

	
	if (fd_printHexOutput)
	{
		std::cerr << rang::bg::green << "printBinHex() BEGIN"<< rang::style::reset << "\n";
		for (size_t i = 0; i < m_output.size(); i++)
		{
			printBinHex(m_output[i].m_opcode, m_output[i].m_secondPart);
		}
		std::cerr << rang::bg::green << "printBinHex() END" << rang::style::reset << "\n";
	}
	
	//if symbol info UNDEFINED
	if (f_error)
	{
		std::cout << rang::fg::red
			<< "Undefined symbol detected"
			<< rang::style::reset
			<< "\n";		
		return;
	}

	//--------------------------------------------------------------//
	//Generate output Emu/logisim RAM
	//--------------------------------------------------------------//


	if (m_ramLocation != 0)
	{
		std::cout << rang::bg::blue << "Generating output file..." << rang::style::reset << "\n";
		writeOutput();
	}	

	std::cerr <<"Total: " <<timer.getElapsedTime_milliseconds() << "ms passed..." << "\n";
}


void Parser::writeOutput()
{
	//sort std::vector
	std::sort(m_output.begin(), m_output.end(),
			  [](MemoryLayout& a, MemoryLayout& b)
			  {
				return a.m_ramIndex < b.m_ramIndex;
			  }
	);

	//--------------------------------------------------------------//
	//output Logisim RAM file
	//--------------------------------------------------------------//

	logisimPrinter.print(m_output);


	//--------------------------------------------------------------//
	//output EMU file
	//--------------------------------------------------------------//

	std::ofstream file("out.txt");

	if (file.is_open())
	{
		size_t usedBytes = 0;
		//TODO hold total bytes in member variable
		for (size_t i = 0; i < m_output.size(); i++)
		{
			usedBytes += m_output[i].m_packetSize;
		}

		file << usedBytes << "\n";

		//write as hex
		file << std::hex;
		for (size_t i = 0; i < m_output.size(); i++)
		{
			//write opcode's ram location " " opcode
			file << m_output[i].m_ramIndex << " " << m_output[i].m_opcode;
			
			//if opcode got second part
			if (m_output[i].m_packetSize == 2)
			{
				file << "\n" << m_output[i].m_ramIndex + 1 << " " << m_output[i].m_secondPart;								
			}

			file << "\n";
		}

		file.close();
	}
}

void Parser::checkTables()
{
	std::cout << rang::bg::blue << "checkTables()...." << rang::style::reset << "\n";

	for (const auto& [symKey, value] : m_symbolTable)
	{
		switch (value.m_status)
		{

			case asmc::LabelStatus::Undefined:
				printError("Undefined label/func");
			break;

			case asmc::LabelStatus::Called_NoRet:
				printError("Couldnt find RET");
			break;

			case asmc::LabelStatus::No_Call:
				printWarning("Func [" + symKey.m_text + "] was not called");
			break;
			
			case asmc::LabelStatus::No_Ret:
				printError("func(s) dont have RET");
			break;

			case asmc::LabelStatus::Called:
			case asmc::LabelStatus::NotUsed:
			case asmc::LabelStatus::Used:
				if (f_error == false || fd_scanTables == true)
				{
					//check jump table if label used
					for (const auto& [token, entry] : m_unresolvedTable)
					{
						if (token == symKey)
						{
							m_symbolTable[symKey].m_status = asmc::LabelStatus::Used;

							for (size_t i = 0; i < entry.size(); i++)
							{ 
								MemoryLayout memlay;

								memlay.m_opcode = entry[i].m_opcode;
								memlay.m_packetSize = entry[i].m_packetSize;
								memlay.m_ramIndex = entry[i].m_ramIndex;
								memlay.m_secondPart = m_symbolTable[symKey].m_ramIndex;

								m_output.push_back(memlay);
							}
							//combine label address with jmp instructions
							//std::cout << "ramIndex " << value.m_ramIndex << "\n";
							//m_jumpTable[labelName].m_secondPart = value.m_ramIndex;
							//m_symbolTable[key].m_status = asmc::LabelStatus::Used;

							//m_output.push_back(m_jumpTable[labelName]);
						}
					}

					//for warning
					if (value.m_status == asmc::LabelStatus::NotUsed)
					{
						//printWarning("Label not used[" + key + "]");
					}
				}

				if (value.m_status == asmc::LabelStatus::NotUsed)
				{
					switch (symKey.m_type)
					{
					case asmc::TokenType::LABEL:
						printWarning("found not used label definition file[" + value.m_fileName + "] line number[" + std::to_string(value.m_lineNumber) + "]");
						break;
					case asmc::TokenType::FUNC:
						printWarning("found not used func definition file[" + value.m_fileName + "] line number[" + std::to_string(value.m_lineNumber) + "]");
						break;
					}
					
				}
			break;

		}//switch end
	}//FOR end
}

void Parser::program()
{
	if (m_currentToken.m_type > asmc::TokenType::LABEL || m_currentToken.m_type < 0)
	{
		printError("Undefined token");
	}
	else
	{
		(this->*m_parserFuncs[m_currentToken.m_type])();
	}
}


//-------------------------------------------------//
//------------------PRINT_X()----------------------//
//-------------------------------------------------//

#ifdef PARSER_TEST_FUNCS

asmc::TokenType Parser::toToken(size_t opcode)
{
		switch (opcode)
		{

			// REG - RAM
			case 0x1:  return asmc::TokenType::LOAD;
			case 0x2:  return asmc::TokenType::STR;
			case 0x8:  return asmc::TokenType::MOV;

			// STACK
			case 0x3:  return asmc::TokenType::CALL;
			case 0x4:  return asmc::TokenType::RET;
			case 0x5:  return asmc::TokenType::PUSH;
			case 0x6:  return asmc::TokenType::POP;

			// ALU
			case 0x10: return asmc::TokenType::ADD;
			case 0x11: return asmc::TokenType::SUB;
			case 0x12: return asmc::TokenType::MUL;
			case 0x13: return asmc::TokenType::DIV;
			case 0x19: return asmc::TokenType::SHL;
			case 0x1a: return asmc::TokenType::SHR;

			case 0x14: return asmc::TokenType::AND;
			case 0x15: return asmc::TokenType::OR;
			case 0x17: return asmc::TokenType::NOT;
			case 0x16: return asmc::TokenType::XOR;

			case 0x18: return asmc::TokenType::CMP;

			// JUMP
			case 0x1b: return asmc::TokenType::JMP;
			case 0x1c: return asmc::TokenType::JAZ;
			case 0x1d: return asmc::TokenType::JLZ;
			case 0x1e: return asmc::TokenType::JGZ;
			case 0x1f: return asmc::TokenType::JSC;
			case 0x20: return asmc::TokenType::JUC;
			case 0x21: return asmc::TokenType::JCT;
			case 0x22: return asmc::TokenType::JCF;
		
			default: return asmc::TokenType::EMPTY;
		}
}

void Parser::printBinHex(std::bitset<32> opcode, std::bitset<32> operand)
{
	std::string str = opcode.to_string();
	
	size_t command = rdx::binToDec(str.substr(0, 8));

	auto commandStr = magic_enum::enum_name(toToken(command));

	std::cerr		
			<< commandStr << "\n"
			<<"oooooooo rrr rrr mmm\n"
			
			//opcode bits
			<< rang::fg::blue			
			<< str.substr(0,8) << " "
			<< rang::style::reset

			//reg bits
			<< rang::fg::yellow
			<< str.substr(8,3) << " "
						
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
		<< "========================================"
		<< "\n";

	std::cerr << std::dec;
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
	//TODO hold previous token for better error printing..

	std::cout << rang::fg::red		
		<< "ERROR::Parser:: " << message
		<< " |file name [" << m_lexer.getCurrentFileName() <<"]"
		<< " line number [" << m_lexer.m_lineNumber << "]"
		<< " currentToken.text[" << m_currentToken.m_text << "]"
		<< " type	[" << magic_enum::enum_name(m_currentToken.m_type) << "]"
		<< "\n##############################\n"
		<< rang::style::reset
		<< "\n";

	f_error = true;
}

void Parser::printWarning(std::string message)
{
	std::cout << rang::fg::yellow
		<< "WARNING::Parser:: " << message
		/*<< " line number [" << m_lexer.m_lineNumber << "]"
		<< " currentToken[" << m_currentToken.m_text << "]"
		<< " type	[" << magic_enum::enum_name(m_currentToken.m_type) << "]"*/
		<< rang::style::reset
		<< "\n";
}


//-------------------------------------------------//
//-------------------------------------------------//
//-------------------------------------------------//

void Parser::moveCurrentToken()
{
	m_currentToken = m_peekToken;

	m_tokenIndex++;
	if (m_tokenIndex >= m_tokenList.size())
	{
		m_tokenList = m_lexer.getTokenList();
		m_tokenIndex = 0;
	}

	m_peekToken = m_tokenList[m_tokenIndex];
}

MemoryLayout Parser::parseOperand(uint32_t opcode)
{
	MemoryLayout memlay;	

	uint32_t rx;

	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::ID:

		if (m_symbolTable.contains(m_currentToken))
		{
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Number);
			memlay.m_secondPart = m_symbolTable[m_currentToken].m_ramIndex;

			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 2; 
			m_ramLocation += 2;
		}
		else
		{
			printError("undefined macro used");
		}
			
		break;
	case asmc::TokenType::HEXNUMBER:
			
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Number);

			memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 2;
			m_ramLocation += 2;
		break;

	case asmc::TokenType::REGISTER:

			rx = opcode & 0x00ff'0000;//get rx part
			opcode = opcode & 0xff00'0000; //clear regx part

			opcode = opcode | (rx << 3);//shift rx part to left

			opcode = opcode | (rdx::hexToDec(m_currentToken.m_text) << asmc_ShiftAmount_RegB);//shift ry part to left

			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Rx_Ry);

			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 1;
			m_ramLocation += 1;
		break;

	case asmc::TokenType::ADDRESS:

			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr);

			memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);
			memlay.m_ramIndex = m_ramLocation;

			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 2;
			m_ramLocation += 2;
		break;

	case asmc::TokenType::REGADR:	

			rx = opcode & 0x00ff'0000;//get rx part
			opcode = opcode & 0xff00'0000; //clear regx part

			opcode = opcode | (rx << 3);//shift rx part to left

			opcode = opcode | (rdx::hexToDec(m_currentToken.m_text) << asmc_ShiftAmount_RegB);//shift ry part to left
			
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_RegAdr);

			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 1;
			m_ramLocation += 1;
		break;

	case asmc::TokenType::ADR_P_REG:
			
			PacketAdrPReg packet = getAdr_P_RegPart(m_currentToken.m_text);

			//insert reg ry
			rx = opcode & 0x00ff'0000;//get rx part
			opcode = opcode & 0xff00'0000;//clear regx part
			
			opcode = opcode | (rx << 3);//shift rx part to left

			opcode = opcode | (packet.m_regPart << asmc_ShiftAmount_RegB);//shift ry part to left 
		
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr_P_Reg);

			memlay.m_secondPart = packet.m_adrPart;
			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 2;
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


void Parser::parseORIGIN()
{
	if (m_peekToken.m_type != asmc::TokenType::HEXNUMBER)
	{
		printError("Expected hex number after .origin");
	}

	moveCurrentToken();

	m_ramLocation = rdx::hexToDec(m_currentToken.m_text);
}

void Parser::parseDB()
{
	if (m_peekToken.m_type != asmc::TokenType::HEXNUMBER)
	{
		printError("Expected hex number after .db");
	}

	moveCurrentToken();

	while (m_currentToken.m_type == asmc::TokenType::HEXNUMBER)
	{
		
		//TODO need optimization
		asmc::MemoryLayout memlay;

		memlay.m_opcode = rdx::hexToDec(m_currentToken.m_text);
		memlay.m_packetSize = 1;
		memlay.m_ramIndex = m_ramLocation;
		memlay.m_secondPart = 1;

		m_ramLocation += 1;
		m_output.push_back(memlay);

		moveCurrentToken();

		if (m_currentToken.m_type == asmc::TokenType::NEWLINE)
		{
			moveCurrentToken();
		}
	}
}

void Parser::parseDEFINE()
{
	if (m_peekToken.m_type != asmc::TokenType::ID)
	{
		printError("Expected identifier after #define");
	}
	//TODO process define
	moveCurrentToken();

	asmc::Token tempToken = m_currentToken;	
	moveCurrentToken();

	if (m_currentToken.m_type != asmc::TokenType::HEXNUMBER)
	{
		printError("macro value MUST be a hexnumber");
		return;
	}

	asmc::Symbol sym;

	// #define abc 0x54
	sym.m_fileName = m_lexer.getCurrentFileName();
	sym.m_lineNumber = m_lineNumber;
	//holds 0x54
	sym.m_ramIndex = rdx::hexToDec(m_currentToken.m_text);

	m_symbolTable[tempToken] = sym;
	
}

void Parser::parseINCLUDE()
{
	if (m_peekToken.m_type != asmc::TokenType::DIRECTORY)
	{
		printError("Expected directory");
		return;
	}	

	
	m_lexer.pushFile(m_peekToken.m_text);
	moveCurrentToken(); //currentToken is directory now

	//currentToken points the token in newfile
	//called in run()
	//moveCurrentToken();  
}

#pragma region REG_RAM

void Parser::parseLOAD()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{		
		printError("unexpected operand");
		return;
	}

	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::LOAD] << asmc_ShiftAmount_Opcode;

	moveCurrentToken();
	uint32_t registerPart = rdx::hexToDec(m_currentToken.m_text);

	if (registerPart > 7)
	{
		printError("Register(s) must be in range [0-7]");
	}

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

		m_output.push_back(memlay);
	}
	

}

void Parser::parseSTR()
{
	if (m_peekToken.m_type != asmc::TokenType::ADDRESS && 
		m_peekToken.m_type != asmc::TokenType::REGADR  &&
		m_peekToken.m_type != asmc::TokenType::ADR_P_REG)
	{
		printError("unexpected operand");
	}

	MemoryLayout memlay;
	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::STR] << asmc_ShiftAmount_Opcode;

	/*
		STR @ff,r1
		STR @r2,r1
		STR @ff+r2,r1
	*/
	moveCurrentToken();//@ff,@r2,@ff+r2

	uint32_t firstOperand = rdx::hexToDec(m_currentToken.m_text);

	switch (m_currentToken.m_type)
	{		
	case asmc::TokenType::ADDRESS:
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr);

			memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);		
			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 2;

			m_ramLocation += 2;
		break;

	case asmc::TokenType::REGADR:
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_RegAdr);

			opcode = opcode | (rdx::hexToDec(m_currentToken.m_text) << asmc_ShiftAmount_RegA);
			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 1;

			m_ramLocation += 1;
		break;

	case asmc::TokenType::ADR_P_REG:
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr_P_Reg);

			PacketAdrPReg packet = getAdr_P_RegPart(m_currentToken.m_text);

			opcode = opcode | (packet.m_regPart << asmc_ShiftAmount_RegA);

			memlay.m_secondPart = packet.m_adrPart;
			memlay.m_ramIndex = m_ramLocation;
			memlay.m_packetSize = 2;

			m_ramLocation += 2;
		break;
	}

	
	moveCurrentToken();

	opcode = opcode | ( rdx::hexToDec(m_currentToken.m_text) << asmc_ShiftAmount_RegB);
	
	memlay.m_opcode = opcode;	
	

	m_output.push_back(memlay);
	
}

void Parser::parseMOV()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("expected register for first operand");
	}

	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::MOV] << asmc_ShiftAmount_Opcode;
	
	moveCurrentToken();
	MemoryLayout memlay;

	uint32_t rx = rdx::hexToDec(m_currentToken.m_text);
	
	moveCurrentToken();
	if (m_currentToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("expected register for second operand");
		
	}
	else
	{		
		uint32_t ry = rdx::hexToDec(m_currentToken.m_text);

		if (rx > 7 || ry > 7)
		{
			printError("Registers must in range [0-7]");
		}

		rx <<= asmc_ShiftAmount_RegA;
		ry <<= asmc_ShiftAmount_RegB;

		opcode = (opcode | rx) | ry;
		
	}
	memlay.m_opcode = opcode;
	memlay.m_secondPart = 0;
	memlay.m_packetSize = 1;
	memlay.m_ramIndex = m_ramLocation;
	

	m_ramLocation += 1;

	m_output.push_back(memlay);
	
}

#pragma endregion

//-----------------ALU---------------------//

#pragma region ALU

void Parser::parseArithmeticPart()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("unexpected operand");
	}

	uint32_t opcode = 0;

	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::ADD:
		opcode = m_opcodeHexTable[asmc::TokenType::ADD];
		break;

	case asmc::TokenType::SUB:
		opcode = m_opcodeHexTable[asmc::TokenType::SUB];
		break;
	
	case asmc::TokenType::MUL:
		opcode = m_opcodeHexTable[asmc::TokenType::MUL];
		break;

	case asmc::TokenType::DIV:
		opcode = m_opcodeHexTable[asmc::TokenType::DIV];
		break;
	}

	opcode = opcode << asmc_ShiftAmount_Opcode;

	moveCurrentToken();

	if (m_peekToken.m_type == asmc::TokenType::EMPTY)
	{
		printError("detected empty token in arithmetic opcode");
	}

	uint32_t registerPart = rdx::hexToDec(m_currentToken.m_text);

	registerPart <<= asmc_ShiftAmount_RegB;
	opcode |= registerPart;

	moveCurrentToken();

	MemoryLayout memlay;

	memlay = parseOperand(opcode);

	m_output.push_back(memlay);
}

void Parser::parseLogicPart()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("Expected register as first operand");
	}

	uint32_t opcode = 0;

	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::AND:
		opcode = m_opcodeHexTable[asmc::TokenType::AND];
		break;

	case asmc::TokenType::OR:
		opcode = m_opcodeHexTable[asmc::TokenType::OR];
		break;

	case asmc::TokenType::XOR:
		opcode = m_opcodeHexTable[asmc::TokenType::XOR];
		break;

	case asmc::TokenType::SHL:
		opcode = m_opcodeHexTable[asmc::TokenType::SHL];
		break;

	case asmc::TokenType::SHR:
		opcode = m_opcodeHexTable[asmc::TokenType::SHR];
		break;
	}

	opcode = opcode << asmc_ShiftAmount_Opcode;

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

void Parser::parseNOT()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER &&
		m_peekToken.m_type != asmc::TokenType::ID)
	{
		printError("unexpected operand");
	}

	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::NOT] << asmc_ShiftAmount_Opcode;

	MemoryLayout memlay;
	moveCurrentToken();

	uint32_t registerPart = 0;
	if (m_currentToken.m_type == asmc::TokenType::REGISTER)
	{
		registerPart = rdx::hexToDec(m_currentToken.m_text);
	}
	//ID
	else
	{
		if (m_symbolTable.contains(m_currentToken))
		{
			registerPart = m_symbolTable[m_currentToken].m_ramIndex;
		}
		else
		{
			printError("undefined macro used at");
		}
		
	}

	if (registerPart > 7)
	{
		printError("register part MUST be in [0-7] range");
	}

	opcode = opcode | (registerPart << asmc_ShiftAmount_RegB);	

	memlay.m_opcode = opcode;
	memlay.m_ramIndex = m_ramLocation;
	memlay.m_packetSize = 1;

	m_ramLocation += 1;

	m_output.push_back(memlay);
}

void Parser::parseCMP()
{
	if (m_peekToken.m_type != asmc::TokenType::REGISTER)
	{
		printError("unexpected operand");
	}

	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::CMP] << asmc_ShiftAmount_Opcode;

	MemoryLayout memlay;

	moveCurrentToken();

	if (m_currentToken.m_type != asmc::REGISTER &&
		m_currentToken.m_type != asmc::REGADR   &&
		m_currentToken.m_type != asmc::ADDRESS)
	{
		printError("unexpected operand");
	}

	switch (m_peekToken.m_type)
	{
	case asmc::TokenType::REGADR:
	case asmc::TokenType::REGISTER:
			opcode = opcode | (rdx::hexToDec(m_currentToken.m_text) << asmc_ShiftAmount_RegA);

			moveCurrentToken();
			
			opcode = opcode | (rdx::hexToDec(m_currentToken.m_text) << asmc_ShiftAmount_RegB);
		break;

	case asmc::TokenType::ADDRESS:
			opcode = opcode | (rdx::hexToDec(m_currentToken.m_text) << asmc_ShiftAmount_RegB);

			moveCurrentToken();

			memlay.m_secondPart = rdx::hexToDec(m_currentToken.m_text);

		break;
	}

	memlay.m_opcode = opcode;
	memlay.m_ramIndex = m_ramLocation;
	memlay.m_packetSize = 2;
	
	
	m_ramLocation += 2;

	m_output.push_back(memlay);
	
}

#pragma endregion

//------------STACK--------------------//

#pragma region STACK

void Parser::parsePUSH()
{
	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::PUSH] << asmc_ShiftAmount_Opcode;

	moveCurrentToken();
	
	uint32_t rx;
	MemoryLayout memlay;

	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::REGISTER:

			rx = rdx::hexToDec(m_currentToken.m_text);

			opcode = opcode | (rx << asmc_ShiftAmount_RegB);
			
			//opcode = asmc_CombineMODBits(opcode, asmc_MOD_Rx_Ry);

			memlay.m_opcode = opcode;
			memlay.m_ramIndex = m_ramLocation;
			m_ramLocation += 1;			
		break;

	case asmc::TokenType::REGADR:
			rx = rdx::hexToDec(m_currentToken.m_text);

			opcode = opcode | (rx << asmc_ShiftAmount_RegB);

			opcode = asmc_CombineMODBits(opcode, asmc_MOD_RegAdr);

			memlay.m_opcode = opcode;
			memlay.m_ramIndex = m_ramLocation;
			m_ramLocation += 1;
		break;

	case asmc::TokenType::ADDRESS:
			//address
			rx = rdx::hexToDec(m_currentToken.m_text);
			
			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr);

			memlay.m_opcode = opcode;
			memlay.m_secondPart = rx;
			memlay.m_ramIndex = m_ramLocation;
			m_ramLocation += 2;
		break;

	case asmc::TokenType::ADR_P_REG:
				
			PacketAdrPReg packet = getAdr_P_RegPart(m_currentToken.m_text);
						
			opcode = opcode | (packet.m_regPart << asmc_ShiftAmount_RegB);//shift rx part									

			opcode = asmc_CombineMODBits(opcode, asmc_MOD_Adr_P_Reg);

			memlay.m_opcode = opcode;
			memlay.m_secondPart = packet.m_adrPart;
			memlay.m_ramIndex = m_ramLocation;
			m_ramLocation += 2;
		break;

	default:
			printError("unexpected operand");
		break;
	}

	m_output.push_back(memlay);


}

void Parser::parsePOP()
{
	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::POP] << asmc_ShiftAmount_Opcode;

	MemoryLayout memlay 
	{
		.m_opcode = opcode,
		.m_ramIndex = m_ramLocation,
		.m_packetSize = 1
	};	

	m_ramLocation += 1;

	m_output.push_back(memlay);
	
}

void Parser::parseCALL()
{
	if (m_peekToken.m_type != asmc::TokenType::ID)
	{
		printError("CALL must be followed by a function name");
	}

	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::CALL] << asmc_ShiftAmount_Opcode;	

	moveCurrentToken();

	//func is defined
	if (m_symbolTable.contains(m_currentToken))
	{
		m_symbolTable[m_currentToken].m_status = asmc::LabelStatus::Called;
		
		asmc::MemoryLayout memlay;

		memlay.m_opcode = opcode;
		memlay.m_packetSize = 2;
		memlay.m_ramIndex = m_ramLocation;
		memlay.m_secondPart = m_symbolTable[m_currentToken].m_ramIndex;

		m_output.push_back(memlay);

	}
	//func definition after the CALL command ?
	else
	{
		m_symbolTable[m_currentToken].m_status = asmc::LabelStatus::No_FuncDef;

		asmc::UnresolvedEntry entry;

		entry.m_opcode = opcode;
		entry.m_secondPart = -1;
		entry.m_ramIndex = m_ramLocation;
		entry.m_packetSize = 2;
		entry.m_fileName = m_lexer.getCurrentFileName();
		entry.m_lineNumber = m_lineNumber;
		entry.m_status = asmc::LabelStatus::No_Ret;
		
		m_unresolvedTable[m_currentToken].push_back(entry);
	}

	m_ramLocation += 2;

}

void Parser::parseFUNC()
{
	//printCurrentPeekToken();

	if (m_peekToken.m_type != asmc::TokenType::ID)
	{
		printError("FUNC must be followed by a function name");
	}

	moveCurrentToken();

	//is func definition in symbol table
	if (m_symbolTable.contains(m_currentToken))
	{
		//is func definition called?
		if (m_symbolTable[m_currentToken].m_status == asmc::LabelStatus::No_FuncDef)
		{
			m_symbolTable[m_currentToken].m_status = asmc::LabelStatus::Called_NoRet;

			//combine with CALL command
			for (const asmc::UnresolvedEntry& entry : m_unresolvedTable[m_currentToken])
			{
				asmc::MemoryLayout memlay;

				memlay.m_opcode = entry.m_opcode;
				memlay.m_packetSize = entry.m_packetSize;
				memlay.m_ramIndex = entry.m_ramIndex;
				memlay.m_secondPart = m_ramLocation;

				m_output.push_back(memlay);
			}
		}
		//else
			//print error func with same name twice or more
	}
	//Func called? later
	else
	{
		Symbol sym =
		{
			.m_ramIndex = m_ramLocation,
			.m_fileName = m_lexer.getCurrentFileName(),
			.m_lineNumber = m_lineNumber,
			.m_status = asmc::LabelStatus::No_Ret
		};

		m_symbolTable[m_currentToken] = sym;
	}
	
	m_lastFuncName = { m_currentToken.m_text, m_currentToken.m_type};

	
}

void Parser::parseRET()
{

	if (m_lastFuncName.m_text == "")
	{		
		printError("FUNC definition required before using RET");
	}

	uint32_t opcode = m_opcodeHexTable[asmc::TokenType::RET] << asmc_ShiftAmount_Opcode;

	MemoryLayout memlay;

	memlay.m_opcode = opcode;
	memlay.m_packetSize = 1;
	memlay.m_ramIndex = m_ramLocation;
	memlay.m_secondPart = 0;

	if (m_symbolTable.contains(m_lastFuncName))
	{
		m_symbolTable[m_lastFuncName].m_status = asmc::LabelStatus::No_Call;
	}

	m_lastFuncName.m_text = "";

	m_ramLocation += 1;
	m_output.push_back(memlay);		
}

#pragma endregion

//----------------JUMP----------------//

void Parser::parseLabel()
{

	if (m_symbolTable.contains(m_currentToken))
	{		
		//print error label defined twice or more
	}
	else
	{
		m_symbolTable[m_currentToken] = { m_ramLocation, m_lexer.getCurrentFileName(), m_lineNumber, asmc::LabelStatus::NotUsed };
	}
	
	m_lineNumber++;
}

void Parser::parseJMP()
{
	if (m_peekToken.m_type != asmc::TokenType::LABEL)
	{
		printError("Expected label after jump instruction");
	}

	uint32_t opcode = 0;
	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::JMP:
		opcode = m_opcodeHexTable[asmc::TokenType::JMP] << asmc_ShiftAmount_Opcode;
		break;

	case asmc::TokenType::JAZ:
		opcode = m_opcodeHexTable[asmc::TokenType::JAZ] << asmc_ShiftAmount_Opcode;
		break;

	case asmc::TokenType::JLZ:
		opcode = m_opcodeHexTable[asmc::TokenType::JLZ] << asmc_ShiftAmount_Opcode;
		break;

	case asmc::TokenType::JGZ:
		opcode = m_opcodeHexTable[asmc::TokenType::JGZ] << asmc_ShiftAmount_Opcode;
		break;

	case asmc::TokenType::JSC:
		opcode = m_opcodeHexTable[asmc::TokenType::JSC] << asmc_ShiftAmount_Opcode;
		break;

	case asmc::TokenType::JUC:
		opcode = m_opcodeHexTable[asmc::TokenType::JUC] << asmc_ShiftAmount_Opcode;
		break;

	case asmc::TokenType::JCT:
		opcode = m_opcodeHexTable[asmc::TokenType::JCT] << asmc_ShiftAmount_Opcode;
		break;

	case asmc::TokenType::JCF:
		opcode = m_opcodeHexTable[asmc::TokenType::JCF] << asmc_ShiftAmount_Opcode;
		break;
	}

	moveCurrentToken();

	if (m_symbolTable.contains(m_currentToken))
	{
		
		/*MemoryLayout memlay = m_symbolTable[m_currentToken].m_memlay;

		memlay.m_opcode = opcode;
		memlay.m_ramIndex = m_ramLocation;
		
		m_output.push_back(memlay);*/

		m_symbolTable[m_currentToken].m_status = asmc::LabelStatus::Used;

		MemoryLayout memlay;

		memlay.m_opcode = opcode;
		memlay.m_packetSize = 2;
		memlay.m_ramIndex = m_ramLocation;
		memlay.m_secondPart = m_symbolTable[m_currentToken].m_ramIndex;

		m_output.push_back(memlay);
	}
	else
	{
		//MemoryLayout memlay;

		//memlay.m_opcode = opcode;
		//memlay.m_ramIndex = m_ramLocation;
		//memlay.m_packetSize = 2;
		////memlay.m_secondPart

		//m_symbolTable[m_currentToken] = { memlay, asmc::LabelStatus::Undefined };

		/*m_symbolTable[m_currentToken].m_status = asmc::LabelStatus::Used;

		MemoryLayout memlay;

		memlay.m_opcode = opcode;
		memlay.m_secondPart = m_symbolTable[m_currentToken].m_ramIndex;
		memlay.m_ramIndex = m_ramLocation;
		memlay.m_packetSize = 2;

		m_output.push_back(memlay);*/

		asmc::UnresolvedEntry entry;

		entry.m_opcode = opcode;
		entry.m_secondPart = -1;
		entry.m_ramIndex = m_ramLocation;
		entry.m_packetSize = 2;
		entry.m_fileName = m_lexer.getCurrentFileName();
		entry.m_lineNumber = m_lineNumber;
		entry.m_status = asmc::LabelStatus::Undefined;

		m_unresolvedTable[m_currentToken].push_back(entry);

	}

	m_ramLocation += 2;
}

//------------------------------------//

}
