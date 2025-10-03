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

 
	//REG - RAM
	
	//LOAD
	m_opcodeList[0x01].push_back({ asmc_MOD_Number ,&Cpu::op_LOADi });
	m_opcodeList[0x01].push_back({ asmc_MOD_Adr ,&Cpu::op_LOADadr });
	m_opcodeList[0x01].push_back({ asmc_MOD_RegAdr ,&Cpu::op_LOADry });
	m_opcodeList[0x01].push_back({ asmc_MOD_Adr_P_Reg ,&Cpu::op_LOADadrRy });
	//STR
	m_opcodeList[0x02].push_back({ asmc_MOD_Adr ,&Cpu::op_STRadr });
	m_opcodeList[0x02].push_back({ asmc_MOD_RegAdr ,&Cpu::op_STRrx });
	m_opcodeList[0x02].push_back({ asmc_MOD_Adr_P_Reg ,&Cpu::op_STRadrRx });
	//MOV
	m_opcodeList[0x08].push_back({ asmc_MOD_Adr_P_Reg ,&Cpu::op_MOV });

	//ALU
		//ADD
	m_opcodeList[0x10].push_back({ asmc_MOD_Rx_Ry ,&Cpu::op_ADDrxry });
	m_opcodeList[0x10].push_back({ asmc_MOD_Number ,&Cpu::op_ADDrxi });
		//SUB
	m_opcodeList[0x11].push_back({ asmc_MOD_Number ,&Cpu::op_SUBrxi });
		//MULT
	m_opcodeList[0x12].push_back({ asmc_MOD_Rx_Ry ,&Cpu::op_MULTrxry});
		//DIV
	m_opcodeList[0x13].push_back({ asmc_MOD_Rx_Ry ,&Cpu::op_DIVrxry });
	
	//STACK
	m_opcodeList[0x03].push_back({ asmc_MOD_Empty ,&Cpu::op_CALL});
	m_opcodeList[0x04].push_back({ asmc_MOD_Empty ,&Cpu::op_RET });

	//JUMP
	m_opcodeList[0x1B].push_back({ asmc_MOD_Empty ,&Cpu::op_JMP });
	m_opcodeList[0x1c].push_back({ asmc_MOD_Empty ,&Cpu::op_JAZ });
	m_opcodeList[0x1d].push_back({ asmc_MOD_Empty ,&Cpu::op_JLZ });
	m_opcodeList[0x1e].push_back({ asmc_MOD_Empty ,&Cpu::op_JGZ });
	m_opcodeList[0x1f].push_back({ asmc_MOD_Empty ,&Cpu::op_JSC });
	m_opcodeList[0x20].push_back({ asmc_MOD_Empty ,&Cpu::op_JUC });
	//TODO check cSet name
	m_opcodeList[0x21].push_back({ asmc_MOD_Empty ,&Cpu::op_JCT });
	//m_opcodeList[0x22].push_back({ asmc_MOD_Empty ,&Cpu::op_JCF });

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

		//check variant list
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