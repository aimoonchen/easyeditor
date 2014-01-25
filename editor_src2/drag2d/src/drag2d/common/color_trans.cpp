#include "color_trans.h"

#include <string>
#include <assert.h>

namespace d2d
{
Colorf transColor(const std::string& str, PixelType type) 
{
	if (str.empty())
		return Colorf(0, 0, 0, 1);

	if (str == "0xffffffff")
		return Colorf(1, 1, 1, 1);

	int len = str.length();

	Colorf ret(0, 0, 0, 1);
	if (len == 4)
	{
		if (type == PT_RGBA || PT_BGRA)
			ret.a = transColor(str[2], str[3]);
		else if (type == PT_ARGB)
			ret.b = transColor(str[2], str[3]);
	}
	else if (len == 10)
	{
		if (type == PT_RGBA) {
			ret.r = transColor(str[2], str[3]);
			ret.g = transColor(str[4], str[5]);
			ret.b = transColor(str[6], str[7]);
			ret.a = transColor(str[8], str[9]);
		} else if (type == PT_ARGB) {
			ret.a = transColor(str[2], str[3]);
			ret.r = transColor(str[4], str[5]);
			ret.g = transColor(str[6], str[7]);
			ret.b = transColor(str[8], str[9]);
		} else if (type == PT_BGRA) {
			ret.b = transColor(str[2], str[3]);
			ret.g = transColor(str[4], str[5]);
			ret.r = transColor(str[6], str[7]);
			ret.a = transColor(str[8], str[9]);
		}
	}

	return ret;
}

float transColor(char high, char low) 
{
	int col = transHex(high) * 16 + transHex(low);
	assert(col >= 0 && col <= 255);
	return col / 255.0f;
}

int transHex(char c) 
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else {
		assert(0);
		return 0;
	}
}

std::string transColor(const Colorf& col, PixelType type) 
{
	std::string ret = "0x";
	if (type == PT_RGBA) {
		ret += transColor(col.r);
		ret += transColor(col.g);
		ret += transColor(col.b);
		ret += transColor(col.a);
	} else if (type == PT_ARGB) {
		ret += transColor(col.a);
		ret += transColor(col.r);
		ret += transColor(col.g);
		ret += transColor(col.b);
	} else if (type == PT_BGRA) {
		ret += transColor(col.b);
		ret += transColor(col.g);
		ret += transColor(col.r);
		ret += transColor(col.a);
	}
	return ret;
}

std::string transColor(float col) 
{
	assert(col >= 0 && col <= 1.0f);

	int c = (int)(col * 255 + 0.5f);
	int high = c / 16;
	int low = c % 16;

	std::string ret;
	ret += transHex(high);
	ret += transHex(low);

	return ret;
}

char transHex(int v) 
{
	assert(v >= 0 && v <= 15);
	if (v >= 0 && v <= 9)
		return '0' + v;
	else if (v >= 10 && v <= 15)
		return 'a' + (v - 10);
	else {
		assert(0);
		return '0';
	}
}
}