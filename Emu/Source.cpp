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

#define scpu_LOADi_r1 0x01048000



int main()
{
	Cpu cpu;

	cpu.ram[0] = scpu_LOADi_r1;
	cpu.ram[1] = 25567;

	cpu.run();


	//using FuncPtr = void (*)();
	//
	//FuncPtr funcPtrList[5];


}