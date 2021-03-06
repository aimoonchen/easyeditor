#ifndef _EASYEDITOR_PASTE_SPRITE_OP_H_
#define _EASYEDITOR_PASTE_SPRITE_OP_H_

#include "SelectSpritesOP.h"
#include "Sprite.h"

#include <vector>

namespace ee
{

class MultiSpritesImpl;
class PasteSpriteCMPT;

class PasteSpriteOP : public SelectSpritesOP
{
public:
	PasteSpriteOP(wxWindow* wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* sprites_impl, PasteSpriteCMPT* cmpt = NULL);
	virtual ~PasteSpriteOP();

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

private:
	void setMousePos(int x, int y);
	void fixPosOrthogonal();

private:
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void LoadFromSelection(const SpriteSelection& selection);

		void InsertToSpritesImpl(const sm::vec2& pos, bool isHorMirror, bool isVerMirror);
		void Draw(const sm::vec2& pos, bool isHorMirror, bool isVerMirror) const;

		const sm::vec2& GetCenter() const { return m_center; }
		bool IsCenterValid() const { return m_valid; }

		void Clear();

		bool Empty() const { return m_selected.empty(); }

	private:
		void ComputeCenter();

	private:
		std::vector<SprPtr> m_selected;
		sm::vec2 m_center;
		bool m_valid;

	}; // SpriteBatch

private:
	PasteSpriteCMPT* m_cmpt;

	SpriteSelection* m_selection;

	sm::vec2 m_pos;

	SpriteBatch m_batch;

}; // PasteSpriteOP

}

#endif // _EASYEDITOR_PASTE_SPRITE_OP_H_