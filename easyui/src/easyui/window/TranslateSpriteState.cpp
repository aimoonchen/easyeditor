#include "TranslateSpriteState.h"
#include "StagePanel.h"

#include <ee/SpriteSelection.h>
#include <ee/panel_msg.h>

#include <easyui.h>

namespace eui
{
namespace window
{

TranslateSpriteState::TranslateSpriteState(StagePanel* stage,
										   ee::SpriteSelection* selection, 
										   const sm::vec2& first_pos, 
										   AnchorMgr* anchor_mgr)
	: ee::TranslateSpriteState(selection, first_pos)
	, m_stage(stage)
	, m_anchor_mgr(anchor_mgr)
{
}

void TranslateSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	GetSelection()->Traverse(Visitor(m_anchor_mgr));
	ee::TranslateSpriteState::OnMouseRelease(pos);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
Visit(const ee::SprPtr& spr, bool& next)
{
	if (dynamic_cast<Sprite*>(spr) != NULL) {
		spr->SetPosition(sm::vec2(0, 0));
		spr->SetAngle(0);
	} else {
		m_anchor_mgr->OnSprPosChanged(spr);
	}
	next = true;
}

}
}