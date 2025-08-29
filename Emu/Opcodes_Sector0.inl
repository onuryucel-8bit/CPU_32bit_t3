#include "Cpu.h"
uint32_t Cpu::getRegisterBits()
{
	uint32_t retVal = 0;
	
	switch ((scpu_GET_ModBits(m_currentOpcode)) >> 15)
	{
	
	case 0:
		std::cout << "TEST\n";
		break;

	//sayi
	case 1: [[fallthrough]];
	//@adr	
	case 2:
		retVal = scpu_SHIFT_RightRegister(m_currentOpcode & scpu_MASK_Rx);
		break;					
	//@ry
	case 3: [[fallthrough]];
	//@adr + ry
	case 4:
		retVal = scpu_SHIFT_RightRegister(m_currentOpcode & scpu_MASK_RxRy);
		break;			
	}

	return retVal;
}
//LOAD rx,sayi
void Cpu::op_LOADi()
{
	uint32_t regBits = getRegisterBits();

	m_pc++;
	m_registerFile[regBits] = ram[m_pc];

	std::cout << "r1[" << m_registerFile[1] << "]\n";
}

//LOAD rx,@adr
void Cpu::op_LOADadr()
{
	uint32_t regBits = getRegisterBits();

	m_pc++;

	uint32_t ramAdr = ram[m_pc];

	m_registerFile[regBits] = ram[ramAdr];
}

//LOAD rx,@ry
void Cpu::op_LOADry()
{
	uint32_t regBits = getRegisterBits();

	uint32_t regx = regBits >> 21;
	m_pc++;

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

