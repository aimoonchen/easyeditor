#include "PackIcon.h"
#include "binary_io.h"
#include "PackCoords.h"
#include "PackNodeFactory.h"

#include <easyicon.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeIcon.h>
#include <simp/simp_types.h>
#include <sprite2/ImageSymbol.h>
#include <sprite2/StaticRectIcon.h>
#include <sprite2/StaticQuadIcon.h>
#include <sprite2/DynamicRectIcon.h>
#include <sprite2/DynamicSectorIcon.h>

namespace esprpacker
{

PackIcon::PackIcon(const std::shared_ptr<eicon::Symbol>& sym)
	: m_type(s2loader::ICON_INVALID)
{
	m_base = PackNodeFactory::Instance()->Create(std::dynamic_pointer_cast<ee::Symbol>(
		sym->GetIcon()->GetImage()));

	auto& icon = sym->GetIcon();
	if (const s2::StaticRectIcon* rect = dynamic_cast<const s2::StaticRectIcon*>(icon.get()))
	{
		m_type = s2loader::ICON_RECT;
		sm::vec2 min, max;
		rect->GetRegion(min, max);
		m_vertices.push_back(min);
		m_vertices.push_back(max);
	}
	else if (const s2::StaticQuadIcon* quad = dynamic_cast<const s2::StaticQuadIcon*>(icon.get()))
	{
		m_type = s2loader::ICON_QUAD;
		const sm::vec2* scr = quad->GetScreen();
		for (int i = 0; i < 4; ++i) {
			m_vertices.push_back(scr[i]);
		}
	}
	else if (const s2::DynamicRectIcon* drect = dynamic_cast<const s2::DynamicRectIcon*>(icon.get()))
	{
		m_type = s2loader::ICON_CHANGED_RECT;
		sm::rect begin, end;
		drect->GetRegion(begin, end);
		m_vertices.push_back(sm::vec2(begin.xmin, begin.ymin));
		m_vertices.push_back(sm::vec2(begin.xmax, begin.ymax));
		m_vertices.push_back(sm::vec2(end.xmin, end.ymin));
		m_vertices.push_back(sm::vec2(end.xmax, end.ymax));
	}
	else if (const s2::DynamicSectorIcon* dsector = dynamic_cast<const s2::DynamicSectorIcon*>(icon.get()))
	{
		m_type = s2loader::ICON_CHANGED_SECTOR;
		float min, max;
		dsector->GetRegion(min, max);
		m_vertices.push_back(sm::vec2(min, max));
	}
}

PackIcon::~PackIcon() 
{ 
	if (m_base) { 
		m_base->RemoveReference(); 
	} 
}

void PackIcon::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::connect(gen, 1, 
		lua::assign("base_id", m_base->GetID()));

	lua::assign_with_end(gen, "type", "\"icon\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::assign_with_end(gen, "icon_type", m_type);

	PackCoords::PackToLua(gen, m_vertices, "vertices");

	gen.detab();
	gen.line("},");
}

int PackIcon::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeIcon::Size();
	sz += PackCoords::SizeOfUnpackFromBin(m_vertices);
	return sz;
}

int PackIcon::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);								// id
	sz += sizeof(uint8_t);								// type
	sz += sizeof(uint32_t);								// base id
	sz += sizeof(uint8_t);								// icon type
	sz += PackCoords::SizeOfPackToBin(m_vertices);	// vertices
	return sz;
}

void PackIcon::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_ICON;
	pack(type, ptr);

	assert(m_base);
	uint32_t base_id = m_base->GetID();
	pack(base_id, ptr);

	uint8_t icon_type = m_type;
	pack(icon_type, ptr);

	PackCoords::PackToBin(m_vertices, ptr);
}

}