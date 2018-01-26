#include "editop/NodeTranslateState.h"
#include "view/StageCanvas.h"
#include "msg/SubjectMgr.h"

namespace eone
{

NodeTranslateState::NodeTranslateState(StageCanvas& canvas, SubjectMgr& sub_mgr,
	                                   const ee::SelectionSet<n3::SceneNode>& selection)
	: m_canvas(canvas)
	, m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void NodeTranslateState::OnMousePress(const sm::vec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void NodeTranslateState::OnMouseRelease(const sm::vec2& pos)
{
	m_sub_mgr.NotifyObservers(MSG_UPDATE_COMPONENTS);
}

void NodeTranslateState::OnMouseDrag(const sm::vec2& pos)
{
	if (m_selection.IsEmpty()) {
		return;
	}

	Translate(m_last_pos, pos);
	m_last_pos = pos;

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void NodeTranslateState::Translate(const sm::vec2& first, const sm::vec2& curr)
{
	m_selection.Traverse(Visitor(m_canvas, first, curr));
}

//////////////////////////////////////////////////////////////////////////
// class NodeTranslateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeTranslateState::Visitor::
Visit(const n3::SceneNodePtr& node, bool& next)
{
	auto& ctrans = node->GetComponent<n3::CompTransform>();

	auto& cam = m_canvas.GetCamera();
	float dist = cam.GetToward().Dot(ctrans.GetPosition() - cam.GetPos());

	sm::vec3 last = m_canvas.TransPos3ScreenToDir(m_last).Normalized() * dist;
	sm::vec3 curr = m_canvas.TransPos3ScreenToDir(m_curr).Normalized() * dist;
	ctrans.Translate(curr - last);

	next = true;
}

}