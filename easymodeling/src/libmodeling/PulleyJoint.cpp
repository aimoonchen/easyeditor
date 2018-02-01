#include "PulleyJoint.h"
#include "Body.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>

#include <sprite2/RVG.h>

namespace emodeling
{

PulleyJoint::PulleyJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_pulleyJoint)
	, m_ratio(1.0f)
{
	m_ground_anchor_a = b0->m_spr->GetPosition();
	m_ground_anchor_b = b1->m_spr->GetPosition();
	m_ground_anchor_a.y += 10;
	m_ground_anchor_b.y += 10;

	sm::vec2 center = (b0->m_spr->GetPosition() + b1->m_spr->GetPosition()) * 0.5f;
	SetLocalAnchorA(center);
	SetLocalAnchorB(center);
}

bool PulleyJoint::IsContain(const sm::vec2& pos) const
{
	return sm::dis_pos_to_pos(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| sm::dis_pos_to_pos(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT
		|| sm::dis_pos_to_pos(m_ground_anchor_a, pos) < JOINT_RADIUS_OUT
		|| sm::dis_pos_to_pos(m_ground_anchor_b, pos) < JOINT_RADIUS_OUT;
}

bool PulleyJoint::IsIntersect(const sm::rect& rect) const
{
	return sm::is_point_in_rect(GetWorldAnchorA(), rect) 
		|| sm::is_point_in_rect(GetWorldAnchorB(), rect)
		|| sm::is_point_in_rect(m_ground_anchor_a, rect) 
		|| sm::is_point_in_rect(m_ground_anchor_b, rect);
}

void PulleyJoint::Draw(DrawType type) const
{
	const sm::vec2 anchorA = GetWorldAnchorA(),
		anchorB = GetWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		s2::RVG::SetColor(pt2::Color(255, 0, 0));
		s2::RVG::DashLine(anchorA, anchorB);
		s2::RVG::SetColor(pt2::Color(102, 204, 102));
		s2::RVG::DashLine(anchorA, m_body_a->m_spr->GetPosition());
		s2::RVG::SetColor(pt2::Color(102, 102, 204));
		s2::RVG::DashLine(anchorB, m_body_b->m_spr->GetPosition());

		s2::RVG::SetColor(pt2::Color(204, 204, 102));
		s2::RVG::Line(anchorA, m_ground_anchor_a);
		s2::RVG::Line(anchorB, m_ground_anchor_b);
		s2::RVG::Line(m_ground_anchor_a, m_ground_anchor_b);

		DrawBodyFlag();
	}

	DrawAnchor(anchorA, type);
	DrawAnchor(anchorB, type);
	DrawAnchor(m_ground_anchor_a, type);
	DrawAnchor(m_ground_anchor_b, type);
}


sm::vec2 PulleyJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_spr);
}

sm::vec2 PulleyJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_spr);
}

void PulleyJoint::SetLocalAnchorA(const sm::vec2& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_spr);
}

void PulleyJoint::SetLocalAnchorB(const sm::vec2& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_spr);
}

void PulleyJoint::DrawAnchor(const sm::vec2& pos, DrawType type) const
{
	pt2::Color color;
	switch (type)
	{
	case e_default:
		color.FromFloat(0.8f, 0.8f, 0.8f);
		break;
	case e_mouseOn:
		color.FromFloat(1, 1, 1);
		break;
	case e_selected:
		color.FromFloat(1, 1, 0);
		break;
	}

	s2::RVG::SetColor(color);
	s2::RVG::Circle(pos, JOINT_RADIUS_IN, true);
	s2::RVG::Circle(pos, JOINT_RADIUS_OUT, false);
}

}