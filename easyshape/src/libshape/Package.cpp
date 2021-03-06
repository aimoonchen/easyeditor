#include "Package.h"
#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace eshape
{

void Paskage::PackShape(ee::Shape* shape, std::ofstream& fout)
{
	if (PolygonShape* poly = dynamic_cast<PolygonShape*>(shape))
	{
		int type = e_polygon;
		fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

		const CU_VEC<sm::vec2>& vertices = poly->GetVertices();
		int size = vertices.size();
		fout.write(reinterpret_cast<const char*>(&size), sizeof(int));
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			fout.write(reinterpret_cast<const char*>(&vertices[i].x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&vertices[i].y), sizeof(float));
		}
	}
	else if (CircleShape* circle = dynamic_cast<CircleShape*>(shape))
	{
		int type = e_circle;
		fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

		fout.write(reinterpret_cast<const char*>(&circle->GetCenter().x), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&circle->GetCenter().y), sizeof(float));
		float r = circle->GetRadius();
		fout.write(reinterpret_cast<const char*>(&r), sizeof(float));
	}
}

}