#include "CameraModes.h"

#include <string>

namespace ee
{

CameraModes* CameraModes::m_instance = NULL;

CameraModes::CameraModes()
{
	m_modes.push_back(Item(CM_ORTHO, "ortho", "����"));
	m_modes.push_back(Item(CM_PERSPECTIVE_NO_HEIGHT, "perspective no height", "ͶӰû�߶�"));
	m_modes.push_back(Item(CM_PERSPECTIVE_AUTO_HEIGHT, "perspective auto height", "ͶӰ����߶�"));
}

CameraMode CameraModes::GetModeFromNameEN(const std::string& name) const
{
	if (name.empty()) {
		return CM_ORTHO;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name_en == name) {
			return item.mode;
		}
	}
	return CM_ORTHO;
}

std::string CameraModes::GetNameENFromMode(CameraMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.mode == mode) {
			return item.name_en;
		}
	}
	return "";
}

CameraMode CameraModes::GetIDFromIdx(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return CM_ORTHO;
	} else {
		return m_modes[idx].mode;
	}
}

int CameraModes::GetIdxFromID(CameraMode mode) const
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
		names.push_back(m_modes[i].name_cn);
	}
}

CameraModes* CameraModes::Instance()
{
	if (!m_instance) {
		m_instance = new CameraModes();
	}
	return m_instance;
}

}