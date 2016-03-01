#include "SelectShapesOP.h"
#include "Rect.h"
#include "MultiShapesImpl.h"
#include "ShapeSelection.h"
#include "EditPanelImpl.h"
#include "EditCMPT.h"
#include "DrawSelectedShapeVisitor.h"
#include "shape_msg.h"
#include "panel_msg.h"
#include "FetchAllVisitor.h"

namespace ee
{

SelectShapesOP::SelectShapesOP(wxWindow* wnd, EditPanelImpl* stage, MultiShapesImpl* shapes_impl,
							   EditCMPT* callback/* = NULL*/)
	: DrawRectangleOP(wnd, stage)
	, m_callback(callback)
	, m_shape_impl(shapes_impl)
	, m_bDraggable(true)
{
	m_selection = shapes_impl->GetShapeSelection();
	m_selection->Retain();

	m_first_pos.SetInvalid();
}

SelectShapesOP::~SelectShapesOP()
{
	clearClipboard();

 	m_selection->Clear();
 	m_selection->Release();
}

bool SelectShapesOP::OnKeyDown(int keyCode)
{
	if (DrawRectangleOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shape_impl->ClearSelectedShape();
		Clear();
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'x' || keyCode == 'X'))
	{
		PasteToSelection();
		m_shape_impl->ClearSelectedShape();
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'c' || keyCode == 'C'))
	{
		clearClipboard();

		std::vector<Shape*> shapes;
		m_selection->Traverse(FetchAllVisitor<Shape>(shapes));
		for (size_t i = 0, n = shapes.size(); i < n; ++i)
			m_clipboard.push_back(shapes[i]->Clone());
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'v' || keyCode == 'V'))
	{
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i) {
			InsertShapeSJ::Instance()->Insert(m_clipboard[i]->Clone());
		}
	}

	return false;
}

bool SelectShapesOP::OnMouseLeftDown(int x, int y)
{
	m_bDraggable = true;

	m_move_last_pos.SetInvalid();

	Vector pos = m_stage->TransPosScrToProj(x, y);
	Shape* selected = m_shape_impl->QueryShapeByPos(pos);
	if (selected)
	{
		if (m_stage->GetKeyState(WXK_CONTROL))
		{
			if (m_selection->IsExist(selected)) {
				m_selection->Remove(selected);
			} else {
				m_selection->Add(selected);
			}
			SelectShapeSetSJ::Instance()->Selecte(m_selection, m_shape_impl);
		}
		else
		{
			if (!m_selection->IsExist(selected))
			{
				m_selection->Clear();
				m_selection->Add(selected);
				SelectShapeSJ::Instance()->Select(selected, m_shape_impl);
			} else {
				m_move_last_pos = pos;
			}
		}
		m_first_pos.SetInvalid();

		if (m_callback)
			m_callback->UpdateControlValue();
	}
	else
	{
		DrawRectangleOP::OnMouseLeftDown(x, y);
		m_first_pos = pos;
		if (m_stage->GetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection->Clear();
	}

	return false;
}

bool SelectShapesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	if (m_first_pos.IsValid())
	{
		Rect rect(m_first_pos, m_stage->TransPosScrToProj(x, y));
		std::vector<Shape*> shapes;
		m_shape_impl->QueryShapesByRect(rect, shapes);
		for (size_t i = 0, n = shapes.size(); i < n; ++i)
			m_selection->Add(shapes[i]);

		SelectShapeSetSJ::Instance()->Selecte(m_selection, m_shape_impl);

		m_first_pos.SetInvalid();

		if (m_callback) {
			m_callback->UpdateControlValue();
		}
	}

	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool SelectShapesOP::OnMouseDrag(int x, int y)
{
	if (DrawRectangleOP::OnMouseDrag(x, y)) return true;

	if (!m_selection->IsEmpty() && m_move_last_pos.IsValid()) {
		Vector pos = m_stage->TransPosScrToProj(x, y);
		m_selection->Traverse(TranslateVisitor(pos - m_move_last_pos));
		m_move_last_pos = pos;
	}

	return !m_bDraggable;
}

bool SelectShapesOP::OnDraw() const
{
	if (DrawRectangleOP::OnDraw()) return true;

	m_selection->Traverse(DrawSelectedShapeVisitor());

	return false;
}

bool SelectShapesOP::Clear()
{
	if (DrawRectangleOP::Clear()) return true;

	clearClipboard();
	m_selection->Clear();
	m_first_pos.SetInvalid();

	return false;
}

void SelectShapesOP::clearClipboard()
{
 	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
 		m_clipboard[i]->Release();
 	m_clipboard.clear();
}

void SelectShapesOP::PasteToSelection() const
{
	std::vector<Shape*> shapes;
	m_selection->Traverse(FetchAllVisitor<Shape>(shapes));
	for (size_t i = 0, n = shapes.size(); i < n; ++i) {
		m_clipboard.push_back(shapes[i]->Clone());
	}
}

void SelectShapesOP::CopyFromSelection()
{
	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i) {
		InsertShapeSJ::Instance()->Insert(m_clipboard[i]->Clone());
	}
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::TranslateVisitor
//////////////////////////////////////////////////////////////////////////

void SelectShapesOP::TranslateVisitor::
Visit(Object* object, bool& next)
{
	Shape* shape = static_cast<Shape*>(object);
	shape->Translate(m_offset);
	next = true;
}

}