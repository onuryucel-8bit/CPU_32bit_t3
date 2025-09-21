#include "StbImage.h"


#pragma warning(push)
#pragma warning(disable : 4996)

	#define STB_IMAGE_IMPLEMENTATION
	#include "stb_image.h"

	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include "stb_image_write.h"

#pragma warning(pop)

//void StbImage::convertToPNG()
//{
//	int width, height, channels;
//	imageData img = stbi_load("fall.jpg", &width, &height, &channels, 0);
//	if (img == NULL)
//	{
//		std::cerr << "ERROR::StbImage::convertToPNG() | check image path or image file | img is NULL\n";
//		return;
//	}
//
//	stbi_write_png("fall.png", width, height, channels, img, width * channels);
//
//	stbi_image_free(img);
//}

//void StbImage::removeColorX(COLOR color)
//{
//	int width, height, channels;
//	imageData img = stbi_load("fall.png", &width, &height, &channels, 0);
//	if (img == NULL)
//	{
//		std::cerr << "ERROR::StbImage::removeColorX() | check image path or image file | img is NULL\n";
//		return;
//	}
//
//
//	int pixel_count = width * height;
//	for (int i = 0; i < pixel_count; ++i) 
//	{
//		img[i * channels + color] = 0; 
//	}
//
//	if (!stbi_write_png("output.png", width, height, channels, img, width * channels)) 
//	{
//		std::cerr << "Failed to write image\n";
//		stbi_image_free(img);
//		return;
//	}
//
//	stbi_image_free(img);
//
//}

void StbImage::loadImg(std::string path, bool verticallFlip)
{

	stbi_set_flip_vertically_on_load(verticallFlip);
	m_file = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
	if (m_file == NULL)
	{
		std::cerr << "ERROR::StbImage::removeColorX() | check image path or image file | img is NULL\n";
		return;
	}
}

void StbImage::saveImg()
{
	if (!stbi_write_png("output.png", m_width, m_height, m_channels, m_file, m_width * m_channels))
	{
		std::cerr << "Failed to write image\n";
		stbi_image_free(m_file);
		return;
	}

	
}

void StbImage::releaseImgData()
{
	stbi_image_free(m_file);
}

Pixel StbImage::getPixel(size_t x, size_t y)
{		
	Pixel retPixel = { 0,0,0 };

	int index = (y * m_width + x)* m_channels;
	
	retPixel.r = m_file[index];
	retPixel.g = m_file[index + 1];
	retPixel.b = m_file[index + 2];

	return retPixel;
}

int StbImage::getImageSize()
{
	return m_width * m_height * m_channels;
}

imageData StbImage::getImage()
{
	return m_file;
}

int StbImage::getImageWidth()
{
	return m_width;
}

int StbImage::getImageHeight()
{
	return m_height;
}

int StbImage::getImageChannels()
{
	return m_channels;
}

StbImage::~StbImage()
{
	stbi_image_free(m_file);
}
