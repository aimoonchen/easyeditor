#ifndef _EASYEDITOR_MIN_BOUNDING_BOX_H_
#define _EASYEDITOR_MIN_BOUNDING_BOX_H_

#include <SM_Vector.h>
#include <cu/cu_stl.h>

namespace ee
{

class MinBoundingBox
{
public:

	// bounding:
	// 1  2
	// 0  3
	static bool Do(const CU_VEC<sm::vec2>& points,
		sm::vec2 bounding[4]);

private:
	static bool TextOtherDir(const CU_VEC<sm::vec2>& points, 
		const sm::vec2& start, const sm::vec2& end, 
		float& min_area, sm::vec2 bounding[4]);
	static void CalculateB(const CU_VEC<sm::vec2>& points, float k, 
		float& b_min, float& b_max);

}; // MinBoundingBox

}

#endif // _EASYEDITOR_MIN_BOUNDING_BOX_H_