#include "Cpu.h"
//ALU
//WORKING
//add rx,ry
void Cpu::op_ADDrxry()	
{
	m_accReg = m_registerFile[m_currentCommand.regA] + m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;

}
//add rx,0x*
void Cpu::op_ADDrxi()	
{
	m_programCounter++;
	uint32_t number = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] + number;

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//add rx,@ff
void Cpu::op_ADDrxAdr()	
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] + m_ram[address];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//add rx,@adr+0xff
void Cpu::op_ADDrxAdrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] + m_ram[address + m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

//------------------------------------------------//

//sub rx,ry					
void Cpu::op_SUBrxry()	
{
	m_accReg = m_registerFile[m_currentCommand.regA] - m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//sub rx,0xff
void Cpu::op_SUBrxi()
{
	m_programCounter++;
	uint32_t number = m_ram[m_programCounter];
	 
	m_accReg = m_registerFile[m_currentCommand.regA] + number;

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//sub rx,@ff
void Cpu::op_SUBrxAdr()	
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] - m_ram[address];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
//sub rx,@adr+ry
void Cpu::op_SUBrxAdrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] - m_ram[address + m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
		
//------------------------------------------------//

void Cpu::op_MULTrxry()	
{
	m_accReg = m_registerFile[m_currentCommand.regA] * m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_MULTrxi()	
{
	m_programCounter++;
	uint32_t number = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] * number;

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_MULTrxAdr()	
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] * m_ram[address];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_MULTrxAdrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] * m_ram[address + m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}
		
//------------------------------------------------//

void Cpu::op_DIVrxry()	
{
	m_accReg = m_registerFile[m_currentCommand.regA] / m_registerFile[m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_DIVrxri()	
{
	m_programCounter++;
	uint32_t number = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] / number;

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_DIVrxAdr()	
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] / m_ram[address];

	m_registerFile[m_currentCommand.regA] = m_accReg;
}

void Cpu::op_DIVrxAdrRy()
{
	m_programCounter++;
	uint32_t address = m_ram[m_programCounter];

	m_accReg = m_registerFile[m_currentCommand.regA] / m_ram[address + m_currentCommand.regB];

	m_registerFile[m_currentCommand.regA] = m_accReg;
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
void Cpu::op_CMPrxAdrRy()
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
void Cpu::op_CMPrxAdr()
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
void Cpu::op_CMPrx()
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