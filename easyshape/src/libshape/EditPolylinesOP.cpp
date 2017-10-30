#include "EditPolylinesOP.h"
#include "EditPolylinesCMPT.h"
#include "ChainShape.h"

#include <ee/EditPanelImpl.h>
#include <ee/ShapeSelection.h>
#include <ee/panel_msg.h>
#include <ee/SettingData.h>

#include <sprite2/RVG.h>
#include <sprite2/RenderParams.h>
#include <SM_DouglasPeucker.h>

namespace eshape
{

EditPolylinesOP::EditPolylinesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
								 ee::MultiShapesImpl* shapes_impl,
								 EditPolylinesCMPT* cmpt)
	: ee::SelectShapesOP(wnd, stage, shapes_impl, cmpt)
	, m_cmpt(cmpt)
	, m_is_dirty(false)
{
	m_last_pos.MakeInvalid();

	clearBuffer();
}

bool EditPolylinesOP::OnMouseLeftDown(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseLeftDown(x, y)) return true;

	if (!m_last_pos.IsValid()) {
		m_last_pos = m_stage->TransPosScrToProj(x, y);
	} else {
		m_last_pos.MakeInvalid();
	}

	return false;
}

bool EditPolylinesOP::OnMouseLeftUp(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseLeftUp(x, y)) return true;

	clearBuffer();
	m_selection->Traverse(UpdateBufferVisitor(m_simplify_buffer));

	return false;
}

bool EditPolylinesOP::OnMouseDrag(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseDrag(x, y)) return true;

	if (m_last_pos.IsValid())
	{
		sm::vec2 curr_pos = m_stage->TransPosScrToProj(x, y);
		sm::vec2 offset = curr_pos - m_last_pos;
		m_selection->Traverse(OffsetVisitor(offset));
		m_last_pos = curr_pos;

		m_is_dirty = true;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditPolylinesOP::OnDraw() const
{
	if (ee::SelectShapesOP::OnDraw()) return true;

	s2::RenderParams rp;
	rp.color.SetMul(s2::Color(0.8f, 0.8f, 0.2f));

	for (auto& item : m_simplify_buffer) {
		item.second->Draw(rp);
		s2::RVG::SetColor(s2::Color(51, 51, 204));
		s2::RVG::Circles(item.second->GetVertices(), ee::SettingData::ctl_pos_sz, true);
	}

	return false;
}

bool EditPolylinesOP::Clear()
{
	if (ee::SelectShapesOP::Clear()) return true;

	clearBuffer();

	return false;
}

void EditPolylinesOP::simplify()
{
	auto itr = m_simplify_buffer.begin();
	for ( ; itr != m_simplify_buffer.end(); ++itr)
	{
		CU_VEC<sm::vec2> simplified;
		sm::douglas_peucker(itr->first->GetVertices(), m_cmpt->GetSimplifyThreshold(), simplified);
		itr->second->SetVertices(simplified);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void EditPolylinesOP::updateFromSimplified()
{
	auto itr = m_simplify_buffer.begin();
	for ( ; itr != m_simplify_buffer.end(); ++itr)
		itr->first->SetVertices(itr->second->GetVertices());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void EditPolylinesOP::clearBuffer()
{
	m_simplify_buffer.clear();

	m_is_dirty = false;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::UpdateBufferVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylinesOP::UpdateBufferVisitor::
UpdateBufferVisitor(std::map<std::shared_ptr<ChainShape>, std::shared_ptr<ChainShape>>& simplifyBuffer)
	: m_simplify_buffer(simplifyBuffer)
{
}

void EditPolylinesOP::UpdateBufferVisitor::
Visit(const ee::ShapePtr& shape, bool& next)
{
	auto chain = std::dynamic_pointer_cast<ChainShape>(shape);
	auto cp = std::shared_ptr<s2::Shape>(shape->Clone());
	auto cp_chain = std::dynamic_pointer_cast<ChainShape>(cp);
	m_simplify_buffer.insert(std::make_pair(chain, cp_chain));
	next = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::OffsetVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylinesOP::OffsetVisitor::
OffsetVisitor(const sm::vec2& offset)
	: m_offset(offset)
{
}

void EditPolylinesOP::OffsetVisitor::
Visit(const ee::ShapePtr& shape, bool& next)
{
	auto chain = std::dynamic_pointer_cast<ChainShape>(shape);
	chain->Translate(m_offset);
	next = true;
}

}