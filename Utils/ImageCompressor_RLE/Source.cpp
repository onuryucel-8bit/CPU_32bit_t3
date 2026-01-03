#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include "libLocal/StbImage.h"
#include "utils/Radix.h"
#include "RleCompressor.h"

//#define INPUT_ARG_TYPE

namespace fs = std::filesystem;

std::vector<stb::Pixel> readImage(stb::StbImage& stb, std::filesystem::path path)
{
	stb.loadImg(path.string());

	if (stb.getImage() == nullptr)
	{
		return {};
	}

	std::vector<stb::Pixel> imgRawData;

	imgRawData.reserve(stb.getWidth() * stb.getHeight() * stb.getChannels());

#ifdef DEBUG_PIXEL
	std::cout << "Print PIXEL\n";
#endif // DEBUG_PIXEL

	//load image pixels to vector
	for (size_t i = 0; i < stb.getHeight(); i++)
	{
		for (size_t j = 0; j < stb.getWidth(); j++)
		{
			stb::Pixel pixel = stb.getPixel(j, i);

#ifdef DEBUG_PIXEL
			std::cout << std::hex << "x[" << j << "] i[" << i <<"]\n"
				<< (int)pixel.r << " "
				<< (int)pixel.g << " "
				<< (int)pixel.b << " " << "\n";
#endif // DEBUG_PIXEL

			uint8_t averageColor = (pixel.r + pixel.g + pixel.b) / 3;

			//read pixels as white or black
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

void writeRleToFile(std::vector<rleInfo>& data, std::string fileName)
{
	std::ofstream file(fileName);

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

void writeRleToFile(std::vector<stb::Pixel>& data, std::string fileName)
{
	std::ofstream file(fileName);

	if (!file.is_open())
	{
		std::cout << "ERROR::writeRleToFile:: couldnt open the file\n";
		return;
	}

	file << ".db ";

	for (size_t i = 0; i < data.size(); i++)
	{
		file << "0x" << std::hex << (int)data[i].r;
		
		if (i != data.size() - 1)
		{
			file << ",";
			
		}

		if ((i + 1) % 32 == 0)
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

	std::string fileName = "images\\frame_1.bmp";
	
	fs::path imagePath = fileName;
	std::cout << "imagepath" << fs::absolute(imagePath)<< "\n";

	//get absolute path of the image file
	imagePath = fs::absolute(imagePath);

	//holds first number in frame_15.bmp => [1] holds 
	size_t underscorePos = imagePath.string().find_last_of("_");

	//holds '.' index
	size_t dotPos = imagePath.string().find_last_of(".");

	size_t frameIndex = std::stoi(imagePath.string().substr(underscorePos + 1, dotPos - underscorePos));
	underscorePos++;
	std::cout << frameIndex << "\n";

	
	int limit = 1095;
	std::vector<rleInfo> dataRLE;

	for (size_t i = 0; i < limit; i++)
	{	
		std::cout << "Compressing[" << imagePath.string() << "]\n";

		std::vector<stb::Pixel> rawdata = readImage(stb, imagePath);

		if (rawdata.empty())
		{
			return -1;
		}

		//write data as txt
		//append
		//outputRaw.insert(outputRaw.end(), rawdata.begin(), rawdata.end());

		//save output image
		//stb.saveAsBMP("output\\outFrame_" + std::to_string(frameIndex) + ".bmp", 32, 32, rawdata);

		//compressed rle data
		rle.compressBMP(dataRLE, rawdata, false);
						
		//next frame path
		frameIndex++;
		size_t nx = std::to_string(frameIndex).length();
		std::string file_name = imagePath.string();
		file_name.replace(underscorePos, file_name.length() - underscorePos, std::to_string(frameIndex) + ".bmp");

		imagePath /= file_name;
	}

	//write .txt
	//writeRleToFile(outputRaw, "rleRAM_data\\output_frame_t5" + std::to_string(frameIndex) + ".txt");
	writeRleToFile(dataRLE, "rleRAM_data\\output_" + std::to_string(frameIndex) + ".txt");

#endif // INPUT_ARG_TYPE
	
}

