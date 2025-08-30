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




#include "Cpu.h"


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


int main()
{
	Cpu cpu;

	cpu.m_registerFile[2] = TEST_ADDRESS;

	cpu.ram[0] = scpu_LOAD_ry;
	//cpu.ram[1] = TEST_ADDRESS;
	cpu.ram[TEST_ADDRESS] = TEST_NUMBER;

	cpu.run();

	std::cout << std::hex << "r1[" << cpu.m_registerFile[1] << "]\n";

	//using FuncPtr = void (*)();
	//
	//FuncPtr funcPtrList[5];

}

//HATALAR

//define parantez onceligini unuttun 
//scpu_SHIFT_RightRegister((m_currentOpcode & scpu_MASK_RxRy))