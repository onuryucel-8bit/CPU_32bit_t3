#pragma once

//Dynamic Logisim ROM/RAM Printer

#include <iostream>

#include "utils/Radix.h"

#define COUT_DEBUG

//template<class DataBitWidth>
class DyLogisimPrinter
{
public:
	//DyLogisimPrinter(size_t bitLength);

	//fills empty space with 0
	//input => 0x4 32bit => 0x00000004
	//input => 0x4 16bit => 0x0004
	//input => 0x4 8bit  => 0x04
	void printROM(size_t bitLength, std::vector<int> data);
	
private:
	
	//checks whether a number can be represented with n bits
	//ex: 256 with 8 bits returns false
	bool inRange(int data, size_t bitLength);

	//size_t m_bitLength;
	//DataBitWidth m_dataWidth;//????
};

void DyLogisimPrinter::printROM(size_t bitLength, std::vector<int> data)
{
	std::cout << "Running dyLogisim Printer()...\n";
	std::vector<std::string> outputArr;

	int zeroHexAmount = bitLength / 4;
	if (bitLength % 4 != 0)
	{
		zeroHexAmount++;
	}

	for (size_t i = 0; i < data.size(); i++)
	{
		if (!inRange(data[i],bitLength))
		{
			std::cout << std::dec << "ERROR::DyLogisimPrinter::printROM():: value["<< data[i] <<"]dec is out of range for["<< bitLength <<"]bit\n";
			return;
		}

		outputArr.push_back(rdx::decToHex(data[i]));
		
		while (outputArr[i].size() < zeroHexAmount)
		{
			outputArr[i] = "0" + outputArr[i];
		}

		//std::cout << std::hex << "RESULT::" << outputArr[i] << "\n";
	}
	//file write
	//----------------------------------------//
	std::ofstream file("ROM_t0.txt");
	
	if (!file.is_open())
	{
		return;
	}

	file << "v3.0 hex words plain\n";
	for (size_t i = 0; i < outputArr.size(); i++)
	{
		file << outputArr[i] << " ";

		if ((i + 1) % 16 == 0)
		{
			file << "\n";
		}
	}
	
	file.close();
	//----------------------------------------//

}

bool DyLogisimPrinter::inRange(int data, size_t bitLength)
{
	int maxValue = static_cast<int>(std::pow(2, bitLength)) - 1;

	return data <= maxValue;
}


//using logisim8 = DyLogisimPrinter<uint8_t>;
//using logisim16 = DyLogisimPrinter<uint16_t>;
//using logisim32 = DyLogisimPrinter<uint32_t>;
