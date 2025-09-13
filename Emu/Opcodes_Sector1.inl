//STACK
void Cpu::op_CALL(){}
void Cpu::op_RET()
{}

void Cpu::op_PUSH_rx()
{
	uint32_t regBits = getRegisterBits();

	m_stack[m_stackPointer] = m_registerFile[regBits];

	m_stackPointer++;
}

void Cpu::op_POP(){}
