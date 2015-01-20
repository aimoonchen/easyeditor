#include "ImagePack.h"
#include "ImageIO.h"

namespace eimage
{

static const int BYTES_PER_PIXEL = 4;

ImagePack::ImagePack(int width, int height)
	: m_width(width)
	, m_height(height)
{
	int sz = width * height * BYTES_PER_PIXEL;
	m_pixels = new uint8_t[sz];
	memset(m_pixels, 0, sizeof(uint8_t) * sz);
}

ImagePack::~ImagePack()
{
	delete[] m_pixels;
}

void ImagePack::AddImage(const d2d::Image* img, int x, int y, int w, int h, bool clockwise)
{
	assert(x >= 0 && x + w <= m_width
		&& y >= 0 && y + h <= m_height);
	const uint8_t* pixels = img->getPixelData();
	if (img->originWidth() == w && img->originHeight() == h) {
		for (int iy = 0; iy < h; ++iy) {
			for (int ix = 0; ix < w; ++ix) {
				int ptr_src = ((h - 1 - iy) * w + ix) * BYTES_PER_PIXEL,
					ptr_dst = ((y + iy) * m_width + (x + ix)) * BYTES_PER_PIXEL;
				for (int i = 0; i < BYTES_PER_PIXEL; ++i) {
					assert(ptr_dst < m_width * m_height * BYTES_PER_PIXEL);
					m_pixels[ptr_dst++] = pixels[ptr_src++];
				}
			}
		}
	} else {
		assert(img->originWidth() == h && img->originHeight() == w);
		if (clockwise) {
	  		for (int iy = 0; iy < w; ++iy) {
	  			for (int ix = 0; ix < h; ++ix) {
	  				int ptr_src = ((w - 1 - iy) * h + ix) * BYTES_PER_PIXEL,
	  					ptr_dst = ((y + ix) * m_width + (x + w - 1 - iy)) * BYTES_PER_PIXEL;
	  				for (int i = 0; i < BYTES_PER_PIXEL; ++i) { 
	  					assert(ptr_dst < m_width * m_height * BYTES_PER_PIXEL);
	  					m_pixels[ptr_dst++] = pixels[ptr_src++];
	  				}
	  			}
	  		}
		} else {
			for (int iy = 0; iy < w; ++iy) {
				for (int ix = 0; ix < h; ++ix) {
					int ptr_src = ((w - 1 - iy) * h + ix) * BYTES_PER_PIXEL,
						ptr_dst = ((y + h - 1 - ix) * m_width + (x + iy)) * BYTES_PER_PIXEL;
					for (int i = 0; i < BYTES_PER_PIXEL; ++i) { 
						assert(ptr_dst < m_width * m_height * BYTES_PER_PIXEL);
						m_pixels[ptr_dst++] = pixels[ptr_src++];
					}
				}
			}
		}
	}
}

void ImagePack::OutputToFile(const wxString& filepath) const
{
	ImageIO::Write(m_pixels, m_width, m_height, filepath.mb_str());
}

}