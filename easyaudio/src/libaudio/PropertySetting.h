#ifndef _EASYAUDIO_PROPERTY_SETTING_H_
#define _EASYAUDIO_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace eaudio
{

class Sprite;

class PropertySetting : public ee::SpritePropertySetting
{
public:
	PropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr);
	virtual ~PropertySetting();

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	wxWindow* m_parent;

}; // PropertySetting

}

#endif // _EASYAUDIO_PROPERTY_SETTING_H_