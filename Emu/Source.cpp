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

class Cpu
{
public:
	Cpu();
	~Cpu();

private:
	uint32_t pc;
	uint32_t regs;
	

};

Cpu::Cpu()
{
}

Cpu::~Cpu()
{
}

//#include "opcodes.inl"

int main()
{
	
}