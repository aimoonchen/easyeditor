#include "ImageProcessCMPT.h"
#include "ImageProcessOP.h"
#include "StagePanel.h"

#include <easyimage.h>

namespace eimage
{

ImageProcessCMPT::ImageProcessCMPT(wxWindow* parent, const wxString& name, 
								   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_edge_shader(NULL)
{
//	m_editOP = new ImageProcessOP(stage);

	m_editOP = new d2d::ZoomViewOP(stage, true);
}

wxSizer* ImageProcessCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "����");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ImageProcessCMPT::OnNormal));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "��Ե���");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ImageProcessCMPT::OnEdgeDetection));
		sizer->Add(btn);

		wxSlider* slider = new wxSlider(this, wxID_ANY, 50, 0, 100);
		Connect(slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(ImageProcessCMPT::OnSetBlend));
		sizer->Add(slider);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "����");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ImageProcessCMPT::OnRelief));
		sizer->Add(btn);
	}
	return sizer;
}

void ImageProcessCMPT::OnNormal(wxCommandEvent& event)
{
	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	d2d::SpriteShader* shader = new d2d::SpriteShader;
	shader->Load();
	shader_mgr->SetSpriteShader(shader);
	m_stage->getCanvas()->resetViewport();
	m_stage->Refresh();
}

void ImageProcessCMPT::OnEdgeDetection(wxCommandEvent& event)
{
	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	m_edge_shader = new EdgeDetectionShader;
	m_edge_shader->Load();
	shader_mgr->SetSpriteShader(m_edge_shader);
	m_stage->getCanvas()->resetViewport();
	m_stage->Refresh();
}

void ImageProcessCMPT::OnSetBlend(wxScrollEvent& event)
{
	if (m_edge_shader) {
		d2d::ShaderMgr::Instance()->sprite();
		m_edge_shader->SetBlend(event.GetInt() * 0.01f);
		m_stage->Refresh();
	}
}

void ImageProcessCMPT::OnRelief(wxCommandEvent& event)
{
	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	ReliefShader* shader = new ReliefShader;
	shader->Load();
	shader_mgr->SetSpriteShader(shader);
	m_stage->getCanvas()->resetViewport();
	m_stage->Refresh();
}

}