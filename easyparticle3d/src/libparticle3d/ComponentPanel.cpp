#include "ComponentPanel.h"
#include "ToolBarPanel.h"
#include "ps_config.h"
#include "PSConfigMgr.h"
#include "language.h"

#include <ps/particle3d.h>

namespace eparticle3d
{

static const float SCALE_START			= 100;
static const float SCALE_END			= 100;
static const float ROTATE_MIN			= 0;
static const float ROTATE_MAX			= 0;

ComponentPanel::ComponentPanel(wxWindow* parent, particle_symbol* pc, ToolbarPanel* toolbar)
	: wxPanel(parent)
	, m_pc(pc)
	, m_toolbar(toolbar)
{
	InitLayout();
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}
}

void ComponentPanel::SetValue(int key, const d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_SCALE:
		m_pc->scale_start = data.val0 * 0.01f;
		m_pc->scale_end = data.val1 * 0.01f;
		break;
	case PS_ROTATE:
		m_pc->angle = data.val0 * 0.5f * d2d::TRANS_DEG_TO_RAD;
		m_pc->angle_var = data.val1 * 0.5f * d2d::TRANS_DEG_TO_RAD;
		break;
	case PS_ALPHA:
		m_pc->alpha_start = data.val0 * 0.01f;
		m_pc->alpha_end = data.val1 * 0.01f;
		break;
	}
}

void ComponentPanel::GetValue(int key, d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_SCALE:
		data.val0 = m_pc->scale_start * 100;
		data.val1 = m_pc->scale_end * 100;
		break;
	case PS_ROTATE:
		data.val0 = (m_pc->angle - m_pc->angle_var) * d2d::TRANS_RAD_TO_DEG;
		data.val1 = (m_pc->angle + m_pc->angle_var) * d2d::TRANS_RAD_TO_DEG;
		break;
	case PS_ALPHA:
		data.val0 = m_pc->alpha_start * 100;
		data.val1 = m_pc->alpha_end * 100;
		break;
	}
}

const ps_color4f& ComponentPanel::GetMulColor() const
{
	return m_pc->col_mul;
}

const ps_color4f& ComponentPanel::GetAddColor() const
{
	return m_pc->col_add;
}

void ComponentPanel::InitLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->AddSpacer(10);
	// Del
	{
		wxButton* btn = new wxButton(this, wxID_ANY, LANG[LK_REMOVE]);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnDelete));
		topSizer->Add(btn);
	}
	// Name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_NAME]));
		sizer->Add(m_name = new wxTextCtrl(this, wxID_ANY));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Icon
	{
		std::string filepath = static_cast<d2d::ISymbol*>(m_pc->ud)->GetFilepath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
			d2d::ImagePanel* panel = new d2d::ImagePanel(this, filepath, 100, 100);
			topSizer->Add(panel);
		}
	}
	topSizer->AddSpacer(10);
	// Scale
	d2d::SliderCtrlTwo* s_scale = new d2d::SliderCtrlTwo(this, LANG[LK_SCALE], "scale", this, PS_SCALE, 
		d2d::SliderItem(LANG[LK_START], "start", SCALE_START, 0, 500), d2d::SliderItem(LANG[LK_END], "end", SCALE_END, 0, 500));
	topSizer->Add(s_scale);
	topSizer->AddSpacer(10);
	m_sliders.push_back(s_scale);
	// Rotate
	d2d::SliderCtrlTwo* s_rotate = new d2d::SliderCtrlTwo(this, LANG[LK_ROTATE], "rotate", this, PS_ROTATE, 
		d2d::SliderItem(LANG[LK_MIN], "min", ROTATE_MIN, -180, 180), d2d::SliderItem(LANG[LK_MAX], "max", ROTATE_MAX, -180, 180));
	topSizer->Add(s_rotate);
	topSizer->AddSpacer(10);
	m_sliders.push_back(s_rotate);
	// Multi Color
	wxButton* mul_btn = new wxButton(this, wxID_ANY, LANG[LK_COL_MUL]);
	Connect(mul_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetMultiCol));
	topSizer->Add(mul_btn);
	// Add Color
	wxButton* add_btn = new wxButton(this, wxID_ANY, LANG[LK_COL_ADD]);
	Connect(add_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetAddCol));
	topSizer->Add(add_btn);
	// Alpha
	d2d::SliderCtrlTwo* s_alpha = new d2d::SliderCtrlTwo(this, LANG[LK_ALPHA], "alpha", this, PS_ALPHA, 
		d2d::SliderItem(LANG[LK_START], "start", 100, 0, 100), d2d::SliderItem(LANG[LK_END], "end", 100, 0, 100));
	topSizer->Add(s_alpha);
	topSizer->AddSpacer(10);
	m_sliders.push_back(s_alpha);
	// Bind PS
	{
		wxButton* btn = new wxButton(this, wxID_ANY, LANG[LK_BIND_PS]);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnBindPS));
		topSizer->Add(btn);
	}
	//
	SetSizer(topSizer);
}

void ComponentPanel::OnDelete(wxCommandEvent& event)
{
	m_toolbar->OnDelChild(this);
}

void ComponentPanel::OnBindPS(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_particle3d);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("����Particle3D�ļ�"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_pc->bind_ps_cfg = PSConfigMgr::Instance()->GetConfig(dlg.GetPath().ToStdString());

		// 		if (m_canvas) {
		// 			m_canvas->ResetViewport();
		// 		}
	}
}

void ComponentPanel::OnSetMultiCol(wxCommandEvent& event)
{
	d2d::Colorf col;
	memcpy(&col.r, &m_pc->col_mul.r, sizeof(m_pc->col_mul));

	d2d::RGBColorSettingDlg dlg(this, NULL, col);
	if (!dlg.ShowModal()) {
		return;
	}

	col = dlg.GetColor();
	memcpy(&m_pc->col_mul.r, &col.r, sizeof(m_pc->col_mul));
}

void ComponentPanel::OnSetAddCol(wxCommandEvent& event)
{
	d2d::Colorf col;
	memcpy(&col.r, &m_pc->col_add.r, sizeof(m_pc->col_add));

	d2d::RGBColorSettingDlg dlg(this, NULL, col);
	if (!dlg.ShowModal()) {
		return;
	}

	col = dlg.GetColor();
	memcpy(&m_pc->col_add.r, &col.r, sizeof(m_pc->col_add));
}

}