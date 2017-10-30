#ifndef _EASYEDITOR_SELECT_SHAPES_OP_H_
#define _EASYEDITOR_SELECT_SHAPES_OP_H_

#include "DrawRectangleOP.h"
#include "Visitor.h"
#include "Shape.h"

#include <vector>

namespace ee
{

class MultiShapesImpl;
class EditCMPT;
class IPropertySetting;
class ShapeSelection;

class SelectShapesOP : public DrawRectangleOP
{
public:
	SelectShapesOP(wxWindow* wnd, EditPanelImpl* stage, MultiShapesImpl* shapes_impl, 
		EditCMPT* callback = NULL);
	virtual ~SelectShapesOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	void clearClipboard();

	void PasteToSelection() const;
	void CopyFromSelection();

protected:
	sm::vec2 m_first_pos;

	ShapeSelection* m_selection;

private:
	class TranslateVisitor : public Visitor<Shape>
	{
	public:
		TranslateVisitor(const sm::vec2& offset) : m_offset(offset) {}
		virtual void Visit(Shape* shape, bool& next);
	private:
		sm::vec2 m_offset;
	}; // TranslateVisitor

private:
	EditCMPT* m_callback;

	MultiShapesImpl* m_shape_impl;

	mutable std::vector<ShapePtr> m_clipboard;

	// To disable mouse able when press ctrl and window query
	bool m_bDraggable;

	sm::vec2 m_move_last_pos;

}; // SelectShapesOP

}

#endif // _EASYEDITOR_SELECT_SHAPES_OP_H_