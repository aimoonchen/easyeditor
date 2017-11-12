#ifndef _EASYSKETCH_ARRANGE_SPRITE_OP_H_
#define _EASYSKETCH_ARRANGE_SPRITE_OP_H_

#include "SelectSpriteOP.h"

namespace esketch
{

class IEditState;

class ArrangeSpriteOP : public SelectSpriteOP
{
public:
	ArrangeSpriteOP(StagePanel* stage);
	virtual ~ArrangeSpriteOP();

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	IEditState* m_state;

	IEditState *m_translate_sprite, *m_rotate_sprite;
	IEditState *m_translate_camera, *m_rotate_camera;

}; // ArrangeSpriteOP

}

#endif // _EASYSKETCH_ARRANGE_SPRITE_OP_H_