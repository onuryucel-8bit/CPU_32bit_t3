#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>


//TEST
#include <bitset>

#define asmc_RAM_SIZE (uint32_t)0xffff

#define asmc_MOD_Empty 0
#define asmc_MOD_Number 1
//@adr
#define asmc_MOD_Adr 2 //010
//@ry
#define asmc_MOD_RegAdr 3 //011
//@adr + ry
#define asmc_MOD_Adr_P_Reg 4 //100
//rx, ry
#define asmc_MOD_Rx_Ry 5 //101


struct Instruction
{
	uint8_t opcode;
	uint8_t regA;
	uint8_t regB;
	uint8_t mod;
};

struct RegPart
{
	uint32_t rx = 0;
	uint32_t ry = 0;
};

extern struct Command;

class Cpu
{
public:
	Cpu(std::vector<uint32_t>& ram);
	~Cpu();

	void run();

	
	uint32_t m_registerFile[8] = { 0 };

	int m_stack[256];

private:

	std::vector<uint32_t> m_ram;

	Instruction m_currentCommand;

	uint32_t m_currentOpcode;
	uint32_t m_programCounter;
	uint8_t m_stackPointer;
	uint8_t m_interrupt;
	uint8_t m_flagRegister;

	uint32_t m_accReg;


	
	
	std::unordered_map<uint32_t, std::vector<Command>> m_opcodeList;

	void getNextInstruction();

	//----------------------------------------------------/

	///-----------------SECTOR_0-------------------------//

	//----------------------------------------------------/
	void op_LOADi();
	void op_LOADadr();
	void op_LOADry();
	void op_LOADadrRy();

	//STR @adr, rx
	void op_STRadr();
	//STR @rx, ry
	void op_STRrx();	
	//STR @adr + rx, ry
	void op_STRadrRx();

	void op_MOV();
	//----------------------------------------------------//

	///-----------------SECTOR_1-------------------------//

	//----------------------------------------------------/



	void op_ADDrxry();
	void op_ADDrxi();
	void op_ADDrxAdr();	
	void op_ADDrxAdrRy();

	void op_SUBrxry();
	void op_SUBrxi();
	void op_SUBrxAdr();	
	void op_SUBrxAdrRy();

	void op_MULTrxry();
	void op_MULTrxi();
	void op_MULTrxAdr();	
	void op_MULTrxAdrRy();

	void op_DIVrxry();
	void op_DIVrxri();
	void op_DIVrxAdr();	
	void op_DIVrxAdrRy();

	void op_SHL();
	void op_SHR();
	void op_AND();
	void op_OR();
	void op_NOT();
	void op_XOR();

	void op_CMPrx();
	void op_CMPrxAdrRy();
	void op_CMPrxAdr();
	

	//----------------------------------------------------/

	///-----------------SECTOR_2-------------------------//

	//----------------------------------------------------/

	void op_CALL();
	void op_RET();

	/*
	PUSH rx
	PUSH @rx
	PUSH @adr
	PUSH @adr + rx
	*/
	void op_PUSH_rx();
	void op_POP();
	//void op_MOV_flag();
	//void op_MOV_();
	//void op_
		
	//----------------------------------------------------/

	///-----------------SECTOR_3-------------------------//

	//----------------------------------------------------/
	
	void op_JMP();
	void op_JAZ();
	void op_JLZ();
	void op_JGZ();
	void op_JSC();
	void op_JUC();
	void op_JCT();
	void op_JMT(); 

};

using funcPtr = void (Cpu::*)();

struct Command
{
	uint8_t mod;
	funcPtr func;
};