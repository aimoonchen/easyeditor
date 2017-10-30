#ifndef _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_
#define _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Visitor.h"
#include "Sprite.h"

namespace ee
{

class SpriteSelection;

class TranslateSpriteState : public ArrangeSpriteState
{
public:
	TranslateSpriteState(SpriteSelection* selection, const sm::vec2& first_pos);
	virtual ~TranslateSpriteState();

	virtual	void OnMousePress(const sm::vec2& pos);
	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

	virtual bool OnDirectionKeyDown(int type);

protected:
	virtual void Translate(const sm::vec2& offset);

protected:
	SpriteSelection* GetSelection() { return m_selection; } 

private:
	class TranslateVisitor : public RefVisitor<Sprite>
	{
	public:
		TranslateVisitor(const sm::vec2& offset) : m_offset(offset) {}
		virtual void Visit(const SprPtr& spr, bool& next);
	private:
		sm::vec2 m_offset;
	}; // TranslateVisitor

private:
	SpriteSelection* m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	bool m_dirty;
	
}; // TranslateState

}

#endif // _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_