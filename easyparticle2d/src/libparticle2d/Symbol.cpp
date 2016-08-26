#include "Symbol.h"
#include "PSConfigMgr.h"

#include <sprite2/Particle2d.h>

namespace eparticle2d
{

Symbol::Symbol()
{
}

void Symbol::LoadResources()
{
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
}

}