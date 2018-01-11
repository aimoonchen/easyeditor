#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Settings.h"

#include <ee/dev_config.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/Config.h>
#include <ee/SpriteRenderer.h>
#include <ee/color_config.h>
#include <ee/cfg_const.h>

#include <easyanim.h>
#include <easytext.h>

#include <shaderlab/Statistics.h>
#include <sprite2/RVG.h>
#include <sprite2/Particle3d.h>
#include <sprite2/CameraType.h>
#include <sprite2/OrthoCamera.h>
#include <gum/DTex.h>
#include <gum/GTxt.h>
#include <gum/StringHelper.h>
#include <node3/PrimitiveDraw.h>

namespace ecomplex
{

StageCanvas::StageCanvas(StagePanel* editPanel,
						 ee::LibraryPanel* library,
						 wxGLContext* glctx)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl(), s2::CAM_ORTHO2D, glctx, true, true)
	, m_stage(editPanel)
	, m_library(library)
	, m_background(NULL)
	, m_fps(1)
{
}

void StageCanvas::OnSize(int w, int h)
{
	ee::CameraCanvas::OnSize(w, h);

// 	m_camera3.SetScreenSize(w, h);
// 
// 	e3d::ShaderMgr* shader_mgr = e3d::ShaderMgr::Instance();
// 	shader_mgr->SetProjection(w, h);
// 	shader_mgr->SetModelView(m_camera3.GetModelViewMat());
}

#ifdef OPEN_SCREEN_CACHE

void StageCanvas::OnDrawSprites() const
{
	wxLogDebug("++++++++ StageCanvas::OnDrawSprites begin");

	m_fps.Begin();

	DrawBackground();

	ee::ScreenCache::Instance()->Draw(ee::CameraMgr::Instance()->GetCamera());

	s2::RVG::SetColor(s2::Color(0, 0.8f, 0));
	const sm::rect& r = m_stage->GetSymbol()->GetScissor();
	s2::RVG::Rect(sm::vec2(r.xmin, r.ymin), sm::vec2(r.xmax, r.ymax), m_clipboxStyle.filling);

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		s2::RVG::SetColor(ee::LIGHT_GREY);
		s2::RVG::Cross(sm::vec2(0,0), EDGE, EDGE);
	}

	m_stage->DrawEditOP();

	m_fps.End();

	m_fps.DrawTime();

	wxLogDebug("++++++++ StageCanvas::OnDrawSprites end");
}

#else

void StageCanvas::OnDrawSprites() const
{
	m_fps.Begin();

	DrawBackground();

	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);

	const sm::rect& clipbox = std::dynamic_pointer_cast<Symbol>(m_stage->GetSymbol())->GetScissor();
	sm::vec2 sz = clipbox.Size();
	if (sz.x > 0 && sz.y > 0) {
		s2::RVG::SetColor(s2::Color(0, 204, 0));
		s2::RVG::LineWidth(2);
		s2::RVG::Rect(nullptr, sm::vec2(clipbox.xmin, clipbox.ymin), sm::vec2(clipbox.xmax, clipbox.ymax), false);
	}

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		s2::RVG::SetColor(ee::LIGHT_GREY);
		s2::RVG::Cross(nullptr, sm::vec2(0,0), EDGE, EDGE);
	}

	s2::Particle3d::Instance()->BufferDraw();

	m_stage->DrawEditOP();

	m_fps.End();

	m_fps.DrawTime();

	if (ee::Config::Instance()->IsDrawStatOpen()) {
		DrawStat();
	}
}

#endif // OPEN_SCREEN_CACHE

void StageCanvas::DrawBackground() const
{
	if (m_background) {
		ee::SpriteRenderer::Instance()->Draw(m_background.get());
	}

	if (Settings::bVisibleBGRect) {
		s2::RVG::SetColor(s2::Color(204, 204, 204));
		s2::RVG::LineWidth(2);
		s2::RVG::Rect(nullptr, sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
	}
}

void StageCanvas::DrawStat() const
{
	s2::OrthoCamera* cam = static_cast<s2::OrthoCamera*>(m_cam2d);
	S2_MAT mt;
	float s = std::max(1.0f, cam->GetScale());
	mt.Scale(s, s);
	mt.Translate(cam->GetPosition().x - 200, cam->GetPosition().y + 200);

	int num = sl::Statistics::Instance()->GetVertices() / 6;

	gum::GTxt::Instance()->Draw(mt, gum::StringHelper::ToString(num));

	sl::Statistics::Instance()->Reset();
}

}