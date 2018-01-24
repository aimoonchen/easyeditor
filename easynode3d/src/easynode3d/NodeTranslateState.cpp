#include "NodeTranslateState.h"
#include "StageCanvas.h"
#include "NodeSelection.h"

#include <ee/panel_msg.h>

#include <sprite2/ModelSprite.h>

namespace enode3d
{

NodeTranslateState::NodeTranslateState(StageCanvas& canvas, const NodeSelection& selection)
	: m_canvas(canvas)
	, m_selection(selection)
{
}

void NodeTranslateState::OnMousePress(const sm::vec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void NodeTranslateState::OnMouseRelease(const sm::vec2& pos)
{
	// todo history
}

void NodeTranslateState::OnMouseDrag(const sm::vec2& pos)
{
	if (m_selection.IsEmpty()) {
		return;
	}

	Translate(m_last_pos, pos);
	m_last_pos = pos;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void NodeTranslateState::Translate(const sm::vec2& first, const sm::vec2& curr)
{
	m_selection.Traverse(Visitor(m_canvas, first, curr));
}

//////////////////////////////////////////////////////////////////////////
// class NodeTranslateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeTranslateState::Visitor::
Visit(const n3::NodePtr& node, bool& next)
{
	auto& cam = m_canvas.GetCamera();
	float dist = cam.GetToward().Dot(node->GetPos() - cam.GetPos());

	const sm::vec3& old = node->GetPos();
	sm::vec3 last = m_canvas.TransPos3ScreenToDir(m_last).Normalized() * dist;
	sm::vec3 curr = m_canvas.TransPos3ScreenToDir(m_curr).Normalized() * dist;
	node->Translate(curr - last);

	next = true;
}

}