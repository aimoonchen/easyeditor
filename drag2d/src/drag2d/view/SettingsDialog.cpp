#include "SettingsDialog.h"

#include "common/Config.h"
#include "common/SettingData.h"

namespace d2d
{

SettingsDialog::SettingsDialog(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Settings"))
	, m_settings(Config::Instance()->GetSettings())
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
		check->SetValue(m_settings.open_sprite_capture);
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
		check->SetValue(m_settings.open_image_edge_clip);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeImageEdgeClip));
		sizer->Add(check, 0);
	}
	sizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("��ʾԭʼ�߿�"));
		check->SetValue(m_settings.visible_image_edge);
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
			check->SetValue(m_settings.visible_label_bg);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeFontBackground));
			sizer->Add(check, 0);
		}
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("��������"));
			check->SetValue(m_settings.visible_label_text);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeFontText));
			sizer->Add(check, 0);
		}
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("�ڵ�����"));
			check->SetValue(m_settings.visible_node_name);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeNodeName));
			sizer->Add(check, 0);
		}
	}
	return sizer;
}

void SettingsDialog::onChangeSpriteCapture(wxCommandEvent& event)
{
	m_settings.open_sprite_capture = event.IsChecked();
}

void SettingsDialog::onChangeImageEdgeClip(wxCommandEvent& event)
{
	m_settings.open_image_edge_clip = event.IsChecked();
}

void SettingsDialog::onChangeVisibleImageEdge(wxCommandEvent& event)
{
	m_settings.visible_image_edge = event.IsChecked();
}

void SettingsDialog::onChangeFontBackground(wxCommandEvent& event)
{
	m_settings.visible_label_bg = event.IsChecked();
}

void SettingsDialog::onChangeFontText(wxCommandEvent& event)
{
	m_settings.visible_label_text = event.IsChecked();
}

void SettingsDialog::onChangeNodeName(wxCommandEvent& event)
{
	m_settings.visible_node_name = event.IsChecked();
}

}