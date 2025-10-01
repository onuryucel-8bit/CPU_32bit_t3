

class InputOutput
{
public:
	InputOutput();
	~InputOutput();

private:

};

InputOutput::InputOutput()
{
}

InputOutput::~InputOutput()
{
}

class RAM : InputOutput
{
public:
	RAM();
	~RAM();

private:

};

RAM::RAM()
{
}

RAM::~RAM()
{
}

class ROM : InputOutput
{
public:
	ROM();
	~ROM();

private:

};

ROM::ROM()
{
}

ROM::~ROM()
{
}

class Graphics : InputOutput
{
public:
	Graphics();
	~Graphics();

private:

};

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

class Teletype : InputOutput
{
public:
	Teletype();
	~Teletype();

private:

};

Teletype::Teletype()
{
}

Teletype::~Teletype()
{
}

class Keyboard : InputOutput
{
public:
	Keyboard();
	~Keyboard();

private:

};

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Cpu.h"
#include "utils/Radix.h"


void print()
{
	std::cout << "hey!\n";
}

//#include "opcodes.inl"

/*
*  32 bit 8 register
*  pc 32 bit
*  stack pointer ?
*  interrupt reg 1 bit
*  flag register
* 
*  ram[256]
*  
*/

#define scpu_BYTE 8
#define scpu_WORD 16
#define scpu_DWORD 32

#define scpu_LOADi_r1	0x01048000 //r1 = sayi
#define scpu_LOAD_adr	0x01050000 //r1 = ram[adr]
#define scpu_LOAD_ry	0x01398000 //r1 = ram[r2]
#define scpu_LOAD_adrRy 0x013A0000 //r1 = ram[adr + r2]

#define scpu_STR_adr	0x02050000 //ram[adr] = r1
#define scpu_STR_rx		0x02298000 //ram[r1] = r2
#define scpu_STR_adrRx	0x022A0000 //ram[adr + r1] = r2

#define scpu_MOV		0x08280000 //r1 = r2

#define TEST_NUMBER  0xABC
#define TEST_ADDRESS 0xff


std::string readFile(std::string path)
{
	std::fstream file(path);

	if (!file.is_open())
	{
		std::cout << "ERROR:: couldnt open the file\n";
		return "";
	}

	std::stringstream ss;

	ss << file.rdbuf();

	return ss.str();
}

class AsmFileReader
{

public:
	AsmFileReader(std::string& source);
	std::vector<uint32_t> read();

private:
	void nextChar();
	std::string getNextLine();
	void skipNewline();

	std::string& m_source;
	size_t m_position;
	char m_currentChar;

};

void AsmFileReader::nextChar()
{
	m_position++;
	if (m_position > m_source.length())
	{
		m_currentChar = EOF;
	}
	else
	{
		m_currentChar = m_source[m_position];
	}

}

std::string AsmFileReader::getNextLine()
{
	std::string str;
	while (m_currentChar != ' ' && m_currentChar != '\n' && m_currentChar != EOF)
	{
		if (std::isxdigit(m_currentChar))
		{
			str += m_currentChar;
		}
		nextChar();
	}	

	nextChar();

	return str;
}

void AsmFileReader::skipNewline()
{
	while (m_currentChar == '\n')
	{
		nextChar();
	}
}

std::vector<uint32_t> AsmFileReader::read()
{
	//ignore first number
	nextChar();
	nextChar();
	nextChar();


	std::vector<uint32_t> output;
	output.resize(asmc_RAM_SIZE);
	
	while (m_currentChar != EOF)
	{
	
		skipNewline();

		//get index
		std::string str = getNextLine();

		if (m_currentChar == EOF)
		{
			break;
		}

		uint32_t index = rdx::hexToDec(str);

		//get hex value of the command
		str = getNextLine();

		output[index] = rdx::hexToDec(str);
	}

	//TEST
	for (int i = 0; i < 20; i++)
	{
		std::cout <<std::hex << output[i] << "\n";
	}
	
	std::cout << output[0xfe] << "\n";
	std::cout << output[0xff] << "\n";
	//TEST

	return output;
}


AsmFileReader::AsmFileReader(std::string& source)
	:m_source(source)
{
	m_currentChar = 0;
	m_position = -1;
}


int main()
{
	std::string source = readFile("D:\\ProgrammingProjects\\Logisim\\32bitCPU_t3\\Assembler\\out.txt");

	AsmFileReader fr(source);

	std::vector<uint32_t> ram = fr.read();

	Cpu cpu(ram);

	cpu.run();


	//Cpu cpu;

	//cpu.m_registerFile[2] = TEST_ADDRESS;

	//cpu.ram[0] = scpu_LOAD_ry;
	////cpu.ram[1] = TEST_ADDRESS;
	//cpu.ram[TEST_ADDRESS] = TEST_NUMBER;

	//cpu.run();

	//std::cout << std::hex << "r1[" << cpu.m_registerFile[1] << "]\n";

	//using FuncPtr = void (*)();
	//
	//FuncPtr funcPtrList[5];

}

//HATALAR

//define parantez onceligini unuttun 
//asmc_SHIFT_RightRegister((m_currentOpcode & asmc_MASK_RxRy))
