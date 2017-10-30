#include "StageCanvas.h"
#include "StagePanel.h"
#include "MotionTrail.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/cfg_const.h>

#include <sprite2/RVG.h>
#include <sprite2/CameraType.h>

namespace etrail
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
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

	auto& trail = m_stage->m_trail;
	if (trail) {
		trail->Draw(S2_MAT());
	}
// 	for (int i = 0, n = m_stage->m_trails.size(); i < n; ++i) {
// 		m_stage->m_trails[i]->Draw(S2_MAT());
// 	}

	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	s2::RVG::SetColor(s2::Color(204, 204, 204));
	s2::RVG::LineWidth(2);
	s2::RVG::Rect(sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
}

}