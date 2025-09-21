#pragma once

#include <iostream>
#include <cstdint>

#pragma warning(push)
#pragma warning(disable : 4996)
	
	#include "stb_image.h"
	#include "stb_image_write.h"

#pragma warning(pop)

using imageData = unsigned char*;

struct Pixel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

class StbImage
{
public:
	~StbImage();

	void loadImg(std::string path, bool verticallFlip);
	void saveImg();
	void releaseImgData();

	Pixel getPixel(size_t x, size_t y);

	//returns width * height * channels
	int getImageSize();

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


