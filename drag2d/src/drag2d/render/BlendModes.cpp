#include "BlendModes.h"

#include <string>

namespace d2d
{

BlendModes* BlendModes::m_instance = NULL;

BlendModes::BlendModes()
{
	// normal
	m_modes.push_back(Item(BM_NORMAL, "normal", "����"));
//	m_modes.push_back(Item(BM_DISSOLVE, "dissolve", "�ܽ�"));

	// darken modes
	m_modes.push_back(Item(BM_DARKEN, "darken", "�䰵"));
	m_modes.push_back(Item(BM_MULTIPLY, "multiply", "��Ƭ����"));
	m_modes.push_back(Item(BM_COLOR_BURN, "color burn", "��ɫ����"));
	m_modes.push_back(Item(BM_LINEAR_BURN, "linear burn", "������ɫ����"));
//	m_modes.push_back(Item(BM_DARKER_COLOR, "darker color", "?"));

	// lighten modes
	m_modes.push_back(Item(BM_LIGHTEN, "lighten", "����"));
	m_modes.push_back(Item(BM_SCREEN, "screen", "��Ļ"));
	m_modes.push_back(Item(BM_COLOR_DODGE, "color dodge", "��ɫ����"));
	m_modes.push_back(Item(BM_LINEAR_DODGE, "linear dodge", "������ɫ����"));
//	m_modes.push_back(Item(BM_LIGHTER_COLOR, "lighter color", "?"));

	// saturation modes
	m_modes.push_back(Item(BM_OVERLAY, "overlay", "����"));
	m_modes.push_back(Item(BM_SOFT_LIGHT, "soft light", "���"));
	m_modes.push_back(Item(BM_HARD_LIGHT, "hard light", "ǿ��"));
	m_modes.push_back(Item(BM_VIVID_LIGHT, "vivid light", "�޹�"));
	m_modes.push_back(Item(BM_LINEAR_LIGHT, "linear light", "���Թ�"));
	m_modes.push_back(Item(BM_PIN_LIGHT, "pin light", "�̶���"));
	m_modes.push_back(Item(BM_HARD_MIX, "hard mix", "ʵɫ���"));

	// substraction modes
	m_modes.push_back(Item(BM_DIFFERENCE, "difference", "����"));
	m_modes.push_back(Item(BM_EXCLUSION, "exclusion", "�ų�"));

	// color modes 
// 	m_modes.push_back(Item(BM_HUE, "hue", "ɫ��"));
// 	m_modes.push_back(Item(BM_SATURATION, "saturation", "���Ͷ�"));
// 	m_modes.push_back(Item(BM_COLOR, "color", "��ɫ"));
// 	m_modes.push_back(Item(BM_LUMINOSITY, "luminosity", "����"));
}

BlendMode BlendModes::GetIDFromNameEN(const std::string& name) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name_cn == name) {
			return item.id;
		}
	}
	return BM_UNKNOWN;
}

BlendMode BlendModes::GetIDFromIdx(int idx) const
{
	if (idx < 0 || idx >= m_modes.size()) {
		return BM_UNKNOWN;
	} else {
		return m_modes[idx].id;
	}
}

int BlendModes::GetIdxFromID(BlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].id == mode) {
			return i;
		}
	}
	return 0;
}

void BlendModes::GetAllNameCN(wxArrayString& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name_cn);
	}
}

BlendModes* BlendModes::Instance()
{
	if (!m_instance) {
		m_instance = new BlendModes();
	}
	return m_instance;
}

}