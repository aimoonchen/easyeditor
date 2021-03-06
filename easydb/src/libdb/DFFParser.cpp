#include "DFFParser.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>

#include <gimg_import.h>
#include <gimg_typedef.h>
#include <gimg_export.h>
#include <gum/Config.h>

#include <assert.h>

namespace edb
{

DFFParser::DFFParser(const char* filepath)
{
	int w, h, fmt;
	uint8_t* pixels = gimg_import(filepath, &w, &h, &fmt);
	if (fmt == GPF_RGBA8 && gum::Config::Instance()->GetPreMulAlpha()) {
		gimg_pre_mul_alpha(pixels, w, h);
	}
	assert(w == EDGE && h == EDGE && fmt == GPF_RGBA8);
	m_alphas = new unsigned char[EDGE*EDGE];

	int ptr = 3;
	for (int j = 0; j < EDGE; ++j) {
		for (int i = 0; i < EDGE; ++i) {
			m_alphas[(EDGE-1-j)*EDGE+i] = pixels[ptr];
			ptr += 4;
		}
	}

	delete[] pixels;

	m_dir = ee::FileHelper::GetFileDir(filepath);
}

void DFFParser::outputImage(int width, int height)
{
	uint8_t* pixels = new uint8_t[width*height*4];
	int ptr = 0;
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			for (int k = 0; k < 3; ++k) {
				pixels[ptr++] = 0;
			}
			pixels[ptr++] = computeAlpha(width, height, i, j);
		}
	}

	std::string filepath = m_dir + "_" + ee::StringHelper::ToString(width) + "_" + ee::StringHelper::ToString(height) + ".png";
	gimg_export(filepath.c_str(), pixels, width, height, GPF_RGBA8, true);
	delete[] pixels;
}

inline float interpolate(int min, int max, float percent)
{
	return min + (max - min) * percent;
}

int DFFParser::computeAlpha(int width, int height, int x, int y)
{
	// todo width < EDGE

	int alpha;
	if (width == EDGE && height == EDGE)
	{
		alpha = getAlpha(x, y);
	}
	else
	{
		int xlen = width / EDGE;
		int ylen = height / EDGE;
		int h_xlen = xlen >> 1;
		int h_ylen = ylen >> 1;

		int left, right, top, bottom;
		if (x < h_xlen) {
			left = right = 0;
		} else if (x >= width - h_xlen) {
			left = right = 31;
		} else {
			left = (x - h_xlen) / xlen;
			right = left + 1;
		}
		if (y < h_ylen) {
			top = bottom = 0;
		} else if (y >= height - h_ylen) {
			top = bottom = 31;
		} else {
			top = (y - h_ylen) / ylen;
			bottom = top + 1;
		}
		int left_top = getAlpha(left, top);
		int left_bottom = getAlpha(left, bottom);
		int right_bottom = getAlpha(right, bottom);
		int right_top = getAlpha(right, top);

		left = h_xlen + xlen * left;
		right = left + xlen;
		top = h_ylen + ylen * top;
		bottom = top + ylen;

		float top_val, bottom_val;
		if (x >= left && x <= right) {
			float percent = (x - left)/(float)xlen;
			assert(percent >= 0 && percent <= 1);
			top_val = interpolate(left_top, right_top, percent);
			bottom_val = interpolate(left_bottom, right_bottom, percent);
		} else if (x < left) {
			top_val = left_top;
			bottom_val = left_bottom;
		} else if (x > right) {
			top_val = right_top;
			bottom_val = right_bottom;
		}

		if (y >= top && y <= bottom) {
			float percent = (y - top)/(float)ylen;
			assert(percent >= 0 && percent <= 1);
			alpha = interpolate(top_val, bottom_val, percent);		
		} else if (y < top) {
			alpha = top_val;
		} else if (y > bottom) {
			alpha = bottom_val;
		}
	}

 	if (alpha >= 128) {
  		//alpha = (alpha - 128) / 128.0f * 255.0f;
 
 		float factor = 0.25f + 0.5f * alpha / 255.0f;
 		alpha = 255 * factor;
  	} else {
  		alpha = 0;
  	}

	return alpha;
}

int DFFParser::getAlpha(int x, int y)
{
	assert(x >= 0 && x < EDGE && y >= 0 && y < EDGE);
	return m_alphas[y * EDGE + x];
}

void DFFParser::outputImageFast(int width, int height)
{
	int size = width*height*4;
	uint8_t* pixels = new uint8_t[size];
	memset(&pixels[0], 0, size);

	fillingAlphaNew(pixels, width, height);

	std::string filepath = m_dir + "_" + ee::StringHelper::ToString(width) + "_" + ee::StringHelper::ToString(height) + ".png";
	gimg_export(filepath.c_str(), pixels, width, height, GPF_RGBA8, true);
	delete[] pixels;
}

void DFFParser::fillingAlphaNew(uint8_t* pixels, int width, int height) const
{
	float cross[EDGE][EDGE][3];

	int ptr = 0;
	for (int j = 0; j < EDGE; ++j) {
		for (int i = 0; i < EDGE; ++i) {
			float x = (i + 0.5f) * width / EDGE;
			float y = (j + 0.5f) * height / EDGE;
			float alpha = m_alphas[ptr++];
			cross[j][i][0] = x;
			cross[j][i][1] = y;
			cross[j][i][2] = alpha;
		}
	}

	for (int j = 0; j < EDGE-1; ++j) {
		for (int i = 0; i < EDGE-1; ++i) {
			float left = cross[j][i][0];
			float right = cross[j][i+1][0];
			float top = cross[j][i][1];
			float bottom = cross[j+1][i][1];

			float left_top = cross[j][i][2];
			float left_bottom = cross[j+1][i][2];
			float right_top = cross[j][i+1][2];
			float right_bottom = cross[j+1][i+1][2];

			int ileft = std::ceil(left);
			int iright = std::ceil(right);
			int itop = std::ceil(top);
			int ibottom = std::ceil(bottom);
			if (i == 0) {
				ileft = 0;
			} else if (i == EDGE - 2) {
				iright = width;
			}
			if (j == 0) {
				itop = 0;
			} else if (j == EDGE - 2) {
				ibottom = height;
			}
			for (int j = itop; j < ibottom; ++j) {
				for (int i = ileft; i < iright; ++i) {
					float xfactor;
					if (i < left) {
						xfactor = (right-left)/(right-i);
					} else if (i > right) {
						xfactor = (right-left)/(i-left);
					} else {
						xfactor = (right-i)/(right-left);
					}
					float yfactor;
					if (j < top) {
						yfactor = (bottom-top)/(bottom-j);
					} else if (j > bottom) {
						yfactor = (bottom-top)/(j-top);
					} else {
						yfactor = (bottom-j)/(bottom-top);
					}
					pixels[(j*width+i)*4+3] = 
						xfactor*yfactor*left_top +
						xfactor*(1-yfactor)*left_bottom +
						(1-xfactor)*(1-yfactor)*right_bottom +
						(1-xfactor)*yfactor*right_top;
				}
			}
		}
	}
}

void DFFParser::fillingAlpha(uint8_t* pixels, int width, int height) const
{
	float cross[EDGE+2][EDGE+2][3];

	// filling cross node
	int ptr = 0;
	for (int j = 0; j < EDGE; ++j) {
		for (int i = 0; i < EDGE; ++i) {
			float x = (i + 0.5f) * width / EDGE;
			float y = (j + 0.5f) * height / EDGE;
			float alpha = m_alphas[ptr++];

// 			int pos = (y * width + x) * 4 + 3;
// 			pixels[pos] = alpha;

			cross[j+1][i+1][0] = x;
			cross[j+1][i+1][1] = y;
			cross[j+1][i+1][2] = alpha;
		}
	}

	cross[0][0][0] = cross[0][0][1] = cross[EDGE+1][0][0] = cross[0][EDGE+1][1] = 0;
	cross[0][EDGE+1][0] = cross[EDGE+1][EDGE+1][0] = width-1;
	cross[EDGE+1][0][1] = cross[EDGE+1][EDGE+1][1] = height-1;
	for (int i = 1; i < EDGE+1; ++i) {
		cross[0][i][0] = cross[1][i][0];
		cross[0][i][1] = 0;
		cross[EDGE+1][i][0] = cross[EDGE][i][0];
		cross[EDGE+1][i][1] = height-1;

		cross[i][0][1] = cross[i][1][1];
		cross[i][0][0] = 0;
		cross[i][EDGE+1][1] = cross[i][EDGE][1];
		cross[i][EDGE+1][0] = width-1;
	}

	int hor_offset = 4;
	int ver_offset = 4 * width;

	// filling grid line
	// horizontal
	for (int j = 0; j < EDGE; ++j) {
		for (int i = 0; i < EDGE-1; ++i) {
			float sx = cross[j+1][i+1][0];
			float sy = cross[j+1][i+1][1];
			float sa = cross[j+1][i+1][2];
			float ex = cross[j+1][i+1+1][0];
			float ey = cross[j+1][i+1+1][1];
			float ea = cross[j+1][i+1+1][2];
			assert(sy == ey);
			float dalpha = float(ea - sa) / (ex - sx);
			
			int pos_start = (sy * width + sx) * 4 + 3;
			float curr_alpha = sa;
			for (int k = sx, pos = pos_start; k < ex; ++k) {
				pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
				curr_alpha += dalpha;
				pos += hor_offset;
			}

			if (i == 0)
			{
				curr_alpha = sa - dalpha;
				for (int k = sx - 1, pos = pos_start - hor_offset; k >= 0; --k)
				{
					pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
					curr_alpha -= dalpha;
					pos -= hor_offset;
				}
			}
			else if (i == EDGE - 2)
			{
				curr_alpha = ea;
				for (int k = ex, pos = pos_start+(ex-sx)*hor_offset; k < width; ++k)
				{
					pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
					curr_alpha += dalpha;
					pos += hor_offset;
				}
			}
		}
	}
	// vertical
	for (int i = 0; i < EDGE; ++i) {
		for (int j = 0; j < EDGE-1; ++j) {
			int sx = cross[j+1][i+1][0];
			int sy = cross[j+1][i+1][1];
			int sa = cross[j+1][i+1][2];
			int ex = cross[j+1+1][i+1][0];
			int ey = cross[j+1+1][i+1][1];
			int ea = cross[j+1+1][i+1][2];
			assert(sx == ex);
			float dalpha = float(ea - sa) / (ey - sy);

			int pos_start = (sy * width + sx) * 4 + 3;
			float curr_alpha = sa;
			for (int k = sy, pos = pos_start; k < ey; ++k) {
				pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
				curr_alpha += dalpha;
				pos += ver_offset;
			}

			if (j == 0)
			{
				curr_alpha = sa - dalpha;
				for (int k = sy - 1, pos = pos_start - ver_offset; k >= 0; --k)
				{
					pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
					curr_alpha -= dalpha;
					pos -= ver_offset;
				}
			}
			else if (j == EDGE - 2)
			{
				curr_alpha = ea;
				for (int k = ey, pos = pos_start+(ey-sy)*ver_offset; k < height; ++k)
				{
					pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
					curr_alpha += dalpha;
					pos += ver_offset;
				}
			}
		}
	}
	// left, right
	for (int i = 0; i < 2; ++i)
	{
		int offset = i * (width - 1) * 4;
		int off_cross = (EDGE+1)*i;
		for (int j = 0; j < EDGE-1; ++j)
		{
			int sy = cross[j+1][off_cross][1];
			int pos_start = width*sy*4+offset+3;
			int sa = pixels[pos_start];
			int ey = cross[j+1+1][off_cross][1];
			int pos_end = width*ey*4+offset+3;
			int ea = pixels[pos_end];

			float dalpha = float(ea - sa) / (ey - sy);

			float curr_alpha = sa;
			for (int k = sy, pos = pos_start; k < ey; ++k) {
				pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
				curr_alpha += dalpha;
				pos += ver_offset;
			}

			if (j == 0) {
				curr_alpha = sa - dalpha;
				for (int k = sy - 1, pos = pos_start - ver_offset; k >= 0; --k)
				{
					pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
					curr_alpha -= dalpha;
					pos -= ver_offset;
				}
			} 
			else if (j == EDGE - 2) 
			{
				curr_alpha = ea;
				for (int k = ey, pos = pos_start+(ey-sy)*ver_offset; k < height; ++k)
				{
					pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
					curr_alpha += dalpha;
					pos += ver_offset;
				}
			}
		}		
	}
	// top, bottom
	for (int j = 0; j < 2; ++j)
	{
		int offset = j * (width*(height-1)) * 4;
		int off_cross = (EDGE+1)*j;
		for (int i = 0; i < EDGE-1; ++i)
		{
			int sx = cross[off_cross][i+1][0];
			int pos_start = sx*4+offset+3;
			int sa = pixels[pos_start];
			int ex = cross[off_cross][i+1+1][0];
			int pos_end = ex*4+offset+3;
			int ea = pixels[pos_end];

			float dalpha = float(ea - sa) / (ex - sx);

			float curr_alpha = sa;
			for (int k = sx, pos = pos_start; k < ex; ++k) {
				pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
				curr_alpha += dalpha;
				pos += hor_offset;
			}

			if (i == 0) {
				curr_alpha = sa - dalpha;
				for (int k = sx - 1, pos = pos_start - hor_offset; k >= 0; --k)
				{
					pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
					curr_alpha -= dalpha;
					pos -= hor_offset;
				}
			} 
			else if (i == EDGE - 2) 
			{
				curr_alpha = ea;
				for (int k = ex, pos = pos_start+(ex-sx)*hor_offset; k < width; ++k)
				{
					pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
					curr_alpha += dalpha;
					pos += hor_offset;
				}
			}
		}
	}
	// center hor
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < EDGE+1; ++i) {
			int y = j;
			int sx = cross[0][i][0];
			int ex = cross[0][i+1][0];
			int pos_start = (width * y + sx) * 4 + 3;
			int pos_end = (width * y + ex) * 4 + 3;
			int sa = pixels[pos_start];
			int ea = pixels[pos_end];

			float dalpha = float(ea - sa) / (ex - sx);
			float curr_alpha = sa + dalpha;
			for (int pos = pos_start+hor_offset; pos < pos_end; pos += hor_offset) {
				pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));
				curr_alpha += dalpha;
			}
		}
	}
	// center ver
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < EDGE+1; ++j) {
			int x = i;
			int sy = cross[j][0][1];
			int ey = cross[j+1][0][1];
			int pos_start = (width * sy + x) * 4 + 3;
			int pos_end = (width * ey + x) * 4 + 3; 
			int sa = pixels[pos_start];
			int ea = pixels[pos_end];

			float dalpha = float(ea - sa) / (ey - sy);
			float curr_alpha = sa + dalpha;
			for (int pos = pos_start+ver_offset; pos < pos_end; pos += ver_offset) {
// 				float alpha = (pixels[pos] + curr_alpha) * 0.5f;
// 				pixels[pos] = std::max(0.0f, std::min(255.0f, alpha));

				pixels[pos] = std::max(0.0f, std::min(255.0f, curr_alpha));

				curr_alpha += dalpha;
			}
		}
	}

	int size = width*height*4;
	for (int pos = 3; pos < size; pos += 4) {
		int alpha = pixels[pos];
		if (alpha >= 128) {
			float factor = 0.25f + 0.5f * alpha / 255.0f;
			alpha = 255 * factor;
		} else {
			alpha = 0;
		}
		pixels[pos] = alpha;
	}
}

}