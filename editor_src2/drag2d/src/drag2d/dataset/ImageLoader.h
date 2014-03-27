#pragma once

#include "common/tools.h"

namespace d2d
{

class ImageLoader
{
public:
	static uint8_t* loadTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& channels);

	static uint8_t* loadData(const std::string& filepath, int& width, int& height, int& channels, int& format);

	// alphaΪ0ʱ����rgbҲ��Ϊ0
	// ������shader����ʱ������ֵ�͸�����֣�����ppm��ʽʱ͸���Ĳ��ֲ���
	static void fixPixelsData(uint8_t* pixels, int width, int height);

private:
	static void loadTexture(unsigned int& texture, uint8_t* pixel, int width, int height, int channels, int format);

	static uint8_t* loadPngByLibpng(const std::string& filename, int& width, int& height, int& channels, int& format);
	static uint8_t* loadPngBySOIL(const std::string& filename, int& width, int& height, int& channels);

	static uint8_t* loadPNM(const std::string& filename, int& width, int& height);
	static uint8_t* loadPPM(const std::string& filename, int& width, int& height);
	static uint8_t* loadPGM(const std::string& filename, int& width, int& height);

}; // ImageLoader

}