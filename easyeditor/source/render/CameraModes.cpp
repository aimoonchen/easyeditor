#include "CameraModes.h"

#include <string>

namespace ee
{

CU_SINGLETON_DEFINITION(CameraModes);

CameraModes::CameraModes()
{
	m_modes.push_back(Item(pt2::CM_ORTHO, "����"));
	m_modes.push_back(Item(pt2::CM_PERSPECTIVE_NO_HEIGHT, "ͶӰû�߶�"));
	m_modes.push_back(Item(pt2::CM_PERSPECTIVE_AUTO_HEIGHT, "ͶӰ����߶�"));
}

pt2::CameraMode CameraModes::ID2Mode(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return pt2::CM_ORTHO;
	} else {
		return m_modes[idx].mode;
	}
}

int CameraModes::Mode2ID(pt2::CameraMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].mode == mode) {
			return i;
		}
	}
	return 0;
}

void CameraModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name);
	}
}

}