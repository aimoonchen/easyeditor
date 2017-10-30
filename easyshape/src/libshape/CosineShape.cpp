#include "CosineShape.h"

#include <ee/SettingData.h>

namespace eshape
{

CosineShape::CosineShape(const CosineShape& cosine)
	: ee::Shape(cosine)
	, s2::CosineShape(cosine)
{
}

CosineShape::CosineShape(const CU_VEC<sm::vec2>& vertices)
	: s2::CosineShape(vertices)
{
}

void CosineShape::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_mid_points.size(); i < n; ++i) {
		m_mid_points[i] += offset;
	}
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_bounding.Translate(offset);
}

// void CosineShape::Draw(const S2_MAT& mt, const s2::RenderColor& color) const
// {
// 	if (m_vertices.empty()) return;
// 
// 	s2::RVG::SetColor(color.mul);
// 	s2::RVG::Polyline(m_midPoints, m_loop);
// 	if (ee::SettingData::ctl_pos_sz != 0) {
// 		s2::RVG::SetColor(s2::Color(102, 204, 102));
// 		s2::RVG::Circles(m_vertices, static_cast<float>(ee::SettingData::ctl_pos_sz), true);
// 	}
// 	if (ee::SettingData::ctl_pos_sz != 0) {
// 		s2::RVG::SetColor(s2::Color(204, 204, 102));
// 		s2::RVG::Circles(m_midPoints, ee::SettingData::ctl_pos_sz * 0.5f, true);
// 	}
// }

}