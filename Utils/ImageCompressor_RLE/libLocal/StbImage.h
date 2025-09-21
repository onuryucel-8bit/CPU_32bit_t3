#pragma once

#include <iostream>
#include <cstdint>

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
	uint8_t r;
	uint8_t g;
	uint8_t b;

	bool operator== (Pixel pixel)
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
	void loadImg(std::string path, bool verticallFlip);
	void saveImg();
	void releaseImgData();

	//If image file released returns {0,0,0}
	Pixel getPixel(size_t x, size_t y);

	//returns width * height * channels
	int getImageSize();

	//returns image data pointer
	imageData getImage();
	int getImageWidth();
	int getImageHeight();
	int getImageChannels();

private:
	imageData m_file;
	int m_width;
	int m_height;
	int m_channels;
};
}


