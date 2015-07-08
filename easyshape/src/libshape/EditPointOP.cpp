#include "EditPointOP.h"
#include "PointShape.h"
#include "ShapeType.h"

namespace libshape
{

EditPointOP::EditPointOP(d2d::EditPanel* editpanel,
						 d2d::MultiShapesImpl* shapes_impl,
						 d2d::OneFloatValue* node_capture)
	: d2d::ZoomViewOP(editpanel, true)
	, m_shapes_impl(shapes_impl)
	, m_node_capture(node_capture)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	Clear();
}

bool EditPointOP::OnKeyDown(int keyCode)
{
	if (d2d::ZoomViewOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapes_impl->ClearShapeSelection();
		m_captured.clear();
		m_stage->RefreshStage();
	}

	return false;
}

bool EditPointOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_pos = m_stage->TransPosScrToProj(x, y);

	m_shapes_impl->GetShapeSelection()->Clear();
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_pos, m_captured);
		if (m_captured.shape &&
			get_shape_type(m_captured.shape->GetShapeDesc()) == ST_POINT) {
			m_shapes_impl->GetShapeSelection()->Add(m_captured.shape);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditPointOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_pos.isValid()) {
		return false;
	}

	m_pos = m_stage->TransPosScrToProj(x, y);
	if (!m_captured.shape) {
		PointShape* point = new PointShape(m_pos);
		m_shapes_impl->GetShapeSelection()->Add(point);
		m_shapes_impl->InsertShape(point);
	}

	Clear();

	m_stage->RefreshStage();

	return false;
}

bool EditPointOP::OnMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseRightDown(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance == 0) {
		return false;
	}

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	NodeCapture capture(m_shapes_impl, tolerance);
	capture.captureEditable(pos, m_captured);
	if (m_captured.shape) {
		m_shapes_impl->RemoveShape(m_captured.shape);
		m_shapes_impl->GetShapeSelection()->Clear();
		m_captured.clear();
		m_stage->RefreshStage();
	}

	return false;
}

bool EditPointOP::OnMouseMove(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseMove(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance == 0) {
		return false;
	}

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	NodeCapture capture(m_shapes_impl, tolerance);
	d2d::IShape* old = m_captured.shape;
	capture.captureEditable(pos, m_captured);
	if (old && !m_captured.shape || !old && m_captured.shape)
		m_stage->RefreshStage();

	return false;
}

bool EditPointOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_pos = m_stage->TransPosScrToProj(x, y);
	if (m_captured.shape && 
		get_shape_type(m_captured.shape->GetShapeDesc()) == ST_POINT) {		
		PointShape* point = static_cast<PointShape*>(m_captured.shape);
		point->SetPos(m_pos);
		m_stage->RefreshStage();
	}

	return false;
}

bool EditPointOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	if (m_pos.isValid()) {
		d2d::PrimitiveDraw::drawCircle(m_pos, m_node_capture->GetValue(), true, 2, d2d::LIGHT_RED);
	}

	return false;
}

bool EditPointOP::Clear()
{
	if (d2d::ZoomViewOP::Clear()) return true;

	m_pos.setInvalid();

	return false;
}

}