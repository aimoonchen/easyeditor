#include "FilterModes.h"

namespace ee
{

CU_SINGLETON_DEFINITION(FilterModes);

FilterModes::FilterModes()
{
	m_modes.push_back(Item(s2::FM_NULL, "��"));

	m_modes.push_back(Item(s2::FM_EDGE_DETECTION, "�߽���"));
	m_modes.push_back(Item(s2::FM_RELIEF, "����"));
	m_modes.push_back(Item(s2::FM_OUTLINE, "��Χ"));
	m_modes.push_back(Item(s2::FM_OUTER_GLOW, "�ⷢ��"));

	m_modes.push_back(Item(s2::FM_GRAY, "�Ҷ�"));
	m_modes.push_back(Item(s2::FM_BLUR, "ģ��"));
	m_modes.push_back(Item(s2::FM_GAUSSIAN_BLUR, "��˹ģ��"));

	m_modes.push_back(Item(s2::FM_HEAT_HAZE, "����"));
	m_modes.push_back(Item(s2::FM_SHOCK_WAVE, "�����"));
	m_modes.push_back(Item(s2::FM_SWIRL, "����"));
	m_modes.push_back(Item(s2::FM_BURNING_MAP, "�յ�ͼ"));

	m_modes.push_back(Item(s2::FM_COL_GRADING, "���ұ�"));	
}

s2::FilterMode FilterModes::ID2Mode(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return s2::FM_NULL;
	} else {
		return m_modes[idx].id;
	}
}

int FilterModes::Mode2ID(s2::FilterMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].id == mode) {
			return i;
		}
	}
	return 0;
}

void FilterModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name);
	}
}

}