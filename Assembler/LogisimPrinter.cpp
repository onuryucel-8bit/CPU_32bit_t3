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


	int romIndex = 0;
	for (size_t i = 0; i < array.size(); i++)
	{	

		size_t target = array[i].m_ramIndex;

		//Fill gap with zeros
		while (romIndex < target)
		{
			file << "0 ";

			romIndex++;
			if (romIndex % 16 == 0)
			{
				file << "\n";
			}
		}
		
		//write hex values to the file
		//----------------------------------------------//
		file << rdx::decToHex(array[i].m_opcode) << " ";

		romIndex++;

		if (array[i].m_packetSize == 2)
		{
			file << rdx::decToHex(array[i].m_secondPart) << " ";
			romIndex++;
		}

		if ((i + 1) % 16 == 0)
		{
			file << "\n";
		}		
		//----------------------------------------------//
		
	}

	file.close();
}
