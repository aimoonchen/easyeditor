#include "EditNodesOP.h"
#include "ChainShape.h"
#include "EditNodesAtomicOP.h"

#include <ee/panel_msg.h>
#include <ee/EditPanelImpl.h>
#include <ee/SettingData.h>

#include <sprite2/RVG.h>
#include <SM_CosineSmooth.h>
#include <SM_DouglasPeucker.h>

namespace eshape
{

EditNodesOP::EditNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl)
	: SelectNodesOP(wnd, stage, shapes_impl, NULL)
{
	m_last_pos.MakeInvalid();
}

bool EditNodesOP::OnKeyDown(int keyCode)
{
	if (keyCode == WXK_DELETE)
	{
		for (size_t i = 0, n = m_node_selection.size(); i < n; ++i)
		{
			SelectNodesOP::ChainSelectedNodes* selected = m_node_selection[i];
			for (size_t j = 0, m = selected->selectedNodes.size(); j < m; ++j)
				selected->polyline->RemoveVertex(selected->selectedNodes[j]);
		}
	}

	SelectNodesOP::OnKeyDown(keyCode);

	return false;
}

bool EditNodesOP::OnMouseLeftDown(int x, int y)
{
	if (SelectNodesOP::OnMouseLeftDown(x, y)) return true;

	if (m_stage->GetKeyState(WXK_CONTROL)) return false;

	if (m_node_selection.empty()) {
		m_last_pos.MakeInvalid();
	} else {
		m_last_pos = m_stage->TransPosScrToProj(x, y);
	}

	return false;
}

bool EditNodesOP::OnMouseDrag(int x, int y)
{
	if (SelectNodesOP::OnMouseDrag(x, y)) return true;

	if (m_stage->GetKeyState(WXK_CONTROL)) return false;

	if (m_last_pos.IsValid())
	{
		sm::vec2 curr_pos = m_stage->TransPosScrToProj(x, y);
		sm::vec2 offset = curr_pos - m_last_pos;
		for (size_t i = 0, n = m_node_selection.size(); i < n; ++i)
		{
			SelectNodesOP::ChainSelectedNodes* selected = m_node_selection[i];
			for (size_t j = 0, m = selected->selectedNodes.size(); j < m; ++j)
			{
				const sm::vec2& from = selected->selectedNodes[j];
				sm::vec2 to = from + offset;
				selected->polyline->ChangeVertex(from, to);
				selected->selectedNodes[j] = to;
			}
		}
		m_last_pos = curr_pos;

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditNodesOP::OnDraw() const
{
	if (SelectNodesOP::OnDraw()) return true;

	const float radius = ee::SettingData::ctl_pos_sz == 0 ? 3 : ee::SettingData::ctl_pos_sz;
	for (size_t i = 0, n = m_buffer.size(); i < n; ++i)
	{
		s2::RVG::SetColor(pt2::Color(204, 51, 51));
		s2::RVG::Polyline(nullptr, m_buffer[i].dst, false);
		s2::RVG::SetColor(pt2::Color(51, 51, 204));
		s2::RVG::Circles(nullptr, m_buffer[i].dst, radius, true);
	}

	return false;
}

bool EditNodesOP::Clear()
{
	if (SelectNodesOP::Clear()) return true;

	m_last_pos.MakeInvalid();
	m_buffer.clear();

	return false;
}

void EditNodesOP::Simplify(float threshold)
{
	m_buffer.clear();
	for (size_t i = 0, n = m_node_selection.size(); i < n; ++i)
	{
		Modified modified;
		modified.src = m_node_selection[i];
		sm::douglas_peucker(modified.src->selectedNodes, threshold, modified.dst);
		m_buffer.push_back(modified);
	}
}

void EditNodesOP::Smooth(float samplingWidth)
{
	m_buffer.clear();
	for (size_t i = 0, n = m_node_selection.size(); i < n; ++i)
	{
		Modified modified;
		modified.src = m_node_selection[i];
		sm::cosine_smooth(modified.src->selectedNodes, samplingWidth, modified.dst);
		m_buffer.push_back(modified);
	}
}

void EditNodesOP::UpdateModified()
{
	std::vector<std::shared_ptr<EditedPolyShape>> polylines;
	CU_VEC<CU_VEC<sm::vec2> > chainsDst;

	for (size_t i = 0, n = m_buffer.size(); i < n; ++i)
	{
		const CU_VEC<sm::vec2>& chain = m_buffer[i].src->polyline->GetVertices();
		const CU_VEC<sm::vec2>& src = m_buffer[i].src->selectedNodes;
		const CU_VEC<sm::vec2>& dst = m_buffer[i].dst;

		size_t begin = 0, end = chain.size() - 1;
		for (size_t j = 0, m = chain.size(); j < m; ++j)
		{
			if (src.front() == chain[j])
			{
				begin = j;
				break;
			}
		}
		for (int j = chain.size() - 1; j >= 0; --j)
		{
			if (src.back() == chain[j])
			{
				end = j;
				break;
			}
		}

		assert(begin <= end);
		CU_VEC<sm::vec2> result;
		copy(chain.begin(), chain.begin() + begin, back_inserter(result));
		copy(dst.begin(), dst.end(), back_inserter(result));
		copy(chain.begin() + end + 1, chain.end(), back_inserter(result));

		m_buffer[i].src->polyline->SetVertices(result);

		polylines.push_back(m_buffer[i].src->polyline);
		chainsDst.push_back(result);
	}

	ee::EditAddRecordSJ::Instance()->Add(
		std::make_shared<edit_nodes::ModifyNodesAOP>(polylines, chainsDst));

	m_buffer.clear();
	ClearSelectedNodes();
}

}