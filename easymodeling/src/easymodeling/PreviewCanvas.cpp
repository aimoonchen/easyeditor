#include "PreviewCanvas.h"
#include "PreviewPanel.h"
#include "Context.h"

#include <ee/DragPhysicsOP.h>
#include <ee/EditPanelImpl.h>
#include <ee/physics_const.h>

#include <sprite2/RVG.h>

namespace emodeling
{

PreviewCanvas::PreviewCanvas(PreviewPanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage_panel(editPanel)
{
	SetBgColor(s2::Color(0, 0, 0));
}

PreviewCanvas::~PreviewCanvas()
{
}

void PreviewCanvas::OnDrawSprites() const
{
 	m_stage_panel->drawPhysics();
 
 	ee::DragPhysicsOP* op = static_cast<ee::DragPhysicsOP*>(m_stage->GetEditOP());
 	if (op->m_mouseJoint)
 	{
 		b2Vec2 target = op->m_mouseJoint->GetAnchorB();
 		sm::vec2 first(target.x * ee::BOX2D_SCALE_FACTOR, target.y * ee::BOX2D_SCALE_FACTOR);
 
 		if (op->m_curr_pos.IsValid())
 		{
			s2::RVG::SetColor(s2::Color(255, 255, 255));
			s2::RVG::LineWidth(1);
 			s2::RVG::Line(first, op->m_curr_pos);
			s2::RVG::LineWidth(2);

			s2::RVG::SetColor(s2::Color(0, 255, 0));
 			s2::RVG::Circle(op->m_curr_pos, 2, true);
 		}
		s2::RVG::SetColor(s2::Color(0, 255, 0));
 		s2::RVG::Circle(first, 2, true);
 	}
}

}