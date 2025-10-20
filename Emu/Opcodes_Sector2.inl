#include "Cpu.h"
//ALU
//WORKING
//add rx,ry
void Cpu::op_ADD_rxry()	
{
	m_accReg = m_registerFile[m_currentCommand.regA] + m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;

}
//add rx,0x*
void Cpu::op_ADD_rxi()	
{
	m_programCounter++;
	uint32_t number = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] + number;

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//add rx,@ff
void Cpu::op_ADD_Adr()	
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] + m_ram[address];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//add rx,@adr+0xff
void Cpu::op_ADD_AdrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] + m_ram[address + m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_ADD_Adr_p_ry()
{
}

//------------------------------------------------//

//sub rx,ry					
void Cpu::op_SUB_rxry()	
{
	m_accReg = m_registerFile[m_currentCommand.regA] - m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//sub rx,hexnumber
void Cpu::op_SUB_rxi()
{
	m_programCounter++;
	uint32_t number = m_ram[m_programCounter];
	 
	m_accReg = m_registerFile[m_currentCommand.regB] - number;

	m_registerFile[m_currentCommand.regB] = m_accReg;
}
//sub rx,@ff
void Cpu::op_SUB_Adr()	
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] - m_ram[address];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//sub rx,@adr+ry
void Cpu::op_SUB_AdrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] - m_ram[address + m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_SUB_Adr_p_ry()
{

}
		
//------------------------------------------------//

void Cpu::op_MULT_rxry()	
{
	m_accReg = m_registerFile[m_currentCommand.regA] * m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_MULT_rxi()	
{
	m_programCounter++;
	uint32_t number = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] * number;

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_MULT_Adr()	
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] * m_ram[address];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_MULT_AdrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] * m_ram[address + m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_MULT_Adr_p_ry()
{

}
		
//------------------------------------------------//

void Cpu::op_DIV_rxry()	
{
	m_accReg = m_registerFile[m_currentCommand.regA] / m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_DIV_rxri()	
{
	m_programCounter++;
	uint32_t number = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] / number;

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_DIV_Adr()	
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] / m_ram[address];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_DIV_AdrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] / m_ram[address + m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_DIV_Adr_p_ry()
{

}

//------------------------------------------------//


//shl rx,ry
void Cpu::op_SHL()
{
	m_accReg = m_registerFile[m_currentCommand.regA] << m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//shr rx,ry
void Cpu::op_SHR()
{
	m_accReg = m_registerFile[m_currentCommand.regA] >> m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//and rx,ry
void Cpu::op_AND()
{
	m_accReg = m_registerFile[m_currentCommand.regA] & m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_OR() 
{
	m_accReg = m_registerFile[m_currentCommand.regA] | m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_NOT()
{
	m_accReg = ~m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_XOR()
{
	m_accReg = m_registerFile[m_currentCommand.regA] ^ m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

//TODO CMP
//CMP rx, @ry
void Cpu::op_CMP_AdrRy()
{
	if (m_registerFile[m_currentCommand.regA] == m_ram[m_currentCommand.regB])
	{
		m_flagRegister = 0;
	}
	else if (m_registerFile[m_currentCommand.regA] < m_ram[m_currentCommand.regB])
	{
		m_flagRegister = 0;
	}
	else if (m_registerFile[m_currentCommand.regA] > m_ram[m_currentCommand.regB])
	{
		m_flagRegister = 0;
	}
}

//CMP rx, @adr
void Cpu::op_CMP_Adr()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	if (m_registerFile[m_currentCommand.regA] == m_ram[address])
	{
		m_flagRegister = 0;
	}
	else if (m_registerFile[m_currentCommand.regA] < m_ram[address])
	{
		m_flagRegister = 0;
	}
	else if (m_registerFile[m_currentCommand.regA] > m_ram[address])
	{
		m_flagRegister = 0;
	}
}

//CMP rx, ry
void Cpu::op_CMP_rx()
{	

	if (m_registerFile[m_currentCommand.regA] == m_registerFile[m_currentCommand.regB])
	{
		m_flagRegister = 0;
	}
	else if (m_registerFile[m_currentCommand.regA] < m_registerFile[m_currentCommand.regB])
	{
		m_flagRegister = 0;
	}
	else if (m_registerFile[m_currentCommand.regA] > m_registerFile[m_currentCommand.regB])
	{
		m_flagRegister = 0;
	}
}