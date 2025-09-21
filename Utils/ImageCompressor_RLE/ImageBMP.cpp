#include "ImageBMP.h"


const Color Color::Red = Color{ 0xff, 0,0 };
const Color Color::Blue = Color{ 0, 0, 0xff };
const Color Color::Green = Color{ 0, 0xff, 0 };
const Color Color::White = Color{ 0xff, 0xff, 0xff };
const Color Color::Black = Color{ 0, 0, 0 };
const Color Color::TestColor = Color{ 0xff,0xff,0x0 };

ImageBMP::ImageBMP()
{
	m_width = 0;
	m_height = 0;
}

ImageBMP::~ImageBMP()
{

}

void ImageBMP::load(std::string path)
{
	std::cout << "#! loading bmp path = " << path << "\n";
	std::ifstream image(path, std::ios::binary);

	if (!image)
	{
		std::cerr << "ERROR :: ImageBMP.load() path is invalid\n";
		return;
	}

	//read headers
	bmpHeader header;
	image.read((char*)&header, sizeof(bmpHeader));

	bmpInfoHeader infoHeader;
	image.read((char*)&infoHeader, sizeof(bmpInfoHeader));

	m_width = infoHeader.width;
	m_height = infoHeader.height;

	//check image format 
	if (header.bitmapSignature_Bytes != 0x4D42)
	{
		std::cerr << "ERROR :: ImageBMP.load() :: check image format .bmp\n";
		return;
	}

	//set file cursor to data start point
	image.seekg(header.pixelDataOffset, std::ios::beg);

	//calc padding
	uint32_t paddingAmaount = (4 - infoHeader.width % 4) % 4;

	//calc row size
	uint32_t rowDataSize = infoHeader.width * 3;

	//resize the buffer for data(without padding)
	m_buffer.resize(infoHeader.width * infoHeader.height * 3);

	size_t offset = 0;
	for (size_t i = 0; i < infoHeader.width; i++)
	{
		//read row
		image.read(m_buffer.data() + offset, rowDataSize);
		//move to next part of data (if padding exist skip it)
		image.seekg(paddingAmaount, std::ios::cur);
		offset += rowDataSize;
	}

	image.close();


}

void ImageBMP::create(std::string fileName, vec32bit size, std::vector<Color>& pixels)
{
	std::cout << "#! creating bmp filename = " << fileName << "\n";

	if (size.x * size.y != pixels.size())
	{
		std::cerr << "ERROR :: ImageBMP::create() width*height != given pixel vector size\n";
		return;
	}

	//----calc padding----

	//calc padding
	uint32_t paddingAmaount = (4 - (size.x * 3) % 4) % 4;

	//calc row size
	uint32_t rowDataSize = size.x * 3;

	std::cout << "TEST paddingAmount" << paddingAmaount << " rowDataSize: " << rowDataSize << "\n";

	//------------------------------------------//
	bmpHeader header;
	header.bitmapSignature_Bytes = 0x4D42;//bm	
	header.pixelDataOffset = 0x36;//data start point

	bmpInfoHeader infoHeader;
	infoHeader.width = size.x;
	infoHeader.height = size.y;

	infoHeader.bitsPerPixel = 24; //0x18 => 24bit
	//------------------------------------------//


	//output file
	std::ofstream file(fileName, std::ios::binary);

	file.write((char*)&header, sizeof(bmpHeader));
	file.write((char*)&infoHeader, sizeof(infoHeader));
	//pixels.size() amount of pixels * color size 3 byte = 16*3 = 48byte



	//write pixels + padding to file
	size_t offset = 0;
	for (size_t i = 0; i < size.y; i++)
	{
		//write data row by row
		file.write((char*)(pixels.data() + offset), rowDataSize);

		//write padding if theres a padding
		if (paddingAmaount > 0)
		{
			file.write((char*)0, paddingAmaount);
		}

		//calc offset
		offset += size.x;
	}

	std::cout << "TEST pixels size = " << pixels.size() << " pixels.size() * sizeof Color = " << pixels.size() * sizeof(Color) << "\n";

	file.close();
}

void ImageBMP::createStandart(std::string fileName, vec32bit size)
{
	if (size.x <= 0 || size.y <= 0)
	{
		std::cerr << "ERROR::ImageBMP::createStandart() :: check image width height for <= 0\n";
		return;
	}

	std::vector<Color> pixels;
	for (size_t i = 0; i < size.x * size.y; i++)
	{
		pixels.push_back(Color::Green);
	}

	create(fileName, size, pixels);
}

void ImageBMP::DEBUG_printRawData()
{
	//print the data as hex
	//& 0xff for wrapping .. -1 & 0xff => 0xff
	for (size_t i = 0; i < m_buffer.size(); i++)
	{
		std::cout << std::hex << (static_cast<int>(m_buffer[i]) & 0xff) << " : ";

		if ((i + 1) % 3 == 0)
		{
			std::cout << "\n";
		}

	}
	std::cout << std::dec << "\n";

}

#pragma region TODO_utils

void ImageBMP::save()
{
}

void ImageBMP::rotate(int base, float radian)
{

}

std::vector<char> ImageBMP::getData()
{
	return m_buffer;
}

std::vector<uint8_t> ImageBMP::getSubPart(vec32bit pos, vec32bit size)
{
	std::vector<uint8_t> data;

	return data;
}

void ImageBMP::setSubPart(vec32bit pos, vec32bit size, std::vector<uint8_t>& data)
{
}

void ImageBMP::setPixel(vec32bit pos, Color color)
{
}

Color ImageBMP::getPixel(vec32bit pos)
{
	return Color();
}



#pragma endregion