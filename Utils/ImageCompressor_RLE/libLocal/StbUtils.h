#pragma once

#include "StbImage.h"

#include <fstream>

namespace stb
{

	class StbUtils
	{
	public:
		StbUtils(stb::StbImage& stbimage);
		~StbUtils();

		//Makes the currently loaded file gray image
		void makeGray();

		//Prints image as ascii
		void printAsciiFormat();

	private:
		stb::StbImage& m_stbimage;
	};

}