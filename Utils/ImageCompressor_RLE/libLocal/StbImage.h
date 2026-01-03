/*
HATALAR:
	stb bmp dosyasini sag ustten => sol alt kisma dogru okuyor
	flip:true ise okuma yonu
		sol alt => sag ust
*/


#pragma once

#include <vector>
#include <iostream>
#include <cstdint>
#include <filesystem>

#pragma warning(push)
#pragma warning(disable : 4996)
	
	#include "stb_image.h"
	#include "stb_image_write.h"

#pragma warning(pop)

namespace stb
{

using imageData = unsigned char*;

struct Pixel
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;

	bool operator== (Pixel& pixel)
	{
		return 
			r == pixel.r &&
			g == pixel.g &&
			b == pixel.b;
	}

};

class StbImage
{
public:
	~StbImage();

	//Before loading an image, releases the current image data if its loaded
	void loadImg(std::string path, bool verticallFlip = false);
	
	//Saves the currently loaded image file as PNG
	void saveFileAsPNG(std::string path, bool verticallFlip = false);

	void saveAsPNG(std::string path, int width, int height, std::vector<stb::Pixel>& image, int channels = 3, bool verticallFlip = false);
	void saveAsBMP(std::string path, int width, int height, std::vector<stb::Pixel>& image, int channels = 3 , bool verticallFlip = false);

	

	void releaseImgData();
	void createBlankPNG(std::string path, int width, int height, stb::Pixel color, int channels = 3);

	//If image file released returns {0,0,0}
	stb::Pixel getPixel(size_t x, size_t y);
	void setPixel(size_t x, size_t y, stb::Pixel pixel);

	//returns dynamic array as [0] => {r,g,b} [1] => {r,g,b}
	std::vector<stb::Pixel> getImageArr();


	//returns width * height * channels
	int getImageSize();

	//returns image data pointer
	imageData getImage();
	int getWidth();
	int getHeight();
	int getChannels();


private:
	imageData m_file;
	int m_width;
	int m_height;
	int m_channels;
};
}


