#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

namespace ep3dinv
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	ee::Rect sr = m_screen.GetRegion();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);

	if (m_stage->m_ps) {
		m_stage->m_ps->Update(1.0f / 30);
		m_stage->m_ps->Draw(ee::Matrix());
	}

	m_stage->DrawEditOP();
}

}