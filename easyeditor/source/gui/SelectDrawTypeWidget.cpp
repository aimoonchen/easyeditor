#include "SelectDrawTypeWidget.h"
#include "SettingData.h"
#include "OrthoCanvas.h"

#include <wx/wx.h>

namespace ee
{

SelectDrawTypeWidget::SelectDrawTypeWidget(wxWindow* parent, OrthoCanvas* canvas)
	: Widget(parent)
	, m_canvas(canvas)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("����+ͼƬ"));
	choices.Add(wxT("����"));
	choices.Add(wxT("ͼƬ"));

	wxRadioBox* choice = new wxRadioBox(this, wxID_ANY, wxT("��ʾ��ʽ"), 
		wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	Connect(choice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
		wxCommandEventHandler(SelectDrawTypeWidget::OnSetDrawType));
	choice->SetSelection(SettingData::draw_type);

	sizer->Add(choice);

	SetSizer(sizer);
}

void SelectDrawTypeWidget::OnSetDrawType(wxCommandEvent& event)
{
	SettingData::draw_type = static_cast<SettingData::DrawType>(event.GetSelection());
	m_canvas->Refresh(true);
}

}