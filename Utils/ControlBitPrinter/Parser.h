#pragma once

#include <vector>
#include <fstream>

#include "Lexer.h"

#include "utils/Radix.h"

namespace asmc
{
	//its like a pointer to pointer ptr**
	struct adrVec
	{
		size_t m_selfAdr;
		//points to the starting address of the control bit ROM
		size_t m_controlBitStartPos;
	};

	class Parser
	{
	public:
		Parser(asmc::Lexer& lexer);
		~Parser();

		void run();

	private:				
		//control bits are defined between curly brackets { } 
		//if parser sees # end of control bits it names it sector
		/*
			{
				Read
				Write
				#
				PC_enb
				#
			}
			{
				...
				#
			}
			
			m_sector[0] = hex(read,write)
			m_sector[1] = hex(PC_enb)
			m_sector[2] = ...
			
			at each # symbol the parser calculates the control bit line

			at each } closing bracket parser stores pushes sectors data to output array
		*/
		std::vector<int> m_sector;

		std::vector<int> m_output;

		std::vector<asmc::adrVec> m_addressROM;
		asmc::adrVec m_addressVec;		

		std::unordered_map<std::string, size_t> m_locationTable;

		asmc::Lexer& m_lexer;

		void calculateControlBits();
		void writeToFile();

		//points to starting address of control bits
		size_t m_controlRomIndex;
	};

}
