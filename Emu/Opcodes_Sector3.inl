//JUMP opcodes

void Cpu::op_JMP()
{
	m_programCounter++;
	m_programCounter = m_ram[m_programCounter];
}
//acc == 0
void Cpu::op_JAZ()
{
	
	if (m_accReg == 0)
	{
		m_programCounter++;
		m_programCounter = m_ram[m_programCounter];
	}
	else
	{
		m_programCounter+= 2;
	}
}
//acc < 0
void Cpu::op_JLZ()
{
	
	if (m_accReg < 0)
	{
		m_programCounter++;
		m_programCounter = m_ram[m_programCounter];
	}
	else
	{
		m_programCounter += 2;
	}
}
//acc > 0
void Cpu::op_JGZ()
{	
	if (m_accReg > 0)
	{
		m_programCounter++;
		m_programCounter = m_ram[m_programCounter];
	}
	else
	{
		m_programCounter += 2;
	}
}

//TODO J_x jump command
void Cpu::op_JSC(){}
void Cpu::op_JUC(){}
void Cpu::op_JCT(){}
void Cpu::op_JMT(){}