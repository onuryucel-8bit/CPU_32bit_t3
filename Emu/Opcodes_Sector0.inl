#include "Cpu.h"

//LOAD rx,sayi
void Cpu::op_LOAD_i()
{
	m_programCounter++;

	m_registerFile[m_currentCommand.regB] = m_ram[m_programCounter];
}
//WORKING
//LOAD rx,@adr
void Cpu::op_LOAD_Adr()
{	
	uint8_t regx = m_currentCommand.regB;

	m_programCounter++;

	uint32_t address = m_ram[m_programCounter];

	m_registerFile[regx] = m_ram[address];
}

//LOAD rx,@ry
void Cpu::op_LOAD_AdrRy()
{
	m_registerFile[m_currentCommand.regA] = m_ram[m_currentCommand.regB];
}

//LOAD rx,@adr + ry
void Cpu::op_LOAD_Adr_p_ry()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_registerFile[m_currentCommand.regA] = m_ram[address + m_currentCommand.regB];
}

//WORKING
//STR @adr, rx
void Cpu::op_STR_Adr()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_ram[address] = m_registerFile[m_currentCommand.regB];
}

//STR @rx, ry
void Cpu::op_STR_rx()
{
	m_ram[m_currentCommand.regA] = m_registerFile[m_currentCommand.regB];
}

//STR @adr + rx, ry
void Cpu::op_STRadrRx()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_ram[address + m_currentCommand.regA] = m_ram[m_currentCommand.regB];
}

//rx = ry
void Cpu::op_MOV()
{
	m_registerFile[m_currentCommand.regA] = m_registerFile[m_currentCommand.regB];
}

//mov flag = stack[sp]
void Cpu::op_MOV_flagStack()
{
	m_flagRegister = m_stack[m_stackPointer];
}

//stack[sp] = flag
//sp++
void Cpu::op_MOV_stackFlag()
{
	m_stack[m_stackPointer] = m_flagRegister;
	m_stackPointer++;
}

//flag = 0xff
void Cpu::op_MOV_flagNumber()
{
	m_programCounter++;
	m_flagRegister = m_ram[m_programCounter];
}


