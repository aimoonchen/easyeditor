#include "Symbol.h"
#include "PSConfigMgr.h"

#include <sprite2/Particle2d.h>
#include <gum/FilepathHelper.h>

namespace eparticle2d
{

Symbol::Symbol()
{
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(200, 200);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
	return true;
}

}