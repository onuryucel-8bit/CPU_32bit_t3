#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

#include "../Cpu.h" //define asmc_ram_max

class AsmFileReader
{

public:
	AsmFileReader(std::string& source);
	std::vector<uint32_t> read();

private:
	void nextChar();
	std::string getNextLine();
	void skipNewline();

	std::string& m_source;
	size_t m_position;
	char m_currentChar;

};
