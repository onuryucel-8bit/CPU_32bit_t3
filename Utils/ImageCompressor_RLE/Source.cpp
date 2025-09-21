#include <unordered_map>
#include <string>

#include "libLocal/StbImage.h"
#include "utils/Radix.h"

std::unordered_map<int, int> rleTable;

void checkTable(int colorVal)
{
	if (rleTable.contains(colorVal))
	{
		rleTable[colorVal]++;
	}
	else
	{
		rleTable[colorVal] = 1;
	}
}

int main()
{
	StbImage stb;
	//"frame_00015.bmp"
	stb.loadImg("test.bmp", false);
	
	std::string rawData;

	for (size_t i = 0; i < stb.getImageHeight(); i++)
	{
		for (size_t j = 0; j < stb.getImageWidth(); j++)
		{
			Pixel pixel = stb.getPixel(j, i);

			std::cout
				<< std::hex
				<< (int)pixel.r << ","
				<< (int)pixel.g << ","
				<< (int)pixel.b << "| ";

			checkTable(pixel.r);
			checkTable(pixel.g);
			checkTable(pixel.b);

			rawData += std::to_string(pixel.r) + std::to_string(pixel.g) + std::to_string(pixel.b);
		}
		std::cout << "\n";
	}

	std::cout << "PRINT table...\n";

	std::cout << std::dec << rawData.length() << "\n";

	std::string rleData;

	for (const auto& [key, value] : rleTable)
	{
		std::cout <<std::hex << "key" << key << " value" << std::dec << value << "\n";

		rleData += rdx::decToHex(key) + std::to_string(value);
	}

	std::cout << rleData << "\n";

	
}