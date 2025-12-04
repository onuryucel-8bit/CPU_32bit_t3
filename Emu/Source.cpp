/*
 
 
	*----------------0x0
	| 
	|  STACK
	| 
	*----------------0x0000_00ff 256
	| 
	|  RAM
	|
	*----------------
	| BUZZER
	*----------------
	| KEYBOARD
	*----------------128x128 + 3 | 256x256 + 3
	| 
	| gReg_command
	| gReg_color
	| gReg_data
	*-----------------
	
	
	
*/

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
#include "FileIO/FileReader.h"

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

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "SDL_Window.h"


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

int main()
{
	/*std::string source = readFile("D:\\ProgrammingProjects\\Logisim\\32bitCPU_t3\\Assembler\\out.txt");

	AsmFileReader fr(source);

	std::vector<uint32_t> ram = fr.read();

	Cpu cpu(ram);

	cpu.run();*/

	Window w;
	w.run(); 
	


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
