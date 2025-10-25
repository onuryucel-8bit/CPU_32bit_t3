#pragma once

#include <vector>
#include <fstream>

#include "Lexer.h"

#include "utils/Radix.h"

namespace asmc
{

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

		asmc::Lexer& m_lexer;

		void calculateControlBits();
		void writeToFile();
	};

}
