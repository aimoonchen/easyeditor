#ifndef _EASYMODELING_FIXTURE_PROPERTY_SETTING_H_
#define _EASYMODELING_FIXTURE_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace ee { class EditPanelImpl; }

namespace emodeling
{

class Fixture;

class FixturePropertySetting : public ee::PropertySetting
{
public:
	FixturePropertySetting(ee::EditPanelImpl* stage, Fixture* fixture);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

private:
	ee::EditPanelImpl* m_stage;

	Fixture* m_fixture;

}; // FixturePropertySetting

}

#endif // _EASYMODELING_FIXTURE_PROPERTY_SETTING_H_
