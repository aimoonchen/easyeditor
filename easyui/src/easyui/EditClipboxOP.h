#ifndef _EASYUI_EDIT_CLIPBOX_OP_H_
#define _EASYUI_EDIT_CLIPBOX_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Rect.h>

namespace eui
{

class EditClipboxOP : public ee::ZoomViewOP
{
public:
	EditClipboxOP(wxWindow* wnd, ee::EditPanelImpl* edit_impl, 
		sm::rect& rect);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;

private:
	static const int NODE_RADIUS = 10;

private:
	sm::rect& m_rect;

	// 1 2
	// 0 3
	int m_clipbox_selected;

}; // EditClipboxOP

}

#endif // _EASYUI_EDIT_CLIPBOX_OP_H_