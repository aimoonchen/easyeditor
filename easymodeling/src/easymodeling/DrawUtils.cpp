#include "DrawUtils.h"

#include <ee/Sprite.h>

namespace emodeling
{

void DrawUtils::DrawBody(Body* body, DrawType dType)
{
	pt2::Color cFace, cEdge;
	GetBodyColor(body->m_type, dType, cFace, cEdge);

	body->Draw(body->m_spr->GetLocalMat(), cFace, cEdge);
}

void DrawUtils::DrawFixture(Fixture* fixture, 
							DrawType dType, bool onlyFixture)
{
	pt2::Color cFace, cEdge;
	if (onlyFixture)
		GetFixtureColor(dType, cFace, cEdge);
	else
		GetBodyColor(fixture->m_body->m_type, dType, cFace, cEdge);

	fixture->Draw(fixture->m_body->m_spr->GetLocalMat(), cFace, cEdge);
}

void DrawUtils::GetBodyColor(Body::Type type, DrawType dType,
							 pt2::Color& cFace, pt2::Color& cEdge)
{
	switch (type)
	{
	case Body::e_static:
		cFace.FromFloat(0.5f, 0.9f, 0.5f, 0.2f);
		break;
	case Body::e_kinematic:
		cFace.FromFloat(0.5f, 0.5f, 0.9f, 0.2f);
		break;
	case Body::e_dynamic:
		cFace.FromFloat(0.5f, 0.5f, 0.3f, 0.2f);
		break;
	}

	const float rScale = 1.1f,
		gScale = 1.1f,
		bScale = 0.5f;
	switch (dType)
	{
	case e_default:
		break;
	case e_mouseOn:
		cFace.r *= rScale;
		cFace.g *= gScale;
		cFace.b *= bScale;
		break;
	case e_selected:
		{
			pt2::Color c = cFace;
			c.r *= rScale;
			c.g *= gScale;
			c.b *= bScale;
			cFace = ColorEnlarge(c, 1.1f);
		}
		break;
	}

	cEdge = ColorEnlarge(cFace, 1.05f);
}

void DrawUtils::GetFixtureColor(DrawType type, pt2::Color& cFace, pt2::Color& cEdge)
{
	cFace.FromFloat(0.6f, 0.35f, 0.6f, 0.5f);
	switch (type)
	{
	case e_mouseOn:
		cFace = ColorEnlarge(cFace, 1.1f);
		break;
	case e_selected:
		cFace = ColorEnlarge(cFace, 1.2f);
		break;
	}

	cEdge = ColorEnlarge(cFace, 1.1f);
}

pt2::Color DrawUtils::ColorEnlarge(const pt2::Color& color, float factor)
{
	pt2::Color ret = color;
	ret.r = std::min(color.r * factor, 1.0f);
	ret.g = std::min(color.g * factor, 1.0f);
	ret.b = std::min(color.b * factor, 1.0f);
	ret.a = color.a;

	return ret;
}

}