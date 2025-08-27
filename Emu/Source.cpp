#include <iostream>
#include <cstdint>


//sfml3
class FramSfml
{

};

//sdl3
class FrameSDL
{

};

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


#define scpu_RAM_SIZE 1024


class Cpu
{
public:
	Cpu();
	~Cpu();

	void run();

	int* ram;
private:
	uint32_t m_currentOpcode;
	uint32_t m_pc;
	uint32_t m_registerFile[8] = { 0 };
	uint8_t m_stackPointer;
	uint8_t m_interrupt;
	uint8_t m_flagRegister;
	
	void op_LOADi();
	
};

Cpu::Cpu()
{
	m_currentOpcode = 0;
	m_pc = 0;	
	m_stackPointer = 0;
	m_interrupt = 0;
	m_flagRegister = 0;

	ram = new int[scpu_RAM_SIZE];
}

Cpu::~Cpu()
{
	delete[] ram;
}

void Cpu::run()
{
	m_currentOpcode = ram[0];
	op_LOADi();
}

void Cpu::op_LOADi()
{
	uint32_t mask = 0x001c0000;
	uint32_t registerAdr = m_currentOpcode & mask;

	registerAdr = registerAdr >> 18;

	m_pc++;
	m_registerFile[registerAdr] = ram[m_pc];

	std::cout << "r1["<< m_registerFile[1] << "]\n";
}



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

#define scpu_LOADi_r1 0x01048000


int main()
{
	Cpu cpu;

	cpu.ram[0] = scpu_LOADi_r1;
	cpu.ram[1] = 25567;

	cpu.run();

	



	void (*ptr)();

	ptr = &print;

	ptr();	

	//------------------------------//
	int* iptr;

	int a = 5;

	iptr = &a;

	//------------------------------//
	void (*funcList[4])();

	funcList[0] = &print;
	funcList[1] = &print;

	funcList[0]();
	funcList[1]();

	
	using FuncPtr = void (*)();
	
	FuncPtr funcPtrList[5];


}