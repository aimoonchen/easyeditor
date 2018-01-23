#include "ScaleSpriteState.h"
#include "Symbol.h"
#include "Sprite.h"
#include "CombineAOP.h"
#include "TranslateSpriteAOP.h"
#include "ScaleSpriteAOP.h"
#include "panel_msg.h"
#include "Math2D.h"
#include "EditSprMsg.h"

#include <SM_Calc.h>

namespace ee
{

ScaleSpriteState::ScaleSpriteState(const SprPtr& spr, const SpriteCtrlNode::Node& ctrl_node)
	: m_spr(spr)
	, m_ctrl_node(ctrl_node)
{
	m_first_pos = m_spr->GetPosition();
	m_first_scale = m_spr->GetScale();
}

void ScaleSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	auto comb = std::make_shared<CombineAOP>();

	comb->Insert(std::make_shared<TranslateSpriteAOP>(m_spr, m_spr->GetPosition() - m_first_pos));
	comb->Insert(std::make_shared<ScaleSpriteAOP>(m_spr, m_spr->GetScale(), m_first_scale));

	EditAddRecordSJ::Instance()->Add(comb);

	EditSprMsg::SetScale(m_spr.get(), m_spr->GetPosition(), m_spr->GetScale());
}

bool ScaleSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	Scale(pos);
	return true;
}

void ScaleSpriteState::Scale(const sm::vec2& curr)
{
	if (!m_spr) {
		return;
	}

	sm::vec2 ctrls[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(*m_spr, ctrls);
	
	sm::vec2 ori = ctrls[m_ctrl_node.type];
	sm::vec2 center = m_spr->GetPosition() + m_spr->GetOffset();
	sm::vec2 fix;
	sm::get_foot_of_perpendicular(center, ori, curr, &fix);

	float scale_times = sm::dis_pos_to_pos(center, fix) / sm::dis_pos_to_pos(center, ori);
	if (fabs(scale_times - 1) < FLT_EPSILON) {
		return;
	}

	sm::vec2 st(1, 1);
	if (m_ctrl_node.type == SpriteCtrlNode::UP || m_ctrl_node.type == SpriteCtrlNode::DOWN) {
		st.y = scale_times;
	} else if (m_ctrl_node.type == SpriteCtrlNode::LEFT || m_ctrl_node.type == SpriteCtrlNode::RIGHT) {
		st.x = scale_times;
	} else {
		st.Set(scale_times, scale_times);
	}
	SetScaleTimes(st);
}

void ScaleSpriteState::SetScaleTimes(const sm::vec2& st)
{
	sm::vec2 scale = m_spr->GetScale();
	scale *= st;
	m_spr->SetScale(scale);
}

}