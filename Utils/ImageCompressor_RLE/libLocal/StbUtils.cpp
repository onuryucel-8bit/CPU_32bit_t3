#include "StbUtils.h"

namespace stb
{

	StbUtils::StbUtils(stb::StbImage& stbimage)
		:m_stbimage(stbimage)
	{
	}

	StbUtils::~StbUtils()
	{
	}

	void StbUtils::makeGray()
	{
		stb::imageData m_file = m_stbimage.getImage();

		if (m_file == nullptr)
		{
			return;
		}
		
		int width = m_stbimage.getWidth();
		int height = m_stbimage.getHeight();
		int channels = m_stbimage.getChannels();

		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				size_t index = (y * width + x) * channels;

				uint8_t average = (m_file[index + 0] + m_file[index + 1] + m_file[index + 2]) / 3;

				m_stbimage.setPixel(x, y, { average, average, average });
			}
		}
	}
	
	void StbUtils::printAsciiFormat()
	{
		//! @ # $ % ^ & * ( ) _ - = +
		std::vector<stb::Pixel> data = m_stbimage.getImageArr();

		int width = m_stbimage.getWidth();		

		std::ofstream file("output.txt");

		std::string ascii = "      !@#$%^&*()_-=+{}[]:;'\|,.<>/?`~qwertyuiopasdfghjklzxcvbnm1234567";

		for (size_t i = 0; i < data.size(); i++)
		{
			uint8_t average = (data[i].r + data[i].g + data[i].b) / 3;

			if ((i + 1) % width == 0)
			{
				file << '\n';
			}

			uint8_t index = average * ascii.length() / 255;

			file << ascii[index];
		}

		file.close();
	}
}