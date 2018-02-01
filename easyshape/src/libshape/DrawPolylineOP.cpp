#include "DrawPolylineOP.h"
#include "DrawLineUtility.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>

#include <painting2/Color.h>
#include <sprite2/RVG.h>

namespace eshape
{

DrawPolylineOP::DrawPolylineOP(wxWindow* wnd, ee::EditPanelImpl* stage, bool isClosed)
	: ee::ZoomViewOP(wnd, stage, true, false)
{
	m_curr_pos.MakeInvalid();

	m_cursor = wxCursor(wxCURSOR_RIGHT_ARROW);

	m_is_closed = isClosed;
}

bool DrawPolylineOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_polyline.back(), pos);
	}
	m_polyline.push_back(pos);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawPolylineOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) return true;

	if (!m_polyline.empty())
	{
		m_polyline.pop_back();
		if (m_polyline.empty()) {
			m_curr_pos.MakeInvalid();
		}
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool DrawPolylineOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y)) return true;

	if (m_polyline.empty()) return false;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_polyline.back(), pos);
	}
	m_curr_pos = pos;
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawPolylineOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDClick(x, y)) return true;

	if (m_is_closed)
		m_polyline.push_back(m_polyline.front());
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawPolylineOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (!m_polyline.empty())
	{
		s2::RVG::SetColor(pt2::Color(0, 0, 0));
		if (m_curr_pos.IsValid())
		{
			m_polyline.push_back(m_curr_pos);
			s2::RVG::Polyline(nullptr, m_polyline, false);
			m_polyline.pop_back();
		}
		else
		{
			s2::RVG::Polyline(nullptr, m_polyline, false);
		}
	}

	return false;
}

bool DrawPolylineOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_polyline.clear();
	m_curr_pos.MakeInvalid();

	return false;
}

bool DrawPolylineOP::ShouldFixPos() const
{
	return DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState());
}

}