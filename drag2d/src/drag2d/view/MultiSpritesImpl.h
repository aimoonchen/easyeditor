#ifndef _DRAG2D_MULTI_SPRITES_IMPL_H_
#define _DRAG2D_MULTI_SPRITES_IMPL_H_

#include "DataTraverseType.h"

#include "common/visitors.h"
#include "message/Observer.h"

#include <wx/wx.h>

namespace d2d
{

class EditPanelImpl;
class SpriteSelection;
class Rect;

class MultiSpritesImpl : public Observer
{
public:
	MultiSpritesImpl(EditPanelImpl* stage);
	virtual ~MultiSpritesImpl();

	virtual void TraverseSprites(IVisitor& visitor, 
		DataTraverseType type = DT_ALL, bool order = true) const = 0;

	virtual ISprite* QuerySpriteByPos(const Vector& pos) const;
	virtual void QuerySpritesByRect(const Rect& rect, bool contain, std::vector<ISprite*>& result) const;		

	SpriteSelection* GetSpriteSelection() { return m_sprite_selection; }
	void ClearSelectedSprite();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void OnSpriteSelected(ISprite* spr, bool clear);

protected:
	SpriteSelection* m_sprite_selection;

}; // MultiSpritesImpl

}

#endif // _DRAG2D_MULTI_SPRITES_IMPL_H_