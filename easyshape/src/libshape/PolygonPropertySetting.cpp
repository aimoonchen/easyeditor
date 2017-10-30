#include "PolygonPropertySetting.h"
#include "PolygonShape.h"

#include <ee/panel_msg.h>

namespace eshape
{

PolygonPropertySetting::PolygonPropertySetting(ee::EditPanelImpl* stage, 
											   const std::shared_ptr<PolygonShape>& poly)
	: PropertySetting("Polygon")
	, m_stage(stage)
	, m_poly(poly)
{
}

void PolygonPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		m_poly->SetName(wxANY_AS(value, wxString).ToStdString());
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float dx = x - m_poly->GetBounding().Center().x;
		m_poly->Translate(sm::vec2(dx, 0.0f));
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float dy = y - m_poly->GetBounding().Center().y;
		m_poly->Translate(sm::vec2(0.0f, dy));
	}
	else if (name == wxT("Mirror"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1)
		{
			float x = m_poly->GetBounding().Center().x;
			auto vertices = m_poly->GetVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].x = x * 2 - vertices[i].x;
			m_poly->SetVertices(vertices);
		}
		else if (type == 2)
		{
			float y = m_poly->GetBounding().Center().y;
			auto vertices = m_poly->GetVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].y = y * 2 - vertices[i].y;
			m_poly->SetVertices(vertices);
		}
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void PolygonPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_poly->GetName());
	sm::vec2 center = m_poly->GetBounding().Center();
	pg->GetProperty(wxT("X"))->SetValue(center.x);
	pg->GetProperty(wxT("Y"))->SetValue(center.y);
	pg->GetProperty(wxT("Mirror"))->SetValue(wxT("none"));
}

void PolygonPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_poly->GetName()));

	sm::vec2 center = m_poly->GetBounding().Center();

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, center.x));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, center.y));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	static const wxChar* mirror_labels[] = { wxT("none"),
		wxT("horizontal"), wxT("vertical"), NULL };
	pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
}

}