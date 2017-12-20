#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/Symbol.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>
#include <ee/Config.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>

#include <sprite2/RVG.h>
#include <sprite2/Particle3d.h>
#include <sprite2/CameraType.h>

namespace eanim
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage_panel(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();

	std::vector<ee::SprPtr> sprs;
	static_cast<StagePanel*>(m_stage_panel)->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs), ee::DT_VISIBLE);
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		ee::SpriteRenderer::Instance()->Draw(sprs[i].get());
	}

	s2::Particle3d::Instance()->BufferDraw();

	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	if (m_background) {
		ee::SpriteRenderer::Instance()->Draw(m_background.get());
	}

	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	s2::RVG::SetColor(ee::LIGHT_GREY);
	s2::RVG::Cross(nullptr, sm::vec2(0,0), xedge, yedge);
}

} // eanim