#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>

#include <sprite2/Skeleton.h>
#include <sprite2/CameraType.h>

namespace libskeleton
{

StageCanvas::StageCanvas(StagePanel* editpanel, wxGLContext* glctx)
	: ee::CameraCanvas(editpanel, editpanel->GetStageImpl(), s2::CAM_ORTHO2D, glctx)
	, m_sk(editpanel->GetSkeleton())
{
}

void StageCanvas::OnDrawSprites() const
{
	m_sk.Draw(nullptr, s2::RenderParams());

	m_stage->DrawEditOP();
}

}