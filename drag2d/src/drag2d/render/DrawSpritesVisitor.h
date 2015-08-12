#ifndef _DRAG2D_DRAW_SPRITES_VISITOR_H_
#define _DRAG2D_DRAW_SPRITES_VISITOR_H_

#include "common/visitors.h"
#include "common/Rect.h"

namespace d2d
{

class SpriteRenderer;

class DrawSpritesVisitor : public IVisitor
{
public:
	DrawSpritesVisitor(const Rect& screen_region, float cam_scale) 
		: m_screen_region(screen_region), m_cam_scale(cam_scale) {}
	virtual void Visit(Object* object, bool& bFetchNext);

protected:
	virtual void DrawSprite(SpriteRenderer* rd, ISprite* spr) const;

private:
	Rect m_screen_region;

	float m_cam_scale;

}; // DrawSpritesVisitor

}

#endif // _DRAG2D_DRAW_SPRITES_VISITOR_H_