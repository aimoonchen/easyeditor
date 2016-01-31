#include "ComplexPolygonShape.h"
#include "Material.h"

namespace libshape
{

ComplexPolygonShape::ComplexPolygonShape()
{
}

ComplexPolygonShape::ComplexPolygonShape(const ComplexPolygonShape& poly)
	: PolygonShape(poly)
{
}

ComplexPolygonShape::ComplexPolygonShape(const std::vector<ee::Vector>& outline, 
										 const std::vector<std::vector<ee::Vector> >& holes)
	: PolygonShape(outline)
	, m_holes(holes)
{
	m_material->BuildSetHoles(holes);
	m_material->BuildEnd();
}

bool ComplexPolygonShape::IsContain(const ee::Vector& pos) const
{
	if (PolygonShape::IsContain(pos)) {
		for (int i = 0, n = m_holes.size(); i < n; ++i) {
			if (ee::Math2D::IsPointInArea(pos, m_holes[i])) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool ComplexPolygonShape::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPolygonIntersectRect(m_vertices, rect);
}

void ComplexPolygonShape::Translate(const ee::Vector& offset)
{
	PolygonShape::Translate(offset);

	for (int i = 0, n = m_holes.size(); i < n; ++i) {
		for (int j = 0, m = m_holes[i].size(); j < m; ++j) {
			m_holes[i][j] += offset;
		}
	}
}

void ComplexPolygonShape::Draw(const ee::Matrix& mt, const ee::ColorTrans& color) const
{
	PolygonShape::Draw(mt, color);

	if (ee::SettingData::draw_poly_bound) {
		for (int i = 0, n = m_holes.size(); i < n; ++i) {
			ee::PrimitiveDraw::DrawPolyline(mt, m_holes[i], color.multi, m_isLoop);
			if (ee::SettingData::ctl_pos_sz != 0) {
				ee::PrimitiveDraw::DrawCircles(m_holes[i], ee::SettingData::ctl_pos_sz, true, 2, ee::Colorf(0.4f, 0.8f, 0.4f));
			}
		}
	} 
}

}