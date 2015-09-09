#pragma once

#include <stdint.h>

//#define USE_SOIL

namespace d2d
{

class ImageLoader
{
public:
	static uint8_t* loadTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& channels);

	static uint8_t* loadData(const std::string& filepath, int& width, int& height, int& channels, int& format);

	// alphaΪ0ʱ����rgbҲ��Ϊ0
	// ������shader����ʱ������ֵ�͸�����֣�����ppm��ʽʱ͸���Ĳ��ֲ���
	// �����255
	static void FormatPixelsAlpha(uint8_t* pixels, int width, int height, int val);

	static void loadTexture(unsigned int& texture, const uint8_t* pixel, int width, int height, int channels, int format);

private:
	static uint8_t* loadImageByLibpng(const std::string& filename, int& width, int& height, int& channels, int& format);
#ifdef USE_SOIL
	static uint8_t* loadImageBySOIL(const std::string& filename, int& width, int& height, int& channels);
#endif // USE_SOIL

	static uint8_t* loadPNM(const std::string& filename, int& width, int& height);
	static uint8_t* loadPPM(const std::string& filename, int& width, int& height);
	static uint8_t* loadPGM(const std::string& filename, int& width, int& height);

	static void PreMuiltiAlpha(uint8_t* pixels, int width, int height);

}; // ImageLoader

}