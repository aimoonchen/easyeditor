#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/DrawShapesVisitor.h>
#include <ee/EditPanelImpl.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/CameraType.h>

namespace etexture
{

StageCanvas::StageCanvas(StagePanel* panel)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_panel(panel)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, wxGLContext* glctx,
						 ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), s2::CAM_ORTHO2D, glctx)
	, m_panel(panel)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<ee::Sprite*> sprs;
	m_sprite_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	m_bg = ee::draw_all_to_one_spr(sprs, m_edited);
}

StageCanvas::~StageCanvas()
{
	if (m_bg) {
		m_bg->RemoveReference();
	}
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		s2::RenderParams params;
		params.mt = m_edited->GetLocalMat().Inverted();
		ee::SpriteRenderer::Instance()->Draw(m_bg, params);
	}

	sm::rect reg = GetVisibleRegion();
	float s = GetCameraScale();
	m_panel->TraverseSprites(ee::DrawSpritesVisitor(reg, s), ee::DT_VISIBLE);
	m_panel->TraverseShapes(ee::DrawShapesVisitor(reg, s), ee::DT_VISIBLE);

	s2::RVG::SetColor(s2::Color(255, 0, 0));
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);

	m_stage->DrawEditOP();
}

}