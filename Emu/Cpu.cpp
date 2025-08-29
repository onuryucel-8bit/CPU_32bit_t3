#include "Cpu.h"

Cpu::Cpu()
{
	m_currentOpcode = 0;
	m_pc = 0;
	m_stackPointer = 0;
	m_interrupt = 0;
	m_flagRegister = 0;

	ram = new int[scpu_RAM_SIZE];

	m_opcodeList[0] = &Cpu::op_LOADi;
	m_opcodeList[1] = &Cpu::op_LOADadr;
	m_opcodeList[2] = &Cpu::op_LOADry;
	m_opcodeList[3] = &Cpu::op_LOADadrRy;
	//....
}

Cpu::~Cpu()
{
	delete[] ram;
}

void Cpu::run()
{
	m_currentOpcode = ram[0];

	uint32_t modBits = scpu_GET_ModBits(m_currentOpcode);
	uint32_t opcodeBits = m_currentOpcode >> 24;

	std::cout << std::hex << "opcodeBits[" << opcodeBits << "]\n";
	std::cout << std::hex << "modBits[" << opcodeBits << "]\n";
	std::cout << std::dec;

	
	op_LOADi();
}

#include "Opcodes_Sector0.inl"

#include "Opcodes_Sector1.inl"

#include "Opcodes_Sector2.inl"

#include "Opcodes_Sector3.inl"