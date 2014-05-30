#pragma once

#include <vector>

#include "DrawRectangleOP.h"

#include "dataset/SelectionSet.h"
#include "render/ShapeStyle.h"

namespace d2d
{
	class MultiSpritesImpl;
	class AbstractEditCMPT;
	class PropertySettingPanel;
	class IPropertySetting;

	class SelectSpritesOP : public DrawRectangleOP
	{
	public:
		SelectSpritesOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			PropertySettingPanel* propertyPanel = NULL, AbstractEditCMPT* callback = NULL);
		virtual ~SelectSpritesOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseRightUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		virtual IPropertySetting* createPropertySetting(ISprite* sprite) const;
		virtual IPropertySetting* createPropertySetting(const std::vector<ISprite*>& sprites) const;

	private:
		void pasteToSelection() const;
		void copyFromSelection();

		ISprite* selectByPos(const Vector& pos) const;

	protected:
		SpriteSelection* m_selection;

		PropertySettingPanel* m_propertyPanel;

		MultiSpritesImpl* m_spritesImpl;

		AbstractEditCMPT* m_callback;

	private:
		void clearClipboard();

	private:
		Vector m_firstPos;
		Vector m_rightFirstScrPos;

		// To disable mouse able when press ctrl and window query
		bool m_bDraggable;

	}; // SelectSpritesOP
}

