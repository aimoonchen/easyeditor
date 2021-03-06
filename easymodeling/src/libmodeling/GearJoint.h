#ifndef _EASYMODELING_GEAR_JOINT_H_
#define _EASYMODELING_GEAR_JOINT_H_

#include "Joint.h"

namespace emodeling
{

class GearJoint : public Joint
{
public:
	GearJoint(Body* b0, Body* b1, Joint* j1, Joint* j2);

	virtual bool IsContain(const sm::vec2& pos) const override;
	virtual bool IsIntersect(const sm::rect& rect) const override;

	virtual void Draw(DrawType type) const override;

public:
	Joint* m_joint1;
	Joint* m_joint2;

	float m_ratio;

}; // GearJoint

}

#endif // _EASYMODELING_GEAR_JOINT_H_
