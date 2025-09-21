#include <vector>
#include <string>

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
	stb.loadImg(path, false);

	if (stb.getImage() == nullptr)
	{
		return {};
	}

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

std::vector<rleInfo> compressRLE(std::vector<stb::Pixel> imgRawData)
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

void writeRleToFile(std::vector<rleInfo>& data, size_t fileIndex)
{
	
}

int main(int argc, char* argv[])
{

#ifdef INPUT_ARG_TYPE
	if (argc < 2)
	{
		std::cout << "Expected file path...\n";
		std::cout << "Usage:: [.exe name] [bmp path]\n";

		return 1;
	}

	stb::StbImage stb;		

	std::cout << "argv " <<argv[1] << "\n";
	std::cout << "argc " <<argc << "\n";

	for (size_t i = 1; i < argc; i++)
	{
		std::vector<stb::Pixel> rawdata = readImage(stb, argv[i]);

		if (rawdata.empty())
		{
			return -1;
		}

		std::vector<rleInfo> dataRLE = compressRLE(rawdata);

		writeRleToFile(dataRLE, i);
	}

#else
	stb::StbImage stb;
	std::string path = "test";

	for (size_t i = 1; i < 2; i++)
	{
		std::vector<stb::Pixel> rawdata = readImage(stb, path);

		if (rawdata.empty())
		{
			return -1;
		}

		std::vector<rleInfo> dataRLE = compressRLE(rawdata);

		writeRleToFile(dataRLE, i);
	}

#endif // INPUT_ARG_TYPE
	
}