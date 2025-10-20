#include <vector>
#include <string>
#include <fstream>

#include "libLocal/StbImage.h"
#include "utils/Radix.h"
#include "RleCompressor.h"

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

#ifdef DEBUG_PIXEL
	std::cout << "Print PIXEL\n";
#endif // DEBUG_PIXEL

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

			uint8_t averageColor = (pixel.r + pixel.g + pixel.b) / 3;

			if (averageColor < 120)
			{
				pixel.r = 0;
				pixel.g = 0;
				pixel.b = 0;			
			}
			else
			{
				pixel.r = 0xff;
				pixel.g = 0xff;
				pixel.b = 0xff;
			}

			imgRawData.push_back(pixel);
		}
	}

	return imgRawData;
}


void writeRleToFile(std::vector<rleInfo>& data)
{
	std::ofstream file("test.txt");

	if (!file.is_open())
	{
		std::cout << "ERROR::writeRleToFile:: couldnt open the file\n";
		return;
	}

	file << ".db ";

	for (size_t i = 0; i < data.size(); i++)
	{
		file << "0x"<< rdx::decToHex(data[i].counter) << ",0x"
			 << rdx::decToHex(data[i].color.r);

		if (i != data.size() - 1)
		{
			file << ",";
		}

		if ((i + 1) % 8 == 0)
		{
			file << "\n";
		}
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

	RleCompressor rle;
			
	std::string path = "C:\\Users\\local1\\Videos\\frames_1fps10MB\\frame_00015.bmp";

	size_t frameNoStartPos = path.find("00014");

	//std::string path = "test.bmp";

	std::string arg3 = "221";

	int limit = std::stoi(arg3);

	//std::cout << "substr test " << path.substr(0, 6) << "\n";
	//std::cout << "number substr test " << std::stoi(path.substr(6, path.length() - 10)) << "\n";

	//std::cout << std::stoi(arg3) << "\n";
	
	for (size_t i = 0; i < limit; i++)
	{	
		std::cout << "Compressing[" << path << "]\n";

		std::vector<stb::Pixel> rawdata = readImage(stb, path);

		if (rawdata.empty())
		{
			return -1;
		}

		std::vector<rleInfo> dataRLE = rle.compressBMP(rawdata, false);

		writeRleToFile(dataRLE);

		std::vector<stb::Pixel> decompressedData = rle.deCompressBMP(dataRLE);

		stb.saveAsBMP("outFrame00015.bmp", true, 128, 128, 3, decompressedData);
		
		std::cout << "FRAME NO " << std::stoi(path.substr(frameNoStartPos, 5));

		int frameNumber = std::stoi(path.substr(frameNoStartPos, 5));

		frameNumber++;

		//TODO 00001,00002,00003....
		path.replace(frameNoStartPos, 6, std::to_string(frameNumber));

		std::cout << "PATH " << path <<"\n";
	}

#endif // INPUT_ARG_TYPE
	
}