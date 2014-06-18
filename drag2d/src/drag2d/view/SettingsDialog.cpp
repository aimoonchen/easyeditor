#include "SettingsDialog.h"

#include "common/Settings.h"

namespace d2d
{

SettingsDialog::SettingsDialog(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Settings"))
{
	initLayout();
}

void SettingsDialog::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddSpacer(10);
	sizer->Add(initEditPanel(), 0, wxCENTER);
	sizer->AddSpacer(10);
	sizer->Add(initImagePanel(), 0, wxCENTER);
	sizer->AddSpacer(10);
	sizer->Add(initViewPanel(), 0, wxCENTER);
	sizer->AddSpacer(10);
	SetSizer(sizer);

	sizer->Layout();
	Refresh();
}

wxSizer* SettingsDialog::initEditPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("�༭"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("����׽"));
		check->SetValue(Settings::bSpriteCapture);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeSpriteCapture));
		sizer->Add(check, 0);
	}
	return sizer;
}

wxSizer* SettingsDialog::initImagePanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("ͼƬ"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("��Ե�ü�"));
		check->SetValue(Settings::bImageEdgeClip);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeImageEdgeClip));
		sizer->Add(check, 0);
	}
	sizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("��ʾԭʼ�߿�"));
		check->SetValue(Settings::bVisibleImgEdge);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeVisibleImageEdge));
		sizer->Add(check, 0);
	}
	return sizer;
}

wxSizer* SettingsDialog::initViewPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("��ʾ"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("���ֿ�"));
			check->SetValue(d2d::Settings::DrawFontType & d2d::Settings::DrawFontBg);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeFontBackground));
			sizer->Add(check, 0);
		}
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("��������"));
			check->SetValue(d2d::Settings::DrawFontType & d2d::Settings::DrawFontText);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeFontText));
			sizer->Add(check, 0);
		}
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("�ڵ�����"));
			check->SetValue(d2d::Settings::bVisibleNodeName);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeNodeName));
			sizer->Add(check, 0);
		}
	}
	return sizer;
}

void SettingsDialog::onChangeSpriteCapture(wxCommandEvent& event)
{
	Settings::bSpriteCapture = event.IsChecked();
}

void SettingsDialog::onChangeImageEdgeClip(wxCommandEvent& event)
{
	Settings::bImageEdgeClip = event.IsChecked();
}

void SettingsDialog::onChangeVisibleImageEdge(wxCommandEvent& event)
{
	Settings::bVisibleImgEdge = event.IsChecked();
}

void SettingsDialog::onChangeFontBackground(wxCommandEvent& event)
{
	d2d::Settings::DrawFontType ^= d2d::Settings::DrawFontBg;
}

void SettingsDialog::onChangeFontText(wxCommandEvent& event)
{
	d2d::Settings::DrawFontType ^= d2d::Settings::DrawFontText;
}

void SettingsDialog::onChangeNodeName(wxCommandEvent& event)
{
	Settings::bVisibleNodeName = event.IsChecked();
}

}