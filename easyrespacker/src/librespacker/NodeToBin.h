#ifndef _EASYRESPACKER_NODE_TO_BIN_H_
#define _EASYRESPACKER_NODE_TO_BIN_H_

#include "typedef.h"

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <stdint.h>

namespace erespacker
{

class NodeToBin
{
public:
	static int TransTime(float time);
	static int TransRadian(float r);
	static int TransFloat(float f);
	static int TransFloatX100(float f);
	static int TransFloatX1024(float f);
	static int TransBool(bool b);

	static int  SizeVertices(const CU_VEC<sm::vec2>& vertices);
	static void PackVertices(const CU_VEC<sm::vec2>& vertices, 
		uint8_t** ptr, bool reverse_y = true, int scale = SCALE);

}; // NodeToBin

}

#endif // _EASYRESPACKER_NODE_TO_BIN_H_