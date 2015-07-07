#include "EditOP.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

namespace eparticle3d
{

EditOP::EditOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool EditOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ParticleSystem* ps = m_stage->m_ps;
	ps->origin = m_stage->TransPosScrToProj(x, y);
	ps->start();
	ps->reset();

	return false;
}

bool EditOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ParticleSystem* ps = m_stage->m_ps;
	ps->origin = m_stage->TransPosScrToProj(x, y);
	ps->start();

	return false;
}

}