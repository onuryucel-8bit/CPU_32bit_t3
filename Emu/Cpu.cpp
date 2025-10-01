#include "Cpu.h"

Cpu::Cpu(std::vector<uint32_t>& ram)
{

	m_ram = ram;

	m_currentOpcode = 0;
	m_programCounter = 0;
	m_stackPointer = 0;
	m_interrupt = 0;
	m_flagRegister = 0;	

	m_accReg = 0;

 

	m_opcodeList[0x01].push_back({asmc_MOD_Number ,&Cpu::op_LOADi });
	m_opcodeList[0x01].push_back({asmc_MOD_Adr ,&Cpu::op_LOADadr });


	m_opcodeList[0x10].push_back({ asmc_MOD_Rx_Ry ,&Cpu::op_ADDrxry });

	m_opcodeList[0x02].push_back({ asmc_MOD_Adr ,&Cpu::op_STRadr });

	
	//....
}

Cpu::~Cpu()
{
	
}

void Cpu::run()
{
	getNextInstruction();
	while (m_currentCommand.opcode != 0)
	{
		std::vector<Command> variantList = m_opcodeList[m_currentCommand.opcode];

		for (const Command& c : variantList)
		{
			if (c.mod == m_currentCommand.mod)
			{
				
				(this->*c.func)();

				break;
			}
		}

		m_programCounter++;
		getNextInstruction();
	}
}

void Cpu::getNextInstruction()
{
	//m_currentOpcode <= ram[ProgramCounter];
	m_currentCommand.opcode = (m_ram[m_programCounter] >> 24);
	m_currentCommand.regA = (m_ram[m_programCounter] & 0x00e0'0000) >> 21;
	m_currentCommand.regB = (m_ram[m_programCounter] & 0x001c'0000) >> 18;
	m_currentCommand.mod = (m_ram[m_programCounter] & 0x0003'8000) >> 15;


}



#include "Opcodes_Sector0.inl"

#include "Opcodes_Sector1.inl"

#include "Opcodes_Sector2.inl"

#include "Opcodes_Sector3.inl"