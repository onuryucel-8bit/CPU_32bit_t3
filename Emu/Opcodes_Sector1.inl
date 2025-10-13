#include "Cpu.h"
//STACK
void Cpu::op_CALL()
{
	//pc points func address
	m_programCounter++;

	//put pc inside stack
	m_stack[m_stackPointer] = m_programCounter + 1;
	m_stackPointer++;

	//call func
	m_programCounter = m_ram[m_programCounter];
	
}

void Cpu::op_RET()
{
	m_stackPointer--;
	m_programCounter = m_stack[m_stackPointer];	
}

//push rx
void Cpu::op_PUSH_rx()
{
	m_stack[m_stackPointer] = m_currentCommand.regB;
	m_stackPointer++;
}

//push @rx / stack = ram[rx]
void Cpu::op_PUSH_AdrRx()
{
	m_stack[m_stackPointer] = m_ram[m_currentCommand.regB];
	m_stackPointer++;
}

//push @adr / stack = ram[adr]
void Cpu::op_PUSH_Adr()
{
	m_programCounter++;

	m_stack[m_stackPointer] = m_ram[m_programCounter];
	m_stackPointer++;
}

//push @adr + rx / stack = ram[adr + rx]
void Cpu::op_PUSH_Adr_p_rx()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_stack[m_stackPointer] = m_ram[address + m_currentCommand.regB];
	m_stackPointer++;
}

void Cpu::op_POP()
{
	m_stackPointer--;
}
