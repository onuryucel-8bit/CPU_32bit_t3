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

void Cpu::op_PUSH_rx()
{
	//uint32_t regBits = getRegisterBits();

	//m_stack[m_stackPointer] = m_registerFile[regBits];

	//m_stackPointer++;
}

void Cpu::op_PUSH_AdrRx()
{
}

void Cpu::op_PUSH_Adr()
{
}

void Cpu::op_PUSH_Adr_p_rx()
{
}

void Cpu::op_POP(){}
