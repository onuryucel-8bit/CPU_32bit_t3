#include "StbImage.h"


#pragma warning(push)
#pragma warning(disable : 4996)

	#define STB_IMAGE_IMPLEMENTATION
	#include "stb_image.h"

	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include "stb_image_write.h"

#pragma warning(pop)

namespace stb
{

void StbImage::loadImg(std::string path, bool verticallFlip)
{
	//Before loading an file, throw old file to garbage
	if (m_file != nullptr)
	{
		releaseImgData();
	}

	stbi_set_flip_vertically_on_load(verticallFlip);
	m_file = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
	if (m_file == nullptr)
	{
		std::cerr << "ERROR::StbImage::loadImg() | check image path or image extension\n";
		return;
	}
} 

void StbImage::saveAsPNG(std::string path, bool verticallFlip)
{
	stbi_flip_vertically_on_write(verticallFlip);
	if (!stbi_write_png(path.c_str(), m_width, m_height, m_channels, m_file, m_width * m_channels))
	{
		std::cerr << "Failed to write image\n";
		stbi_image_free(m_file);
		return;
	}
}

void StbImage::saveAsBMP(std::string path, bool verticallFlip, int width, int height, int channels, std::vector<stb::Pixel> image)
{
	stbi_flip_vertically_on_write(verticallFlip);
	if (!stbi_write_bmp(path.c_str(), width, height, channels, image.data()))
	{
		std::cerr << "Failed to write image\n";
		stbi_image_free(m_file);		
		return;
	}
}

Pixel StbImage::getPixel(size_t x, size_t y)
{
	if (m_file == nullptr)
	{
		std::cout << "WARNING::StbImage::getPixel() image data is null\n";
		return { 0,0,0 };
	}

	Pixel retPixel = { 0,0,0 };

	int index = (y * m_width + x) * m_channels;

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

void StbImage::releaseImgData()
{
	if (m_file != nullptr)
	{
		stbi_image_free(m_file);

		m_file = nullptr;
	}
}

StbImage::~StbImage()
{
	if (m_file != nullptr)
	{
		stbi_image_free(m_file);
	}
}

}