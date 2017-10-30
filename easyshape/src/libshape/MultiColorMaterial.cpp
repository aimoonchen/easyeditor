//#include "MultiColorMaterial.h"
//
//namespace eshape
//{
//
//MultiColorMaterial::MultiColorMaterial(const CU_VEC<sm::vec2>& vertices, 
//							 const s2::Color& color)
//{
//	Build(vertices);
//}
//
//Json::Value MultiColorMaterial::Store(const std::string& dirpath) const
//{
//	Json::Value val;
//	val["type"] = "multi-color";
//	val[""]
//	return val;
//}
//
//void MultiColorMaterial::Draw(const S2_MAT& mt) const
//{
//	s2::RVG::Triangles(mt, m_tris, m_color);
//}
//
//void MultiColorMaterial::Build(const CU_VEC<sm::vec2>& vertices)
//{
//	m_tris.clear();
//
//	CU_VEC<sm::vec2> bounding;
//	ee::Math2D::removeDuplicatePoints(vertices, bounding);
//
//	ee::Triangulation::normal(bounding, m_tris);
//}
//
//void MultiColorMaterial::Build(const CU_VEC<sm::vec2>& vertices, 
//						  const CU_VEC<sm::vec2>& segments)
//{
//	m_tris.clear();
//
//	CU_VEC<sm::vec2> bounding;
//	ee::Math2D::removeDuplicatePoints(vertices, bounding);
//
//	sm::triangulate_lines(bounding, segments, m_tris);
//}
//
//}