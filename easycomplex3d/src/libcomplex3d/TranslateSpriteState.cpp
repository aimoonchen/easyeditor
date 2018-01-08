#include "TranslateSpriteState.h"
#include "StagePanel.h"

#include <ee/SpriteSelection.h>
#include <ee/panel_msg.h>

#include <sprite2/ModelSprite.h>

namespace ecomplex3d
{

TranslateSpriteState::TranslateSpriteState(StagePanel* stage,
										   const ee::SpriteSelection& selection)
	: m_stage(stage)
	, m_selection(selection)
{
}

void TranslateSpriteState::OnMousePress(const sm::ivec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void TranslateSpriteState::OnMouseRelease(const sm::ivec2& pos)
{
	// todo history
}

void TranslateSpriteState::OnMouseMove(const sm::ivec2& pos)
{
	if (m_selection.IsEmpty()) {
		return;
	}

	Translate(m_last_pos, pos);
	m_last_pos = pos;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void TranslateSpriteState::Translate(const sm::ivec2& first, const sm::ivec2& curr)
{
	m_selection.Traverse(Visitor(m_stage, first, curr));
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
Visit(const ee::SprPtr& spr, bool& next)
{
	auto model_spr = std::dynamic_pointer_cast<s2::ModelSprite>(spr);

	const sm::vec3& old = model_spr->GetPos3();
	sm::vec3 last = m_stage->TransPos3ScreenToProject(m_last, old.z);
	sm::vec3 curr = m_stage->TransPos3ScreenToProject(m_curr, old.z);
	model_spr->Translate3(curr - last);

	next = true;
}

}