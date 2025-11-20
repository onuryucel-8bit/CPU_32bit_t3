#include "LogisimPrinter.h"

LogisimPrinter::LogisimPrinter()
{
}

LogisimPrinter::~LogisimPrinter()
{
}

void LogisimPrinter::print(std::vector<asmc::MemoryLayout>& array)
{
	std::ofstream file("logisimRAM_output.txt");

	file << "v3.0 hex words plain\n";


	int lastPosition = -1;
	for (size_t i = 0; i < array.size(); i++)
	{	
		//check address jumps
		if (lastPosition + 1 != array[i].m_ramIndex)
		{
			//fill [lastPosition : array[i].m_ramIndex] = 0
			for (size_t j = 0; j < array[i].m_ramIndex - lastPosition; j++)
			{
				file << "0 ";

				//TODO +1 ???
				if ((j + i + 1) % 16 == 0)
				{
					file << "\n";
				}
			}			
		}

		//write hex values to the file
		//----------------------------------------------//
		file << rdx::decToHex(array[i].m_opcode) << " ";

		lastPosition++;

		if (array[i].m_packetSize == 2)
		{
			file << rdx::decToHex(array[i].m_secondPart) << " ";
			lastPosition++;
		}

		if ((i + 1) % 16 == 0)
		{
			file << "\n";
		}		
		//----------------------------------------------//
		
	}

	file.close();
}
