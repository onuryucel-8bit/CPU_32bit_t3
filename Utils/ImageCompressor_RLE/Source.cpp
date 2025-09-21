#include <vector>
#include <string>

#include "libLocal/StbImage.h"
#include "utils/Radix.h"

struct rleInfo
{
	size_t counter;
	stb::Pixel color;
};


std::vector<stb::Pixel> readImage(stb::StbImage& stb, std::string path)
{
	stb.loadImg(path, false);

	std::vector<stb::Pixel> imgRawData;

	imgRawData.reserve(stb.getImageWidth() * stb.getImageHeight() * stb.getImageChannels());

	for (size_t i = 0; i < stb.getImageHeight(); i++)
	{
		for (size_t j = 0; j < stb.getImageWidth(); j++)
		{
			stb::Pixel pixel = stb.getPixel(j, i);

			imgRawData.push_back(pixel);
		}
	}

	return imgRawData;
}

std::vector<uint8_t> compressRLE(std::vector<stb::Pixel> imgRawData)
{
	stb::Pixel lastColor = imgRawData[0];
	size_t counter = 1;
	
	std::vector<rleInfo> output;

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

	//std::cout << output << "\n";
	std::cout << "raw data length in bytes[" << imgRawData.size() * 3 << "]\n";
	std::cout << "output length[" << output.size() << "]\n";
	std::cout << "output length in bytes[" << output.size() * 3 << "]\n";

	return output;
}

int main()
{
	stb::StbImage stb;
	//"frame_00015.bmp"
	std::vector<stb::Pixel> rawdata = readImage(stb, "frame_00015.bmp");
			
	std::vector<uint8_t> dataRLE = compressRLE(rawdata);


	
	
	
}