#ifndef _EASYEDITOR_BOX2D_RENDER_H_
#define _EASYEDITOR_BOX2D_RENDER_H_

#include <SM_Vector.h>
#include <sprite2/Color.h>

#include <Box2D/Box2D.h>

#include <vector>

namespace ee
{

class Box2DRender : public b2Draw
{
public:
	Box2DRender(float ratio);

	virtual void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;

	virtual void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;

	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	virtual void DrawTransform(const b2Transform& xf) override;

	virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

	virtual void DrawString(int x, int y, const char* string, ...); 

	virtual void DrawAABB(b2AABB* aabb, const b2Color& color);

private:
	sm::vec2 TransVertex(const b2Vec2& vertex) const;

	void TransVertices(const b2Vec2* src, int count, std::vector<sm::vec2>& dst) const;

	s2::Color TransColor(const b2Color& col, float alpha = 1) const;

private:
	float m_ratio;

}; // Box2DRender

}

#endif // _EASYEDITOR_BOX2D_RENDER_H_