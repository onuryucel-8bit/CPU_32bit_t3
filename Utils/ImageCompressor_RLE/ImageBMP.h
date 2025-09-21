//vector assign - iterator
//https://stackoverflow.com/questions/4423361/constructing-a-vector-with-istream-iterators

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

#include "../../libsLocal/Vec2.hpp"
#include "../../libsLocal/utils/Radix.h"

using vec32bit = sml::Vector2<uint32_t>;

//#define DEBUG_PRINT_BMP_ALL

//#define DEBUG_PRINT_BMPinfoHeader
//#define DEBUG_PRINT_BMPheader
//#define DEBUG_PRINT_BUFFER_SIZE
//#define DEBUG_PRINT_CREATE_HEADER_SIZE

#ifdef DEBUG_PRINT_BMP_ALL
	#define DEBUG_PRINT_BMPinfoHeader
	#define DEBUG_PRINT_BMPheader
	#define DEBUG_PRINT_BUFFER_SIZE
	#define DEBUG_PRINT_CREATE_HEADER_SIZE
#endif // DEBUG_PRINT_BMP_ALL

//14 bytes 112 bits
#pragma pack(push, 1)
typedef struct BMP_Header
{
	uint16_t bitmapSignature_Bytes = 0;//2byte 'BM'
	uint32_t FileSize = 0;//4byte

	//4byte wiki => app specific
	uint32_t reservedBytes = 0;//4byte

	//Offset from beginning of file to the beginning of the bitmap data
	uint32_t pixelDataOffset = 0; //4byte start point of the image data

}bmpHeader;
#pragma pack(pop)

//40 bytes 320 bit
typedef struct BMP_InfoHeader
{
	uint32_t sizeOfInfoHeader = 40; //4byte

	uint32_t width = 0; //4bytes
	uint32_t height = 0;//4bytes

	uint16_t numberOfColorPlanes = 1;//2byte

	uint16_t bitsPerPixel = 0;//2byte

	//0 no Compression , 1 = 8bit RLE , 2 = 4bit RLE
	uint32_t compressionMethod = 0;//4byte

	//(compressed) Size of Image 
	//It is valid to set this = 0 if Compression = 0
	uint32_t imageSize = 0;//4byte

	//XpixelsPerM
	int32_t horizantalResolution = 0;//4byte

	//YpixelsPerM
	int32_t verticalResolution = 0;//4byte

	uint32_t colorsUsed = 0;//4byte

	uint32_t importantColors = 0;//4byte

}bmpInfoHeader;

//3 bytes 24bit

struct Color
{
	uint8_t b = 0;
	uint8_t g = 0;
	uint8_t r = 0;

	Color()
	{

	}
	//for easy use
	Color(uint8_t r_, uint8_t g_, uint8_t b_)
	{
		r = r_;
		g = g_;
		b = b_;
	}


	static const Color Red;
	static const Color Blue;
	static const Color Green;
	static const Color White;
	static const Color Black;
	static const Color TestColor;

};

class ImageBMP
{
public:
	ImageBMP();
	~ImageBMP();

	//loads bmp file to vector<char>
	void load(std::string path);


	void create(std::string fileName, vec32bit size, std::vector<Color>& pixels);
	void createStandart(std::string fileName, vec32bit size);
	void save();

	void rotate(int base, float radian);

	std::vector<char> getData();
	std::vector<uint8_t> getSubPart(vec32bit pos, vec32bit size);
	void setSubPart(vec32bit pos, vec32bit size, std::vector<uint8_t>& data);

	void setPixel(vec32bit pos, Color color);
	Color getPixel(vec32bit pos);

	void DEBUG_printRawData();
	

private:
	std::vector<char> m_buffer;
	uint32_t m_width; //4bytes
	uint32_t m_height;//4bytes
};