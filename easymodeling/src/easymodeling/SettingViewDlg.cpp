
#include "SettingViewDlg.h"

#include <wx/spinctrl.h>

using namespace emodeling;

SettingViewDlg::SettingViewDlg(wxWindow* parent, d2d::IStageCanvas* canvas)
	: wxDialog(parent, wxID_ANY, wxT("Display Setting"))
	, m_canvas(canvas)
	, m_ctlPointSize(NULL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddSpacer(20);
	sizer->Add(initDisplayTypePanel(), 0, wxCENTRE/* | wxALL, 20*/);
	sizer->AddSpacer(20);
	sizer->Add(initStylePanel(), 0, wxCENTRE/* | wxALL, 20*/);
	SetSizer(sizer);

	sizer->Layout();
	Refresh(true);
}

wxSizer* SettingViewDlg::initDisplayTypePanel()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("physics+image"));
	choices.Add(wxT("physics"));
	choices.Add(wxT("image"));

	wxRadioBox* drawChoice = new wxRadioBox(this, wxID_ANY, wxT("display type"), 
		wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	Connect(drawChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
		wxCommandEventHandler(SettingViewDlg::onChangeDisplayType));
	drawChoice->SetSelection(d2d::Settings::drawType);

	sizer->Add(drawChoice);

	return sizer;
}

wxSizer* SettingViewDlg::initStylePanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("display style"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("polyline"));
		wxBoxSizer* lineSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("node size:")));

			m_ctlPointSize = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
				wxSP_ARROW_KEYS, 0, 30, d2d::Settings::ctlPosSize);
			Connect(m_ctlPointSize->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(SettingViewDlg::onChangeStyle));
			sizer->Add(m_ctlPointSize);

			lineSizer->Add(sizer);
		}
		topSizer->Add(lineSizer);
	}
	topSizer->AddSpacer(10);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("polygon"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxCheckBox* trisCheck = new wxCheckBox(this, wxID_ANY, wxT("triangle edge"));
			trisCheck->SetValue(d2d::Settings::bDisplayTrisEdge);
			Connect(trisCheck->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingViewDlg::onChangeDisplayTriangles));
			sizer->Add(trisCheck);
		}
		topSizer->AddSpacer(5);
		{
			wxCheckBox* boundCheck = new wxCheckBox(this, wxID_ANY, wxT("bounding"));
			boundCheck->SetValue(d2d::Settings::bDisplayPolyBound);
			Connect(boundCheck->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingViewDlg::onChangeDisplayPolygonBound));
			sizer->Add(boundCheck);
		}
		topSizer->Add(sizer);
	}
	return topSizer;
}

void SettingViewDlg::onChangeDisplayType(wxCommandEvent& event)
{
	d2d::Settings::drawType = static_cast<d2d::Settings::DrawType>(event.GetSelection());

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SettingViewDlg::onChangeStyle(wxSpinEvent& event)
{
	d2d::Settings::ctlPosSize = m_ctlPointSize->GetValue();
	if (m_canvas) m_canvas->Refresh(true);
}

void SettingViewDlg::onChangeDisplayTriangles(wxCommandEvent& event)
{
	d2d::Settings::bDisplayTrisEdge = event.IsChecked();
	if (m_canvas) m_canvas->Refresh(true);
}

void SettingViewDlg::onChangeDisplayPolygonBound(wxCommandEvent& event)
{
	d2d::Settings::bDisplayPolyBound = event.IsChecked();
	if (m_canvas) m_canvas->Refresh(true);
}