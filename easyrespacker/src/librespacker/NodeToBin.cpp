#include "NodeToBin.h"
#include "pack_unpack.h"

#include <ee/Math2D.h>
#include <ee/Exception.h>

#include <math.h>

namespace erespacker
{

int NodeToBin::TransTime(float time)
{
	return static_cast<int>(floor(time * 1000 + 0.5f));
}

int NodeToBin::TransRadian(float r)
{
	return static_cast<int>(floor(r * SM_RAD_TO_DEG + 0.5f));
}

int NodeToBin::TransFloat(float f)
{
	return static_cast<int>(floor(f + 0.5f));
}

int NodeToBin::TransFloatX100(float f)
{
	return TransFloat(100 * f);
}

int NodeToBin::TransFloatX1024(float f)
{
	return static_cast<int>(f * 1024 + 0.5f);
}

int NodeToBin::TransBool(bool b)
{
	return b ? 1 : 0;
}

int NodeToBin::SizeVertices(const CU_VEC<sm::vec2>& vertices)
{
	int sz = 0;
	sz += sizeof(uint16_t);								// number
	sz += sizeof(int32_t) * 2 * vertices.size();		// vertices
	return sz;
}

void NodeToBin::PackVertices(const CU_VEC<sm::vec2>& vertices, uint8_t** ptr, 
							 bool reverse_y, int scale)
{
	if (vertices.size() > USHRT_MAX) {
		throw ee::Exception("NodeToBin::PackVertices num too large.");
	}

	uint16_t num = vertices.size();
	pack(num, ptr);

	for (int i = 0, n = vertices.size(); i < n; ++i) 
	{
		const sm::vec2& pos = vertices[i];
		int32_t x = static_cast<int>(floor(pos.x * scale + 0.5f)),
			    y = static_cast<int>(floor(pos.y * scale + 0.5f));
		if (reverse_y) {
			y = -y;
		}
		pack(x, ptr);
		pack(y, ptr);
	}
}

}