#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>
#include <ee/cfg_const.h>

#include <sprite2/RVG.h>
#include <sprite2/CameraType.h>

namespace eparticle2d
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();

	std::vector<ee::SprPtr> sprs;
	static_cast<StagePanel*>(m_stage)->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		auto& spr = sprs[i];
		if (!spr->IsVisible())
			continue;
		ee::SpriteRenderer::Instance()->Draw(sprs[i].get());
	}

	if (m_stage->m_ps) {
		m_stage->m_ps->Draw(m_stage->GetPSMat());
	}
	
	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	s2::RVG::SetColor(ee::LIGHT_RED);
	s2::RVG::Rect(nullptr, sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
}

}