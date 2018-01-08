#include "StageCanvas.h"

#include <ee/EditPanelImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/color_config.h>

#include <easy3d/ViewFrustum.h>

#include <node3/RenderContext.h>
#include <node3/RenderCtxStack.h>
#include <node3/PrimitiveDraw.h>

namespace ecomplex3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
						 ee::MultiSpritesImpl* sprites_impl, ee::LibraryPanel* library)
	: e3d::StageCanvas(stage_wnd, stage)
	, m_sprites_impl(sprites_impl)
	, m_library(library)
{
}

sm::ivec2 StageCanvas::TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	sm::vec3 pos = GetCamera3().GetModelViewMat() * proj;
	return e3d::ViewFrustum::TransPos3ProjectToScreen(pos, m_screen_width, m_screen_height);
}

sm::vec3 StageCanvas::TransPos3ScreenToProject(const sm::ivec2& scr, float proj_z) const
{
	sm::vec3 pos = GetCamera3().GetModelViewMat() * sm::vec3(0, 0, proj_z);
	return e3d::ViewFrustum::TransPos3ScreenToProject(scr, pos.z, m_screen_width, m_screen_height);
}

sm::vec3 StageCanvas::TransPos3ScreenToDir(const sm::ivec2& screen) const
{
	return e3d::ViewFrustum::TransPos3ScreenToDir(screen, m_screen_width, m_screen_height);
}

void StageCanvas::OnSize(int w, int h)
{
	e3d::StageCanvas::OnSize(w, h);
	m_screen_width = w;
	m_screen_height = h;
}

void StageCanvas::OnDrawSprites() const
{
	auto ctx = n3::RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return;
	}
	const_cast<n3::RenderContext*>(ctx)->SetModelView(GetCamera3().GetModelViewMat());

	DrawBackground();
 	DrawSprites();
 
  	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	n3::PrimitiveDraw::SetColor(ee::LIGHT_RED.ToABGR());
	n3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(10, 10, 10));
//  	e3d::DrawGrids(sm::vec3(-10, -10, 0), sm::vec3(10, 10, 0), sm::vec3(0.5f, 0.5f, FLT_MAX), 
//  		ee::LIGHT_RED);
}

void StageCanvas::DrawSprites() const
{
	std::vector<ee::SprPtr> sprs;
	m_sprites_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));

	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		auto& spr = sprs[i];
		if (!spr->IsVisible())
			continue;
		ee::SpriteRenderer::Instance()->Draw(spr.get());
	}
}

}