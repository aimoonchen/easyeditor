#include "EditNodesOP.h"
#include "ChainShape.h"
#include "EditNodesAtomicOP.h"
#include "message/panel_msg.h"

namespace libshape
{

EditNodesOP::EditNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl)
	: SelectNodesOP(wnd, stage, shapesImpl, NULL)
{
	m_lastPos.SetInvalid();
}

bool EditNodesOP::OnKeyDown(int keyCode)
{
	if (keyCode == WXK_DELETE)
	{
		for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		{
			SelectNodesOP::ChainSelectedNodes* selected = m_nodeSelection[i];
			for (size_t j = 0, m = selected->selectedNodes.size(); j < m; ++j)
				selected->chain->Remove(selected->selectedNodes[j]);
		}
	}

	SelectNodesOP::OnKeyDown(keyCode);

	return false;
}

bool EditNodesOP::OnMouseLeftDown(int x, int y)
{
	if (SelectNodesOP::OnMouseLeftDown(x, y)) return true;

	if (m_stage->GetKeyState(WXK_CONTROL)) return false;

	if (m_nodeSelection.empty())
		m_lastPos.SetInvalid();
	else
		m_lastPos = m_stage->TransPosScrToProj(x, y);

	return false;
}

bool EditNodesOP::OnMouseDrag(int x, int y)
{
	if (SelectNodesOP::OnMouseDrag(x, y)) return true;

	if (m_stage->GetKeyState(WXK_CONTROL)) return false;

	if (m_lastPos.IsValid())
	{
		ee::Vector currPos = m_stage->TransPosScrToProj(x, y);
		ee::Vector offset = currPos - m_lastPos;
		for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		{
			SelectNodesOP::ChainSelectedNodes* selected = m_nodeSelection[i];
			for (size_t j = 0, m = selected->selectedNodes.size(); j < m; ++j)
			{
				const ee::Vector& from = selected->selectedNodes[j];
				ee::Vector to = from + offset;
				selected->chain->Change(from, to);
				selected->selectedNodes[j] = to;
			}
			selected->chain->refresh();
		}
		m_lastPos = currPos;

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
		ee::PrimitiveDraw::DrawPolyline(m_buffer[i].dst, ee::Colorf(0.8f, 0.2f, 0.2f), false);
		ee::PrimitiveDraw::DrawCircles(m_buffer[i].dst, radius, true, 2, ee::Colorf(0.2f, 0.2f, 0.8f));
	}

	return false;
}

bool EditNodesOP::Clear()
{
	if (SelectNodesOP::Clear()) return true;

	m_lastPos.SetInvalid();
	m_buffer.clear();

	return false;
}

void EditNodesOP::simplify(float threshold)
{
	m_buffer.clear();
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		Modified modified;
		modified.src = m_nodeSelection[i];
		ee::DouglasPeucker::Do(modified.src->selectedNodes, threshold, modified.dst);
		m_buffer.push_back(modified);
	}
}

void EditNodesOP::smooth(float samplingWidth)
{
	m_buffer.clear();
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		Modified modified;
		modified.src = m_nodeSelection[i];
		ee::CosineSmooth::Do(modified.src->selectedNodes, samplingWidth, modified.dst);
		m_buffer.push_back(modified);
	}
}

void EditNodesOP::updateModified()
{
	std::vector<ChainShape*> chains;
	std::vector<std::vector<ee::Vector> > chainsDst;

	for (size_t i = 0, n = m_buffer.size(); i < n; ++i)
	{
		const std::vector<ee::Vector>& chain = m_buffer[i].src->chain->GetVertices();
		const std::vector<ee::Vector>& src = m_buffer[i].src->selectedNodes;
		const std::vector<ee::Vector>& dst = m_buffer[i].dst;

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
		std::vector<ee::Vector> result;
		copy(chain.begin(), chain.begin() + begin, back_inserter(result));
		copy(dst.begin(), dst.end(), back_inserter(result));
		copy(chain.begin() + end + 1, chain.end(), back_inserter(result));

		m_buffer[i].src->chain->Load(result);

		chains.push_back(m_buffer[i].src->chain);
		chainsDst.push_back(result);
	}

	ee::EditAddRecordSJ::Instance()->Add(new edit_nodes::ModifyNodesAOP(chains, chainsDst));

	m_buffer.clear();
	clearSelectedNodes();
}

}