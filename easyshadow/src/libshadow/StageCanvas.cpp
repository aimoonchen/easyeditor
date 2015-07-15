#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

namespace eshadow
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_stage(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, d2d::ISprite* edited, 
						 const d2d::MultiSpritesImpl* bg_sprites)
	: d2d::OrthoCanvas(stage)
	, m_stage(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
}

void StageCanvas::InitGL()
{
	d2d::OrthoCanvas::InitGL();

	d2d::ImageMgr::Instance()->Traverse(d2d::ReloadTextureVisitor<d2d::Image>());
	m_bg = d2d::draw_all_to_one_spr(m_sprite_impl, m_edited);
	ResetViewport();

	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		d2d::Matrix mat(m_edited->GetTransInvMatrix());
		d2d::SpriteRenderer::Instance()->Draw(m_bg, mat);
	}

	m_stage->GetSymbol()->Draw(d2d::Matrix());
	m_stage->DrawEditOP();
}

}