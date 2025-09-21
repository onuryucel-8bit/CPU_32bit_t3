#include <vector>
#include <string>
#include <fstream>

#include "libLocal/StbImage.h"
#include "utils/Radix.h"

struct rleInfo
{
	size_t counter;
	stb::Pixel color;
};

//#define INPUT_ARG_TYPE

std::vector<stb::Pixel> readImage(stb::StbImage& stb, std::string path)
{
	stb.loadImg(path, true);

	if (stb.getImage() == nullptr)
	{
		return {};
	}

	std::vector<stb::Pixel> imgRawData;

	imgRawData.reserve(stb.getImageWidth() * stb.getImageHeight() * stb.getImageChannels());

	std::cout << "Print PIXEL\n";

	//load image pixels to vector
	for (size_t i = 0; i < stb.getImageHeight(); i++)
	{
		for (size_t j = 0; j < stb.getImageWidth(); j++)
		{
			stb::Pixel pixel = stb.getPixel(j, i);

#ifdef DEBUG_PIXEL
			std::cout << std::hex << "x[" << j << "] i[" << i <<"]\n"
				<< (int)pixel.r << " "
				<< (int)pixel.g << " "
				<< (int)pixel.b << " " << "\n";
#endif // DEBUG_PIXEL

			imgRawData.push_back(pixel);
		}
	}

	return imgRawData;
}

std::vector<rleInfo> compressRLE(std::vector<stb::Pixel> imgRawData)
{
	/*
		INPUT : (r,g,b) (r,g,b) ....
				a,a,a,b,b,b,c,b,a,b....

		OUTPUT: vector = { [3,a] [3,b] [1,c] [1,b] [1,a] [1,b]... }
	
	*/

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

	//std::cout << output << "\n";
	std::cout << "raw data in bytes  [" << imgRawData.size() * 3 << "]\n";
	std::cout << "output in bytes    [" << output.size() * 3 << "]\n";

	return output;
}

void writeRleToFile(std::vector<rleInfo>& data)
{
	std::ofstream file("test.txt");

	if (!file.is_open())
	{
		std::cout << "ERROR::writeRleToFile:: couldnt open the file\n";
		return;
	}

	for (size_t i = 0; i < data.size(); i++)
	{
		file << rdx::decToHex(data[i].counter) << ","
			 << rdx::decToHex(data[i].color.r) << "\n";
	}

	file.close();
}

int main(int argc, char* argv[])
{

#ifdef INPUT_ARG_TYPE
	if (argc < 2)
	{
		std::cout << "Expected file path...\n";
		std::cout << "Usage:: [.exe name] [bmp path]\n";

		return -1;
	}

	stb::StbImage stb;			
	
	std::vector<stb::Pixel> rawdata = readImage(stb, argv[1]);

	if (rawdata.empty())
	{
		return -1;
	}

	std::vector<rleInfo> dataRLE = compressRLE(rawdata);

	writeRleToFile(dataRLE);
	

#else
	stb::StbImage stb;
		
	//std::string path = "frame_00015.bmp";
	std::string path = "test.bmp";
	
	std::vector<stb::Pixel> rawdata = readImage(stb, path);

	if (rawdata.empty())
	{
		return -1;
	}

	std::vector<rleInfo> dataRLE = compressRLE(rawdata);

	writeRleToFile(dataRLE);
	

#endif // INPUT_ARG_TYPE
	
}