#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"

#include <sprite2/RenderParams.h>

namespace ep3dinv
{

Symbol::Symbol()
	: m_ps(NULL)
{
}

Symbol::~Symbol()
{
	if (m_ps) {
		delete m_ps;
	}
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = dynamic_cast<const ee::Sprite*>(spr)->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}
	if (m_ps) {
		m_ps->Update(1.0f / 30);
		m_ps->Draw(p.mt);
	}
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	return sm::rect(200, 200);
}

void Symbol::LoadResources()
{
	if (!m_ps) {
		m_ps = new ParticleSystem;
	}
	m_ps->LoadFromFile(m_filepath);
}

}