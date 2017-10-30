#include "PropertySetting.h"
#include "Sprite.h"
#include "Symbol.h"

#include <sprite2/UpdateParams.h>

#include <ee/panel_msg.h>

namespace libanim2
{

PropertySetting::PropertySetting(ee::EditPanelImpl* edit_impl, const std::shared_ptr<Sprite>& spr)
	: ee::SpritePropertySetting(edit_impl, spr)
{
	m_type = "Anim2";
}

void PropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());
	if (name == "Static") {
		spr->SetStaticTime(s2::UpdateParams(), wxANY_AS(value, int));
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());
	pg->GetProperty("Static")->SetValue(spr->GetStaticTime());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("ANIM2", wxPG_LABEL));

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());

	wxIntProperty* static_prop = new wxIntProperty("Static", wxPG_LABEL, spr->GetStaticTime());
	static_prop->SetValue(spr->GetStaticTime());
	pg->Append(static_prop);
}

}