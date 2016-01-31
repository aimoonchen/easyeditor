#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"

using namespace edb;

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::OrthoCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
	, m_batch(100, ee::SpriteBatch::USAGE_STATIC)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	m_stage_panel->TraverseShapes(ee::DrawShapesVisitor(ee::Rect()), ee::DT_VISIBLE);

	// fixme
//	editPanel->traverseSprites(ee::DrawSpritesVisitor(m_batch), ee::DT_VISIBLE);

	drawConnection();
	m_stage_panel->DrawEditOP();
}

void StageCanvas::drawConnection() const
{
	std::map<ee::Sprite*, Node*>& connection
		= Context::Instance()->stage->m_graphics.connection;
	std::map<ee::Sprite*, Node*>::iterator itr = connection.begin();
	for ( ; itr != connection.end(); ++itr)
	{
		ee::Sprite* from = itr->first;
		for (size_t i = 0, n = itr->second->out.size(); i < n; ++i)
		{
			ee::Sprite* to = itr->second->out[i];
			ee::PrimitiveDraw::DrawLine(from->GetPosition(), to->GetPosition(), 
				ee::Colorf(0.8f, 0.8f, 0.8f));
		}
	}
}