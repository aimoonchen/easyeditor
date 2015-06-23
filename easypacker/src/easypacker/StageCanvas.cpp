#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "config.h"

namespace epacker
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage_impl(stage)
{
}

void StageCanvas::OnDraw()
{
	d2d::Rect sr = m_screen.GetRegion();
	m_stage_impl->traverseSprites(d2d::DrawSpritesVisitor(sr), d2d::DT_VISIBLE);

	drawRegion();
}

void StageCanvas::drawRegion()
{
	const float width = Context::Instance()->width,
		height = Context::Instance()->height;

	StagePanel* stage = static_cast<StagePanel*>(m_stage);

	// bg
	d2d::PrimitiveDraw::rect(
		d2d::Vector(0, 0), 
		d2d::Vector(width, height), 
		d2d::LIGHT_RED_LINE);

	int x = 0, y = 0;
	int count = stage->GetTextureAccount();
	for (int i = 0; i < count; ++i)
	{
		d2d::PrimitiveDraw::rect(d2d::Vector(x, y), 
			d2d::Vector(x + width, y + height), 
			d2d::LIGHT_GREY_LINE);
		x += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;
	}

	// physics
	stage->drawPhysics();
}

}