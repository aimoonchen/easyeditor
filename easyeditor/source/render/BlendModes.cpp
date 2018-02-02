#include "BlendModes.h"

#include <string>

namespace ee
{

CU_SINGLETON_DEFINITION(BlendModes);

BlendModes::BlendModes()
{
	// normal
	m_modes.push_back(Item(pt2::BM_NULL, "��"));
//	m_modes.push_back(Item(pt2::BM_DISSOLVE, "�ܽ�"));

	// darken modes
	m_modes.push_back(Item(pt2::BM_DARKEN, "�䰵"));
	m_modes.push_back(Item(pt2::BM_MULTIPLY, "��Ƭ����"));
	m_modes.push_back(Item(pt2::BM_COLOR_BURN, "��ɫ����"));
	m_modes.push_back(Item(pt2::BM_LINEAR_BURN, "������ɫ����"));
//	m_modes.push_back(Item(pt2::BM_DARKER_COLOR, "?"));

	// lighten modes
	m_modes.push_back(Item(pt2::BM_LIGHTEN, "����"));
	m_modes.push_back(Item(pt2::BM_SCREEN, "��Ļ"));
	m_modes.push_back(Item(pt2::BM_COLOR_DODGE, "��ɫ����"));
	m_modes.push_back(Item(pt2::BM_LINEAR_DODGE, "������ɫ����"));
//	m_modes.push_back(Item(pt2::BM_LIGHTER_COLOR, "?"));

	// saturation modes
	m_modes.push_back(Item(pt2::BM_OVERLAY, "����"));
	m_modes.push_back(Item(pt2::BM_SOFT_LIGHT, "���"));
	m_modes.push_back(Item(pt2::BM_HARD_LIGHT, "ǿ��"));
	m_modes.push_back(Item(pt2::BM_VIVID_LIGHT, "����"));
	m_modes.push_back(Item(pt2::BM_LINEAR_LIGHT, "���Թ�"));
	m_modes.push_back(Item(pt2::BM_PIN_LIGHT, "�̶���"));
	m_modes.push_back(Item(pt2::BM_HARD_MIX, "ʵɫ���"));

	// substraction modes
	m_modes.push_back(Item(pt2::BM_DIFFERENCE, "����"));
	m_modes.push_back(Item(pt2::BM_EXCLUSION, "�ų�"));

	// color modes 
// 	m_modes.push_back(Item(pt2::BM_HUE, "ɫ��"));
// 	m_modes.push_back(Item(pt2::BM_SATURATION, "���Ͷ�"));
// 	m_modes.push_back(Item(pt2::BM_COLOR, "��ɫ"));
// 	m_modes.push_back(Item(pt2::BM_LUMINOSITY, "����"));
}

pt2::BlendMode BlendModes::ID2Mode(int id) const
{
	if (id < 0 || id >= static_cast<int>(m_modes.size())) {
		return pt2::BM_NULL;
	} else {
		return m_modes[id].mode;
	}
}

int BlendModes::Mode2ID(pt2::BlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].mode == mode) {
			return i;
		}
	}
	return 0;
}

void BlendModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name);
	}
}

}