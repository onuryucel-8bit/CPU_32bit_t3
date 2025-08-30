#pragma once

#include <iostream>
#include <cstdint>

#define scpu_RAM_SIZE 1024

//getRegisterBits()
	#define scpu_MASK_Rx 0x001c0000
	#define scpu_MASK_RxRy 0x00fc0000

	#define scpu_SHIFT_RightRegister(bits) bits >> 18
	#define scpu_GET_ModBits(bits) bits & 0x00038000
//END

class Cpu
{
public:
	Cpu();
	~Cpu();

	void run();

	int* ram;
	uint32_t m_registerFile[8] = { 0 };

private:

	uint32_t m_currentOpcode;
	uint32_t m_pc;
	uint8_t m_stackPointer;
	uint8_t m_interrupt;
	uint8_t m_flagRegister;

	using funcPtr = void (Cpu::*)();

	funcPtr m_opcodeList[36];

	uint32_t getRegisterBits();

	///-----------------SECTOR_0-------------------------//
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
	void op_CMP();
	

	//----------------------------------------------------/

	///-----------------SECTOR_2-------------------------//

	//----------------------------------------------------/

	void op_CALL();
	void op_RET();
	void op_PUSH();
	void op_POP();
	//void op_MOV_flag();
	//void op_MOV_();
	//void op_

	
	void op_JMP();
	void op_JAZ();
	void op_JLZ();
	void op_JGZ();
	void op_JSC();
	void op_JUC();
	void op_JCT();
	void op_JMT();
};
