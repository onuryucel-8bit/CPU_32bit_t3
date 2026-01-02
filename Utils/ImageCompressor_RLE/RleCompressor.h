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

	void compressBMP(std::vector<rleInfo>& output ,std::vector<stb::Pixel> imgRawData, bool writeCompResult);
	std::vector<stb::Pixel> deCompressBMP(std::vector<rleInfo> compressedData);

private:

	
};

