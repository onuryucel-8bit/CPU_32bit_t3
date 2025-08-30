#include "Cpu.h"

//LOAD rx,sayi
void Cpu::op_LOADi()
{
	uint32_t regBits = getRegisterBits();

	m_pc++;
	m_registerFile[regBits] = ram[m_pc];

	
}

//LOAD rx,@adr
void Cpu::op_LOADadr()
{
	uint32_t regBits = getRegisterBits();

	m_pc++;

	uint32_t ramAdr = ram[m_pc];

	m_registerFile[regBits] = ram[ramAdr];

	std::cout << std::hex << "r1[" << m_registerFile[1] << "]\n";
}

//LOAD rx,@ry
void Cpu::op_LOADry()
{
	uint32_t regBits = getRegisterBits();

	uint32_t regx = regBits >> 21;
	m_pc++;

	regBits = regBits & 0x07;

	uint32_t address = m_registerFile[regBits];

	m_registerFile[regx] = ram[address];
}

//LOAD rx,@adr + ry
void Cpu::op_LOADadrRy()
{
	uint32_t regBits = getRegisterBits();

	uint32_t regx = regBits >> 21;
	m_pc++;

	uint32_t address = ram[m_pc] + m_registerFile[regBits];

	m_registerFile[regx] = ram[address];
}

//STR @adr, rx
void Cpu::op_STRadr()
{
	uint32_t regBits = getRegisterBits();

	m_pc++;

	uint32_t address = ram[m_pc];

	ram[address] = m_registerFile[regBits];
}

//STR @rx, ry
void Cpu::op_STRrx()
{
	uint32_t regBits = getRegisterBits();

	uint8_t regy = regBits & 0x7;

	uint8_t regx = regBits >> 3;
	//ram[rx] = m_registerFile[ry]
	
	ram[regx] = m_registerFile[regy];
}

//STR @adr + rx, ry
void Cpu::op_STRadrRx()
{

}

//rx = ry
void Cpu::op_MOV()
{
	uint32_t regBits = getRegisterBits();

	uint8_t regy = regBits & 0x7;

	uint8_t regx = regBits >> 3;

	m_registerFile[regx] = m_registerFile[regy];
}


