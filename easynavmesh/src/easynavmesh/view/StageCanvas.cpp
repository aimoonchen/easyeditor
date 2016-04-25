#include "StageCanvas.h"
#include "StagePanel.h"
#include "dataset/Mesh.h"

#include <ee/SpriteRenderer.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/EE_RVG.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/DrawShapesVisitor.h>

namespace enav
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	ee::RVG::Color(ee::Colorf(0.8f, 0.8f, 0.8f));
	ee::RVG::LineWidth(2);
	const ee::Rect& r = m_stage->GetRegion();
	ee::RVG::Rect(ee::Vector(r.xmin, r.ymin), ee::Vector(r.xmax, r.ymax), false);

	m_stage->TraverseSprites(ee::DrawSpritesVisitor(r, 1), ee::DT_VISIBLE);
	m_stage->TraverseShapes(ee::DrawShapesVisitor(r), ee::DT_VISIBLE);

	m_stage->GetMesh()->Draw();

	m_stage->DrawEditOP();
}

}