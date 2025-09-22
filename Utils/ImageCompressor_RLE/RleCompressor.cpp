#include "RleCompressor.h"

RleCompressor::RleCompressor()
{
}

RleCompressor::~RleCompressor()
{
}

std::vector<rleInfo> RleCompressor::compressBMP(std::vector<stb::Pixel> imgRawData)
{
	stb::Pixel lastColor = imgRawData[0];
	size_t counter = 1;

	std::vector<rleInfo> output;

	//RLE compression
	for (size_t i = 1; i < imgRawData.size(); i++)
	{
		if (lastColor == imgRawData[i])
		{
			counter++;
		}
		else
		{
			output.push_back({ counter, lastColor });
			lastColor = imgRawData[i];
			counter = 1;
		}
	}

	//insert last color
	//its leaving loop before inserting 
	output.push_back({ counter, lastColor });



	std::cout << "-------------------\n";
	std::cout << "raw data in kb  [" << (float)(imgRawData.size() * 3) / 1204 << "]\n";
	std::cout << "raw data in mb  [" << (float)(imgRawData.size() * 3) / (1204 * 1204) << "]\n";
	std::cout << "-------------------\n";
	std::cout << "raw data in bytes  [" << imgRawData.size() * 3 << "]\n";
	std::cout << "output in bytes    [" << output.size() * 3 << "]\n";
	std::cout << "-------------------\n";
	std::cout << "output in kb    [" << (float)(output.size() * 3) / 1204<< "]\n";

	return output;
}

std::vector<stb::Pixel> RleCompressor::deCompressBMP(std::vector<rleInfo> compressedData)
{
	std::vector<stb::Pixel> pixelData;

	int testInt = 0;

	for (size_t i = 0; i < compressedData.size(); i++)
	{
		for (size_t j = 0; j < compressedData[i].counter; j++)
		{
			pixelData.push_back(compressedData[i].color);
		}

		testInt += compressedData[i].counter;
	}

	return pixelData;
}
