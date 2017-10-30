#include "Symbol.h"
#include "Shadow.h"
#include "config.h"
#include "FileIO.h"

#include <ee/Sprite.h>
#include <ee/SymbolType.h>

#include <sprite2/RenderParams.h>
#include <gum/FilepathHelper.h>

namespace eshadow
{

Symbol::Symbol()
{
	m_shadow = new Shadow(SOFT_SHADOW_RADIUS);
}

Symbol::~Symbol()
{
	if (m_shadow) {
		m_shadow->RemoveReference();
	}
}

int Symbol::Type() const
{
	return ee::SYM_SHADOW;
}

s2::RenderReturn Symbol::DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
	}
	if (m_shadow) {
		m_shadow->Draw(p.mt, p.color.GetMul().a);
	}
	return s2::RENDER_OK;
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	if (m_shadow) {
		return m_shadow->GetRegion();
	} else {
		return sm::rect(sm::vec2(0, 0), 200, 200);
	}
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}
	FileIO::LoadFromFile(m_filepath.c_str(), this);
	return true;
}

}