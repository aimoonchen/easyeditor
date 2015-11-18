#ifndef _EASYUI_EDIT_CLIPBOX_OP_H_
#define _EASYUI_EDIT_CLIPBOX_OP_H_

#include <drag2d.h>

namespace eui
{

class EditClipboxOP : public d2d::ZoomViewOP
{
public:
	EditClipboxOP(wxWindow* wnd, d2d::EditPanelImpl* edit_impl, 
		d2d::Rect& rect);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	static const int NODE_RADIUS = 10;

private:
	d2d::Rect& m_rect;

	// 1 2
	// 0 3
	int m_clipbox_selected;

}; // EditClipboxOP

}

#endif // _EASYUI_EDIT_CLIPBOX_OP_H_