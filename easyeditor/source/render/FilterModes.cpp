#include "FilterModes.h"
#include "ShaderMgr.h"

namespace ee
{

FilterModes* FilterModes::m_instance = NULL;

FilterModes::FilterModes()
{
	m_modes.push_back(Item(FM_NULL, "null", "��"));

	m_modes.push_back(Item(FM_EDGE_DETECTION, "edge detection", "�߽���"));
	m_modes.push_back(Item(FM_RELIEF, "relief", "����"));
	m_modes.push_back(Item(FM_OUTLINE, "outline", "��Χ"));

	m_modes.push_back(Item(FM_GRAY, "gray", "�Ҷ�"));
	m_modes.push_back(Item(FM_BLUR, "blur", "ģ��"));
	m_modes.push_back(Item(FM_GAUSSIAN_BLUR, "gaussian blur", "��˹ģ��"));

	m_modes.push_back(Item(FM_HEAT_HAZE, "heat haze", "����"));
	m_modes.push_back(Item(FM_SHOCK_WAVE, "shock wave", "�����"));
	m_modes.push_back(Item(FM_SWIRL, "swirl", "����"));
}

FilterMode FilterModes::GetModeFromNameEN(const std::string& name) const
{
	if (name.empty()) {
		return FM_NULL;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name_en == name) {
			return item.id;
		}
	}
	return FM_NULL;
}

std::string FilterModes::GetNameENFromMode(FilterMode id) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.id == id) {
			return item.name_en;
		}
	}
	return "";
}

FilterMode FilterModes::GetModeFromIdx(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return FM_NULL;
	} else {
		return m_modes[idx].id;
	}
}

int FilterModes::GetIdxFromMode(FilterMode mode) const
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
		names.push_back(m_modes[i].name_cn);
	}
}

int FilterModes::QueryShaderIdx(FilterMode mode) const
{
	std::map<FilterMode, int>::const_iterator itr = 
		m_map2idx.find(mode);
	if (itr == m_map2idx.end()) {
		return 0;
	} else {
		return itr->second;
	}
}

FilterModes* FilterModes::Instance()
{
	if (!m_instance) {
		m_instance = new FilterModes();
	}
	return m_instance;
}

}