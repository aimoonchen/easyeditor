#ifndef _EASYSKELETON_JOINT_H_
#define _EASYSKELETON_JOINT_H_

#include <easyskeleton.h>

namespace eskeleton
{

class Joint : public libskeleton::Joint
{
public:
	Joint(const ee::SprPtr& spr, const s2::JointPose& joint_pose);

	virtual void Translate(const sm::vec2& trans);
	virtual void Rotate(float rot);
	virtual void Scale(const sm::vec2& scale);

	void UpdateToJoint();
	void UpdateToSkin();
	
	void BindSkin(const sm::vec2& world_pos, bool static_skin);

	void Clear();

private:
	void UpdateChildren();

}; // Joint

}

#endif // _EASYSKELETON_JOINT_H_