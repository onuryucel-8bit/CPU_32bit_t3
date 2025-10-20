#include "FileReader.h"


void AsmFileReader::nextChar()
{
	m_position++;
	if (m_position > m_source.length())
	{
		m_currentChar = EOF;
	}
	else
	{
		m_currentChar = m_source[m_position];
	}

}

std::string AsmFileReader::getNextLine()
{
	std::string str;
	while (m_currentChar != ' ' && m_currentChar != '\n' && m_currentChar != EOF)
	{
		if (std::isxdigit(m_currentChar))
		{
			str += m_currentChar;
		}
		nextChar();
	}

	nextChar();

	return str;
}

void AsmFileReader::skipNewline()
{
	while (m_currentChar == '\n')
	{
		nextChar();
	}
}

std::vector<uint32_t> AsmFileReader::read()
{
	//ignore first number
	nextChar();
	nextChar();
	nextChar();


	std::vector<uint32_t> output;
	output.resize(asmc_RAM_SIZE);

	while (m_currentChar != EOF)
	{

		skipNewline();

		//get index
		std::string str = getNextLine();

		if (m_currentChar == EOF)
		{
			break;
		}

		uint32_t index = rdx::hexToDec(str);

		//get hex value of the command
		str = getNextLine();

		output[index] = rdx::hexToDec(str);
	}

	//TEST
	/*for (int i = 0; i < 20; i++)
	{
		std::cout << std::hex << output[i] << "\n";
	}*/

	//std::cout << output[0xfe] << "\n";
	//std::cout << output[0xff] << "\n";
	//TEST

	return output;
}


AsmFileReader::AsmFileReader(std::string& source)
	:m_source(source)
{
	m_currentChar = 0;
	m_position = -1;
}
