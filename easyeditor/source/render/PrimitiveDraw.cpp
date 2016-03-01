#include "PrimitiveDraw.h"
#include "PrimitiveDrawNew.h"
#include "GL10.h"
#include "Symbol.h"
#include "SymbolMgr.h"
#include "Vector.h"
#include "Rect.h"
#include "Color.h"
#include "Math2D.h"
#include "ShaderMgr.h"

#include <rvg.h>
#include <sl_shader.h>

#include <assert.h>

namespace ee
{

static const int MAX_VERTICES = 4096 * 10;
static float VERTICES[MAX_VERTICES * 2];

void PrimitiveDraw::DrawRect(const Vector& center, float radius, const ShapeStyle& style)
{
	DrawRect(center, radius, radius, style);
}

void PrimitiveDraw::DrawRect(const Vector& center, float hWidth, float hHeight, const ShapeStyle& style)
{
	DrawRect(center - Vector(hWidth, hHeight), center + Vector(hWidth, hHeight), style);
}

void PrimitiveDraw::DrawRect(const Rect& r, const ShapeStyle& style)
{
	DrawRect(Vector(r.xmin, r.ymin), Vector(r.xmax, r.ymax), style);
}

void PrimitiveDraw::DrawRect(const Vector& p0, const Vector& p1, const ShapeStyle& style)
{
	//////////////////////////////////////////////////////////////////////////
	rvg_rect(p0.x, p0.y, p1.x, p1.y, false);
	sl_shader_flush();
	//////////////////////////////////////////////////////////////////////////

// 	int type = style.fill ? GL10::GL_QUADS : GL10::GL_LINE_LOOP;
// 	if (!style.fill) {
// 		PrimitiveDrawNew::SetLineWidth(style.size);
// 	}
// 
// 	int idx = 0;
// 	VERTICES[idx++] = p0.x;
// 	VERTICES[idx++] = p0.y;
// 
// 	VERTICES[idx++] = p0.x;
// 	VERTICES[idx++] = p1.y;
// 
// 	VERTICES[idx++] = p1.x;
// 	VERTICES[idx++] = p1.y;
// 
// 	VERTICES[idx++] = p1.x;
// 	VERTICES[idx++] = p0.y;
// 
// 	PrimitiveDrawNew::SetColor(style.color);
// 	PrimitiveDrawNew::LineStypeBegin(style.lineStyle);
// 	PrimitiveDrawNew::Draw(type, VERTICES, 4);
// 	PrimitiveDrawNew::LineStypeEnd(style.lineStyle);
}

void PrimitiveDraw::DrawCircle(const Vector& center, float radius, bool isFill/* = false*/, 
							   float size/* = 2*/, const Colorf& color/* = Colorf(0, 0, 0)*/, size_t kSegments/* = 16*/)
{
	PrimitiveDrawNew::SetColor(color);
	const float k_increment = 2.0f * PI / kSegments;
	float theta = 0.0f;
	if (!isFill)
	{
		PrimitiveDrawNew::SetLineWidth(size);
		int idx = 0;
 		for (size_t i = 0; i < kSegments; ++i)
 		{
 			Vector v = center + Vector(cosf(theta), sinf(theta)) * radius;
			VERTICES[idx++] = v.x;
			VERTICES[idx++] = v.y;
 			theta += k_increment;
 		}
		PrimitiveDrawNew::Draw(GL10::GL_LINE_LOOP, VERTICES, kSegments);
	}
	else
	{
		int idx = 0;
		VERTICES[idx++] = center.x;
		VERTICES[idx++] = center.y;
		for (size_t i = 0; i < kSegments; ++i)
		{
			Vector v = center + Vector(cosf(theta), sinf(theta)) * radius;
			VERTICES[idx++] = v.x;
			VERTICES[idx++] = v.y;
			theta += k_increment;
		}
		VERTICES[idx++] = VERTICES[2];
		VERTICES[idx++] = VERTICES[3];
		PrimitiveDrawNew::Draw(GL10::GL_TRIANGLE_FAN, VERTICES, kSegments + 2);
	}
}

void PrimitiveDraw::DrawCircles(const std::vector<Vector>& circles, float radius, bool isFill/* = false*/, 
								float size/* = 2*/, const Colorf& color/* = Colorf(0, 0, 0)*/, size_t kSegments/* = 16*/)
{
	if (!isFill)
	{
		for (size_t i = 0, n = circles.size(); i < n; ++i) {
			DrawCircle(circles[i], radius, isFill, size, color, kSegments);
		}
	}
	else
	{
		if (circles.empty()) return;

		PrimitiveDrawNew::SetColor(color);
		
		int idx = 0;
		const float k_increment = 2.0f * PI / kSegments;
		for (size_t i = 0, n = circles.size(); i < n; ++i)
		{
			float theta = 0.0f;
			Vector lastPos;
			for (size_t j = 0; j < kSegments; ++j)
			{
				if (j == 0) {
					VERTICES[idx++] = circles[i].x + cosf(theta) * radius;
					VERTICES[idx++] = circles[i].y + sinf(theta) * radius;
				} else {
					VERTICES[idx++] = lastPos.x;
					VERTICES[idx++] = lastPos.y;
				}
				lastPos = circles[i] + Vector(cosf(theta + k_increment), sinf(theta + k_increment)) * radius;
				VERTICES[idx++] = lastPos.x;
				VERTICES[idx++] = lastPos.y;
				VERTICES[idx++] = circles[i].x;
				VERTICES[idx++] = circles[i].y;

				theta += k_increment;
			}
		}
		assert(idx < MAX_VERTICES * 2);
		PrimitiveDrawNew::Draw(GL10::GL_TRIANGLES, VERTICES, idx >> 1);
	}
}

void PrimitiveDraw::DrawCircle(const Matrix& mt, const Vector& center, float radius, bool isFill/* = false*/, 
							   float size/* = 2*/, const Colorf& color/* = Colorf(0, 0, 0)*/, size_t kSegments/* = 16*/)
{
	PrimitiveDrawNew::SetColor(color);
	const float k_increment = 2.0f * PI / kSegments;
	float theta = 0.0f;
	if (!isFill)
	{
		PrimitiveDrawNew::SetLineWidth(size);
		int idx = 0;
		for (size_t i = 0; i < kSegments; ++i)
		{
			Vector v = center + Vector(cosf(theta), sinf(theta)) * radius;
			Vector v_trans = Math2D::TransVector(v, mt);
			VERTICES[idx++] = v_trans.x;
			VERTICES[idx++] = v_trans.y;
			theta += k_increment;
		}
		PrimitiveDrawNew::Draw(GL10::GL_LINE_LOOP, VERTICES, kSegments);
	}
	else
	{
		int idx = 0;
		Vector center_trans = Math2D::TransVector(center, mt);
		VERTICES[idx++] = center_trans.x;
		VERTICES[idx++] = center_trans.y;
		for (size_t i = 0; i < kSegments; ++i)
		{
			Vector v = center + Vector(cosf(theta), sinf(theta)) * radius;
			Vector v_trans = Math2D::TransVector(v, mt);
			VERTICES[idx++] = v_trans.x;
			VERTICES[idx++] = v_trans.y;
			theta += k_increment;
		}
		VERTICES[idx++] = VERTICES[2];
		VERTICES[idx++] = VERTICES[3];
		PrimitiveDrawNew::Draw(GL10::GL_TRIANGLE_FAN, VERTICES, kSegments + 2);
	}
}

void PrimitiveDraw::DrawPoint(const Vector& vertex, const Colorf& color, float size /*= 2*/)
{
	PrimitiveDrawNew::SetPointSize(size);
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(GL10::GL_POINTS, &vertex.x, 1);	
}

void PrimitiveDraw::DrawPoints(const std::vector<Vector>& vertices, const Colorf& color, float size/* = 2*/)
{
	if (vertices.empty()) {
		return;
	}

	PrimitiveDrawNew::SetPointSize(size);
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(GL10::GL_POINTS, &vertices[0].x, vertices.size());
}

void PrimitiveDraw::DrawLine(const Vector& p0, const Vector& p1, 
							 const Colorf& color, float size/* = 2*/)
{
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::SetLineWidth(size);

	VERTICES[0] = p0.x;
	VERTICES[1] = p0.y;
	VERTICES[2] = p1.x;
	VERTICES[3] = p1.y;
	PrimitiveDrawNew::Draw(GL10::GL_LINE_STRIP, VERTICES, 2);
}

void PrimitiveDraw::DrawDotLine(const Vector& p0, const Vector& p1, 
								const Colorf& color, float size /*= 2*/)
{
	PrimitiveDrawNew::LineStypeBegin(LS_DOT);
	DrawLine(p0, p1, color, size);
	PrimitiveDrawNew::LineStypeEnd(LS_DOT);
}

void PrimitiveDraw::DrawDashLine(const Vector& p0, const Vector& p1, 
								 const Colorf& color, float size /*= 2*/)
{
	PrimitiveDrawNew::LineStypeBegin(LS_DASH);
	DrawLine(p0, p1, color, size);
	PrimitiveDrawNew::LineStypeEnd(LS_DASH);
}

void PrimitiveDraw::DrawDotDashLine(const Vector& p0, const Vector& p1, 
									const Colorf& color, float size /*= 2*/)
{
	PrimitiveDrawNew::LineStypeBegin(LS_DOT_DASH);
	DrawLine(p0, p1, color, size);
	PrimitiveDrawNew::LineStypeEnd(LS_DOT_DASH);
}

void PrimitiveDraw::DrawLines(const std::vector<Vector>& vertices, 
							  const Colorf& color, float size /*= 2*/)
{
	if (vertices.empty()) {
		return;
	}
	PrimitiveDrawNew::SetLineWidth(size);
	PrimitiveDrawNew::Draw(GL10::GL_LINES, &vertices[0].x, vertices.size());
}

void PrimitiveDraw::DrawPolyline(const std::vector<Vector>& vertices, 
								 const Colorf& color, bool isClose, float size /*= 2*/)
{
	if (vertices.empty()) {
		return;
	}
	int type = isClose ? GL10::GL_LINE_LOOP : GL10::GL_LINE_STRIP;
	PrimitiveDrawNew::SetLineWidth(size);
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(type, &vertices[0].x, vertices.size());
}
 
void PrimitiveDraw::DrawPolygon(const std::vector<Vector>& vertices, const Colorf& color)
{
	if (vertices.empty()) {
		return;
	}
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(GL10::GL_POLYGON, &vertices[0].x, vertices.size());
}

void PrimitiveDraw::DrawTriangles(const std::vector<Vector>& triangles, const Colorf& color)
{
	if (triangles.empty()) {
		return;
	}

	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(GL10::GL_TRIANGLES, &triangles[0].x, triangles.size());
}

void PrimitiveDraw::DrawTrianglesLine(const std::vector<Vector>& triangles, const Colorf& color, float size)
{
	if (triangles.empty()) {
		return;
	}

	PrimitiveDrawNew::SetLineWidth(size);
	PrimitiveDrawNew::SetColor(color);
	for (int i = 0, n = triangles.size(); i < n; i += 3) {
		PrimitiveDrawNew::Draw(GL10::GL_LINE_LOOP, &triangles[i].x, 3);	
	}
}

void PrimitiveDraw::Cross(const Vector& center, float edge, const Colorf& color, float size)
{
	Cross(center, edge, edge, color, size);
}

void PrimitiveDraw::Cross(const Vector& center, float xedge, float yedge, const Colorf& color, float size)
{
	Vector s = center, e = center;
	s.x -= xedge;
	e.x += xedge;
	DrawLine(s, e, color, size);

	s  = e = center;
	s.y -= yedge;
	e.y += yedge;
	DrawLine(s, e, color, size);
}

//////////////////////////////////////////////////////////////////////////
// todo: with Matrix
//////////////////////////////////////////////////////////////////////////

void PrimitiveDraw::DrawRect(const Matrix& mt, float hWidth,
						 float hHeight, const ShapeStyle& style)
{
	DrawRect(mt, -Vector(hWidth, hHeight), Vector(hWidth, hHeight), style);
}

void PrimitiveDraw::DrawRect(const Matrix & mt, const Rect& r,
						 const ShapeStyle& style)
{
	DrawRect(mt, Vector(r.xmin, r.ymin), Vector(r.xmax, r. ymax), style);
}

void PrimitiveDraw::DrawRect(const Matrix& mt, const Vector& p0,
						 const Vector& p1, const ShapeStyle& style)
{
	int type = style.fill ? GL10::GL_TRIANGLE_FAN : GL10::GL_LINE_LOOP;
	if (!style.fill) {
		PrimitiveDrawNew::SetLineWidth(style.size);
	}

	int idx = 0;
	Vector p = Math2D::TransVector(p0, mt);
	VERTICES[idx++] = p.x;
	VERTICES[idx++] = p.y;
	p = Math2D::TransVector(Vector(p0.x, p1.y), mt);
	VERTICES[idx++] = p.x;
	VERTICES[idx++] = p.y;
	p = Math2D::TransVector(p1, mt);
	VERTICES[idx++] = p.x;
	VERTICES[idx++] = p.y;
	p = Math2D::TransVector(Vector(p1.x, p0.y), mt);
	VERTICES[idx++] = p.x;
	VERTICES[idx++] = p.y;

	PrimitiveDrawNew::SetColor(style.color);
	PrimitiveDrawNew::Draw(type, &VERTICES[0], 4);
}

void PrimitiveDraw::DrawPoint(const Matrix& mt, const Vector& vertex, const Colorf& color, float size)
{
	Vector new_vertex = Math2D::TransVector(vertex, mt);
	DrawPoint(vertex, color, size);
}

void PrimitiveDraw::DrawPolyline(const Matrix& mt, const std::vector<Vector>& vertices, 
								 const Colorf& color, bool isClose, float size) 
{
	std::vector<Vector> trans;
	MatrixTrans(mt, vertices, trans);
	DrawPolyline(trans, color, isClose, size);
}

void PrimitiveDraw::DrawTriangles(const Matrix& mt, const std::vector<Vector>& triangles, const Colorf& color)
{
	if (triangles.empty()) {
		return;
	}

	std::vector<Vector> trans;
	MatrixTrans(mt, triangles, trans);	

	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(GL10::GL_TRIANGLES, &trans[0].x, triangles.size());
}

void PrimitiveDraw::DrawTriangles(const Matrix& mt, const std::vector<Vector>& triangles, const std::vector<Colorf>& colors)
{
	if (triangles.empty() || triangles.size() != colors.size()) {
		return;
	}

	std::vector<Vector> trans;
	MatrixTrans(mt, triangles, trans);	

	PrimitiveDrawNew::Draw(GL10::GL_TRIANGLES, &trans[0].x, &colors[0].r, triangles.size());	
}

void PrimitiveDraw::MatrixTrans(const Matrix& mt, const std::vector<Vector>& src, std::vector<Vector>& dst)
{
	if (src.empty()) {
		return;
	}

	dst.clear();
	dst.reserve(src.size());
	for (int i = 0, n = src.size(); i < n; ++i) {
		dst.push_back(Math2D::TransVector(src[i], mt));
	}
}

}