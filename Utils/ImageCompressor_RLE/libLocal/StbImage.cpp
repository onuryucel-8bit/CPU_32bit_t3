#include "StbImage.h"


#pragma warning(push)
#pragma warning(disable : 4996)

	#define STB_IMAGE_IMPLEMENTATION
	#include "stb_image.h"

	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include "stb_image_write.h"
#include "StbUtils.h"

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

void StbImage::saveFileAsPNG(std::string path, bool verticallFlip)
{
	if (m_file == nullptr)
	{
		std::cerr << "Current file is empty\n";
		return;
	}

	stbi_flip_vertically_on_write(verticallFlip);
	if (!stbi_write_png(path.c_str(), m_width, m_height, m_channels, m_file, m_width * m_channels))
	{
		std::cerr << "Failed to write image\n";
		stbi_image_free(m_file);
		return;
	}
}

void StbImage::saveAsPNG(std::string path, int width, int height, std::vector<stb::Pixel>& image, int channels, bool verticallFlip)
{
	stbi_flip_vertically_on_write(verticallFlip);
	if (!stbi_write_png(path.c_str(), width, height, channels, image.data(), width * channels))
	{
		std::cerr << "Failed to write image\n";
		stbi_image_free(m_file);
		return;
	}
}

void StbImage::saveAsBMP(std::string path, int width, int height, std::vector<stb::Pixel>& image, int channels, bool verticallFlip)
{
	stbi_flip_vertically_on_write(verticallFlip);
	if (!stbi_write_bmp(path.c_str(), width, height, channels, image.data()))
	{
		std::cerr << "Failed to write image\n";
		stbi_image_free(m_file);		
		return;
	}
}



stb::Pixel StbImage::getPixel(size_t x, size_t y)
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

void StbImage::setPixel(size_t x, size_t y, stb::Pixel pixel)
{
	int index = (y * m_width + x) * m_channels;

	m_file[index] = pixel.r;
	m_file[index + 1] = pixel.g;
	m_file[index + 2] = pixel.b;

}

std::vector<stb::Pixel> StbImage::getImageArr()
{
	std::vector<stb::Pixel> imageData;
	imageData.reserve(m_width * m_height);

	for (size_t i = 0; i < m_width * m_height; i++)
	{
		size_t index = i * m_channels;

		stb::Pixel pixel =
		{
			m_file[index + 0],
			m_file[index + 1],
			m_file[index + 2]
		};
		imageData.push_back(pixel);
	}

	return imageData;
}

int StbImage::getImageSize()
{
	return m_width * m_height * m_channels;
}

imageData StbImage::getImage()
{
	return m_file;
}

int StbImage::getWidth()
{
	return m_width;
}

int StbImage::getHeight()
{
	return m_height;
}

int StbImage::getChannels()
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

void StbImage::createBlankPNG(std::string path, int width, int height, stb::Pixel color, int channels)
{
	std::vector<stb::Pixel> imageData;
	imageData.reserve(width * height);

	for (size_t i = 0; i < width * height; i++)
	{
		imageData.push_back(color);
	}

	saveAsPNG(path, width, height, imageData);
}

StbImage::~StbImage()
{
	if (m_file != nullptr)
	{
		stbi_image_free(m_file);
	}
}

}