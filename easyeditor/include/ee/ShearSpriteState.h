#ifndef _EASYEDITOR_SHEAR_SPRITE_STATE_H_
#define _EASYEDITOR_SHEAR_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace ee
{

class ShearSpriteState : public ArrangeSpriteState
{
public:
	ShearSpriteState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node);
	virtual ~ShearSpriteState();

	virtual void OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

private:
	void Shear(const Vector& curr);
	void Shear2(const Vector& curr);

private:
	Sprite* m_sprite;

	SpriteCtrlNode::Node m_ctrl_node;

	Vector m_first_shear;

}; // ShearSpriteState

}

#endif // _EASYEDITOR_SHEAR_SPRITE_STATE_H_