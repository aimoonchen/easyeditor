#include "Joint.h"
#include "Bone.h"

#include <ee/Sprite.h>

namespace eskeleton
{

Joint::Joint(const ee::SprPtr& spr, const s2::JointPose& joint_pose)
	: libskeleton::Joint(spr, joint_pose)
{
}

void Joint::Translate(const sm::vec2& trans)
{
	m_world_pose.trans += trans;
	if (m_parent) {
		m_local_pose = s2::world2local(m_parent->GetWorldPose(), m_world_pose);
	}
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		const s2::Sprite* spr = m_children[i]->GetSkinSpr();
		Bone* cbone = (Bone*)(std::dynamic_pointer_cast<const ee::Sprite>(spr)->GetUserData());
		cbone->Translate(trans);
	}
}

void Joint::Rotate(float rot)
{
	m_local_pose.rot += rot;
	m_world_pose.rot += rot;
}

void Joint::Scale(const sm::vec2& scale)
{
	m_local_pose.scale *= scale;
	m_world_pose.scale *= scale;
}

void Joint::UpdateToJoint()
{
	m_world_pose = s2::local2world(m_parent->GetWorldPose(), m_local_pose);
	m_skin.Update(this);
	UpdateChildren();
}

void Joint::UpdateToSkin()
{
	const s2::Sprite* spr = m_skin.spr;
	s2::JointPose src(spr->GetCenter(), spr->GetAngle(), spr->GetScale());
	m_world_pose = s2::local2world(src, -m_skin.skin_local);
	if (m_parent) {
		m_local_pose = s2::world2local(m_parent->GetWorldPose(), m_world_pose);
	}
	UpdateChildren();
}

void Joint::BindSkin(const sm::vec2& world_pos, bool static_skin)
{
	if (static_skin) {
		m_skin.spr->Translate(world_pos - m_world_pose.trans);
		m_world_pose.trans = world_pos;
	} else {
		m_world_pose.trans = world_pos;
		const s2::Sprite* spr = m_skin.spr;
		s2::JointPose src(spr->GetCenter(), spr->GetAngle(), spr->GetScale());
		m_skin.skin_local = -s2::world2local(src, m_world_pose);
	}
}

void Joint::Clear()
{
	DeconnectParent();
	while (!m_children.empty()) {
		m_children[0]->DeconnectParent();
	}
	if (m_skin.spr) {
		m_skin.spr = NULL;
	}
}

void Joint::UpdateChildren()
{
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		Joint* c = dynamic_cast<Joint*>(m_children[i]);
		c->UpdateToJoint();
		const ee::SprConstPtr& spr = std::dynamic_pointer_cast<const ee::Sprite>(c->GetSkinSpr());
		Bone* cbone = (Bone*)(spr->GetUserData());
		cbone->Update();
	}
}

}