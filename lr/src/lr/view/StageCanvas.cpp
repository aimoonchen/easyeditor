#include "StageCanvas.h"
#include "StagePanel.h"

#include "dataset/Grids.h"
#include "frame/config.h"
#include "frame/SettingCfg.h"

namespace lr
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* statge)
	: d2d::OrthoCanvas(statge)
	, m_statge(statge)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);
}

void StageCanvas::onDraw()
{
	m_statge->traverseSprites(d2d::DrawSpritesVisitor(), d2d::DT_VISIBLE);
	m_statge->traverseShapes(d2d::DrawShapesVisitor(), d2d::DT_VISIBLE);

	DrawRegion();

	if (const Grids* grids = m_statge->GetGrids()) {
		grids->Draw();
	}

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::DrawRegion() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		cfg->m_map_width * 0.5f,
		cfg->m_map_height * 0.5f,
		d2d::LIGHT_GREY_LINE);

	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		cfg->m_view_width * 0.5f,
		cfg->m_view_height * 0.5f,
		d2d::LIGHT_RED_LINE);

	m_statge->DebugDraw();
}

}