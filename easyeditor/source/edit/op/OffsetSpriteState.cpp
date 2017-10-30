#include "OffsetSpriteState.h"
#include "Sprite.h"
#include "OffsetSpriteAOP.h"
#include "panel_msg.h"
#include "Math2D.h"
#include "SpriteCtrlNode.h"
#include "ArrangeSpriteImpl.h"

#include <SM_Calc.h>
#include <sprite2/OrthoCamera.h>

namespace ee
{

OffsetSpriteState::OffsetSpriteState(const SprPtr& spr, const s2::Camera* camera)
	: m_spr(spr)
	, m_camera(camera)
{
	m_old_offset = m_spr->GetOffset();
}

void OffsetSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	float s = 1;
	if (m_camera->Type() == s2::CAM_ORTHO2D) {
		s = static_cast<const s2::OrthoCamera*>(m_camera)->GetScale();
	}

	float r = ArrangeSpriteImpl::CTRL_NODE_RADIUS * s * 2;

	sm::vec2 ctrl_nodes[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(*m_spr, ctrl_nodes);
	sm::vec2 fixed = pos;
	if (sm::dis_pos_to_pos(fixed, m_spr->GetPosition()) < r) {
		fixed = m_spr->GetPosition();
	} else {
		for (int i = 0; i < 8; ++i) {
			if (sm::dis_pos_to_pos(fixed, ctrl_nodes[i]) < r) {
				fixed = ctrl_nodes[i];
				break;
			}
		}
	}

	sm::vec2 new_offset = sm::rotate_vector(fixed - m_spr->GetCenter(), -m_spr->GetAngle());
	m_spr->SetOffset(new_offset);
	AtomicOP* aop = new OffsetSpriteAOP(m_spr, new_offset, m_old_offset);
	EditAddRecordSJ::Instance()->Add(aop);
}

bool OffsetSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	sm::vec2 offset = sm::rotate_vector(pos - m_spr->GetCenter(), -m_spr->GetAngle());
	m_spr->SetOffset(offset);
	return true;
}

}