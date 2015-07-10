#include "FixturePropertySetting.h"

#include <easymodeling.h>

namespace emodeling
{

FixturePropertySetting::FixturePropertySetting(d2d::EditPanel* stage, libmodeling::Fixture* fixture)
	: d2d::IPropertySetting("Fixture")
	, m_stage(stage)
	, m_fixture(fixture)
{
}

void FixturePropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
		m_fixture->name = wxANY_AS(value, wxString);
	else if (name == wxT("density"))
		m_fixture->density = wxANY_AS(value, float);
	else if (name == wxT("friction"))
		m_fixture->friction = wxANY_AS(value, float);
	else if (name == wxT("restitution"))
		m_fixture->restitution = wxANY_AS(value, float);
	else if (name == wxT("isSensor"))
		m_fixture->isSensor = wxANY_AS(value, bool);
	else if (name == wxT("categoryBits"))
		m_fixture->categoryBits = wxANY_AS(value, int);
	else if (name == wxT("maskBits"))
		m_fixture->maskBits = wxANY_AS(value, int);
	else if (name == wxT("groupIndex"))
		m_fixture->groupIndex = wxANY_AS(value, int);
	else
		dirty = false;

	if (dirty) {
		m_stage->SetCanvasDirty();
	}
}

void FixturePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_fixture->name);
	pg->GetProperty(wxT("density"))->SetValue(m_fixture->density);
	pg->GetProperty(wxT("friction"))->SetValue(m_fixture->friction);
	pg->GetProperty(wxT("restitution"))->SetValue(m_fixture->restitution);
	pg->GetProperty(wxT("isSensor"))->SetValue(m_fixture->isSensor);
	pg->GetProperty(wxT("categoryBits"))->SetValue(m_fixture->categoryBits);
	pg->GetProperty(wxT("maskBits"))->SetValue(m_fixture->maskBits);
	pg->GetProperty(wxT("groupIndex"))->SetValue(m_fixture->groupIndex);
}

void FixturePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_fixture->name));

	pg->Append(new wxFloatProperty(wxT("density"), wxPG_LABEL, m_fixture->density));
	pg->SetPropertyAttribute(wxT("density"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("friction"), wxPG_LABEL, m_fixture->friction));
	pg->SetPropertyAttribute(wxT("friction"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("restitution"), wxPG_LABEL, m_fixture->restitution));
	pg->SetPropertyAttribute(wxT("restitution"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("isSensor"), wxPG_LABEL, m_fixture->isSensor));
	pg->SetPropertyAttribute("isSensor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxIntProperty(wxT("categoryBits"), wxPG_LABEL, m_fixture->categoryBits));
	pg->SetPropertyAttribute(wxT("categoryBits"), wxPG_ATTR_MIN, 0);
	pg->SetPropertyAttribute(wxT("categoryBits"), wxPG_ATTR_MAX, 0xFFFF);

	pg->Append(new wxIntProperty(wxT("maskBits"), wxPG_LABEL, m_fixture->maskBits));
	pg->SetPropertyAttribute(wxT("maskBits"), wxPG_ATTR_MIN, 0);
	pg->SetPropertyAttribute(wxT("maskBits"), wxPG_ATTR_MAX, 0xFFFF);

	pg->Append(new wxIntProperty(wxT("groupIndex"), wxPG_LABEL, m_fixture->groupIndex));
	pg->SetPropertyAttribute(wxT("groupIndex"), wxPG_ATTR_MIN, -32768);
	pg->SetPropertyAttribute(wxT("groupIndex"), wxPG_ATTR_MAX, 32767);
}

}