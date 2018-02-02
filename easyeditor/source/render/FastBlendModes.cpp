#include "FastBlendModes.h"

#include <string>

namespace ee
{

CU_SINGLETON_DEFINITION(FastBlendModes);

FastBlendModes::FastBlendModes()
{
	m_modes.push_back(Item(pt2::FBM_NULL, "��"));
	m_modes.push_back(Item(pt2::FBM_ADD, "���"));
	m_modes.push_back(Item(pt2::FBM_SUBTRACT, "���"));
}

pt2::FastBlendMode FastBlendModes::ID2Mode(int id) const
{
	if (id < 0 || id >= static_cast<int>(m_modes.size())) {
		return pt2::FBM_NULL;
	} else {
		return m_modes[id].mode;
	}
}

int FastBlendModes::Mode2ID(pt2::FastBlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].mode == mode) {
			return i;
		}
	}
	return 0;
}

void FastBlendModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name);
	}
}

}