#ifndef _LIBSHAPE_EDIT_CIRCLE_OP_H_
#define _LIBSHAPE_EDIT_CIRCLE_OP_H_

#include <drag2d.h>

#include "NodeCapture.h"

namespace libshape
{

class INodeCapture;

class EditCircleOP : public d2d::ZoomViewOP
{
public:
	EditCircleOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel, INodeCapture* node_capture);

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw(const d2d::Screen& scr) const;
	virtual bool clear();

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::PropertySettingPanel* m_propertyPanel;

	INodeCapture* m_node_capture;

	d2d::Vector m_firstPress;
	d2d::Vector m_currPos;

	NodeAddr m_captured;

}; // EditCircleOP

}

#endif // _LIBSHAPE_EDIT_CIRCLE_OP_H_