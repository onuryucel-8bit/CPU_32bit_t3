#include "Cpu.h"

//LOAD rx,sayi
void Cpu::op_LOADi()
{
	m_programCounter++;

	m_registerFile[m_currentCommand.regB] = m_ram[m_programCounter];
}
//WORKING
//LOAD rx,@adr
void Cpu::op_LOADadr()
{	
	uint8_t regx = m_currentCommand.regB;

	m_programCounter++;

	uint32_t address = m_ram[m_programCounter];

	m_registerFile[regx] = m_ram[address];
}

//LOAD rx,@ry
void Cpu::op_LOADry()
{
	m_registerFile[m_currentCommand.regA] = m_ram[m_currentCommand.regB];
}

//LOAD rx,@adr + ry
void Cpu::op_LOADadrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_registerFile[m_currentCommand.regA] = m_ram[address + m_currentCommand.regB];
}

//WORKING
//STR @adr, rx
void Cpu::op_STRadr()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_ram[address] = m_registerFile[m_currentCommand.regB];
}

//STR @rx, ry
void Cpu::op_STRrx()
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


