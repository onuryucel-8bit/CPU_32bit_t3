#pragma once

#include <vector>

#include "libLocal/StbImage.h"


struct rleInfo
{
	size_t counter;
	stb::Pixel color;
};

class RleCompressor
{
public:
	RleCompressor();
	~RleCompressor();

	std::vector<rleInfo> compressBMP(std::vector<stb::Pixel> imgRawData);
	std::vector<stb::Pixel> deCompressBMP(std::vector<rleInfo> compressedData);

private:

	
};

