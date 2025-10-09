#pragma once
/*
1	1	LOAD
2	10	STR
3	11	CALL
4	100	RET
5	101	PUSH
6	110	POP
7	111
8	1000	MOV
9	1001	MOV(PULL)
A	1010	MOV(PFR)
B	1011	MOV(CLF)
C	1100	MOV(CCF)
D	1101
E	1110
F	1111
10	10000	ADD
11	10001	SUB
12	10010	MUL
13	10011	DIV

14	10100	AND
15	10101	OR
16	10110	XOR
17	10111	NOT
18	11000	CMP

19	11001	SHL
1A	11010	SHR

1B	11011	JMP
1C	11100	JAZ
1D	11101	JLZ
1E	11110	JGZ
1F	11111	JSC
20	100000	JUC
21	100001	JCT
22	100010	JCF

*/
#include <iostream>
#include <cstdint>
#include <vector>
#include <array>
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
	
	uint32_t m_programCounter;
	uint8_t m_stackPointer;
	uint8_t m_interrupt;
	uint8_t m_flagRegister;

	uint32_t m_accReg;

	bool f_running;
	
	
	std::unordered_map<uint32_t, std::vector<Command>> m_opcodeList;

	//TODO optimize RAM
	//std::unordered_map<int, std::array<uint32_t, 1204>> m_ramDir;


	void getNextInstruction();

	//----------------------------------------------------/

	///-----------------SECTOR_0-------------------------//

	//----------------------------------------------------/
	void op_LOAD_i();
	void op_LOAD_Adr();
	void op_LOAD_AdrRy();
	void op_LOAD_Adr_p_ry();

	//STR @adr, rx
	void op_STR_Adr();
	//STR @rx, ry
	void op_STR_rx();	
	//STR @adr + rx, ry
	void op_STRadrRx();

	void op_MOV();
	void op_MOV_flagStack();
	void op_MOV_stackFlag();
	void op_MOV_flagNumber();

	//----------------------------------------------------//

	///-----------------SECTOR_1-------------------------//

	//----------------------------------------------------/



	void op_ADD_rxry();
	void op_ADD_rxi();
	void op_ADD_Adr();	
	void op_ADD_AdrRy();
	void op_ADD_Adr_p_ry();

	void op_SUB_rxry();
	void op_SUB_rxi();
	void op_SUB_Adr();
	void op_SUB_AdrRy();
	void op_SUB_Adr_p_ry();

	void op_MULT_rxry();
	void op_MULT_rxi();
	void op_MULT_Adr();	
	void op_MULT_AdrRy();
	void op_MULT_Adr_p_ry();

	void op_DIV_rxry();
	void op_DIV_rxri();
	void op_DIV_Adr();	
	void op_DIV_AdrRy();
	void op_DIV_Adr_p_ry();

	void op_SHL();
	void op_SHR();
	void op_AND();
	void op_OR();
	void op_NOT();
	void op_XOR();

	void op_CMP_rx();
	void op_CMP_AdrRy();
	void op_CMP_Adr();
	

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
	void op_PUSH_AdrRx();
	void op_PUSH_Adr();
	void op_PUSH_Adr_p_rx();
	void op_POP();
		
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