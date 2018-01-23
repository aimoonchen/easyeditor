#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "ArrangeSpriteCMPT.h"
#include "PasteSymbolOP.h"
#include "BuildingCfg.h"
#include "config.h"

#include <ee/UniversalCMPT.h>
#include <ee/PasteSpriteCMPT.h>
#include <ee/StringHelper.h>
#include <ee/panel_msg.h>

#include <wx/spinctrl.h>

namespace sg
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library, 
						   ee::PropertySettingPanel* property, 
						   StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
{
	AddChild(new ee::UniversalCMPT(this, "paste", stage->GetStageImpl(), 
		std::make_shared<PasteSymbolOP>(stage, library)));
	AddChild(new ee::PasteSpriteCMPT(this, "batch", stage, stage->GetStageImpl(), stage));
	AddChild(new ArrangeSpriteCMPT(this, "arrange", property, stage));

	SetSizer(InitLayout());	

	stage->SetLayoutInfo(ROW, COL, EDGE);
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(10);
	initOptSetting(sizer);
	sizer->AddSpacer(20);
	sizer->Add(InitChildrenLayout());
	return sizer;
}

void ToolbarPanel::initOptSetting(wxSizer* sizer)
{
	wxChoice* choice = new wxChoice(this, wxID_ANY);
	for (int i = 1; i <= 10; ++i) {
		std::string str = "Townhall " + ee::StringHelper::ToString(i);
		choice->Append(str);	
	}
	sizer->Add(choice, 0, wxALIGN_LEFT);

	Connect(choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(ToolbarPanel::onLevelChanged));
	choice->SetSelection(0);
}

void ToolbarPanel::initSizeSetting(wxBoxSizer* topSizer)
{
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("row:")));

		m_row = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 2, 100, ROW);
		Connect(m_row->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_row);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("col:")));

		m_col = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 2, 100, ROW);
		Connect(m_col->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_col);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(20);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("length:")));

		m_edge = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 16, 128, EDGE);
		Connect(m_edge->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_edge);

		topSizer->Add(sizer);
	}
	onParamsChanged(wxSpinEvent());
	topSizer->AddSpacer(20);
}

void ToolbarPanel::onParamsChanged(wxSpinEvent& event)
{
	StagePanel* stage = static_cast<StagePanel*>(m_stage);
	stage->SetLayoutInfo(m_row->GetValue(), 
		m_col->GetValue(), m_edge->GetValue());
	stage->UpdateAllSpritesLocation();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ToolbarPanel::onLevelChanged(wxCommandEvent& event)
{
	m_stage->SetBaseLevel(event.GetSelection() + 1);
	m_stage->GetBuildingCfg().ResetLibraryList();
}

}