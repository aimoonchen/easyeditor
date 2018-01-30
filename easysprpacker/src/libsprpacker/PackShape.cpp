#include "PackShape.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "PackCoords.h"

#include <easyshape.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <ee/ImageSymbol.h>

#include <sprite2/Shape.h>
#include <simp/NodeShape.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackShape::PackShape(const std::shared_ptr<eshape::Symbol>& sym)
	: m_color(0, 0, 0, 0)
	, m_texture(NULL)
	, m_filling(false)
{
	auto& shape = sym->GetShape();
	if (const s2::PointShape* point = dynamic_cast<const s2::PointShape*>(shape.get()))
	{
		m_type = s2loader::SHAPE_POINT;
		m_vertices.push_back(point->GetPos());
	}
	else if (const s2::RectShape* rect = dynamic_cast<const s2::RectShape*>(shape.get()))
	{
		m_type = s2loader::SHAPE_RECT;
		const sm::rect& r = rect->GetRect();
		m_vertices.push_back(sm::vec2(r.xmin, r.ymin));
		m_vertices.push_back(sm::vec2(r.xmax, r.ymax));
	}
	else if (const s2::CircleShape* circle = dynamic_cast<const s2::CircleShape*>(shape.get()))
	{
		m_type = s2loader::SHAPE_CIRCLE;
		m_vertices.push_back(circle->GetCenter());
		m_vertices.push_back(sm::vec2(circle->GetRadius(), circle->GetRadius()));
	}
	else if (const s2::PolygonShape* polygon = dynamic_cast<const s2::PolygonShape*>(shape.get()))
	{
		m_filling = true;
		m_vertices = polygon->GetVertices();
		auto& p = polygon->GetPolygon();
		if (auto cp = dynamic_cast<const s2::ColorPolygon*>(p.get())) {
			m_type = s2loader::SHAPE_POLYGON_COLOR;
			m_color = cp->GetColor();
		} else if (auto tp = dynamic_cast<const s2::TexturePolygon*>(p.get())) {
			m_type = s2loader::SHAPE_POLYGON_TEXTURE;
			auto mat = dynamic_cast<const eshape::TextureMaterial*>(tp);
			m_texture = PackNodeFactory::Instance()->Create(
				std::dynamic_pointer_cast<const ee::Symbol>(mat->GetImage()));
		}
	}
	else if (auto polyline = dynamic_cast<const s2::PolylineShape*>(shape.get()))
	{
		m_type = s2loader::SHAPE_POLYLINE;
		m_vertices = polyline->GetVertices();
	}
}

PackShape::~PackShape()
{
	if (m_texture) {
		m_texture->RemoveReference();
	}
}

void PackShape::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"shape\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::assign_with_end(gen, "shape_type", m_type);
	if (m_type == s2loader::SHAPE_POLYGON_TEXTURE) {
		lua::assign_with_end(gen, "texture", m_texture->GetID());
	} else {
		lua::assign_with_end(gen, "color", m_color.ToRGBA());
	}

	PackCoords::PackToLuaF32(gen, m_vertices, "vertices");

	gen.detab();
	gen.line("},");
}

int PackShape::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeShape::Size();
	sz += PackCoords::SizeOfUnpackFromBinF32(m_vertices);
	return sz;
}

int PackShape::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);								// id
	sz += sizeof(uint8_t);								// type
	sz += sizeof(uint8_t);								// shape type
	sz += sizeof(uint32_t);								// color or texture_id
	sz += PackCoords::SizeOfPackToBinF32(m_vertices);	// vertices
	return sz;
}

void PackShape::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_SHAPE;
	pack(type, ptr);

	uint8_t shape_type = m_type;
	pack(shape_type, ptr);

	if (m_type == s2loader::SHAPE_POLYGON_TEXTURE) {
		uint32_t id = m_texture->GetID();
		pack(id, ptr);
	} else {
		uint32_t font_color = m_color.ToRGBA();
		pack(font_color, ptr);
	}

	PackCoords::PackToBinF32(m_vertices, ptr);
}

}