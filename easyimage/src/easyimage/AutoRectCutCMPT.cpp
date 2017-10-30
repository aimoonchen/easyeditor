#include "AutoRectCutCMPT.h"
#include "AutoRectCutOP.h"
#include "StagePanel.h"

#include <easyimage.h>

#include <ee/Sprite.h>
#include <ee/ImageSymbol.h>
#include <ee/panel_msg.h>
#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/ImageData.h>

#include <gimg_typedef.h>
#include <gimg_export.h>
#include <pimg/Cropping.h>

namespace eimage
{

AutoRectCutCMPT::AutoRectCutCMPT(wxWindow* parent, const std::string& name, 
								 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_editop = new AutoRectCutOP(stage, stage->GetStageImpl());
}

wxSizer* AutoRectCutCMPT::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

//	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Add Rect"));
//	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
// 	{
// 		wxSizer* input_sizer = new wxBoxSizer(wxVERTICAL);
// 
// 		wxString choices[5] = {"8", "16", "32", "64", "128"};
// 
// 		wxSizer* width_sizer = new wxBoxSizer(wxHORIZONTAL);
// 		width_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Width: ")));
// 		m_width_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choices);
// 		m_width_choice->SetSelection(3);
// 		width_sizer->Add(m_width_choice);
// 		input_sizer->Add(width_sizer);
// 
// 		wxSizer* height_sizer = new wxBoxSizer(wxHORIZONTAL);
// 		height_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Height: ")));
// 		m_height_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choices);
// 		m_height_choice->SetSelection(3);
// 		height_sizer->Add(m_height_choice);
// 		input_sizer->Add(height_sizer);
// 
// 		sizer->Add(input_sizer);
// 	}
// 	sizer->AddSpacer(10);
// 	{
// 		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Add"));
// 		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
// 			wxCommandEventHandler(AutoRectCutCMPT::OnAddRect));
// 		sizer->Add(btn);
// 	}
//	sizer->AddSpacer(20);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Auto"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoRectCutCMPT::OnCreateRects));
		top_sizer->Add(btn);
	}
	top_sizer->AddSpacer(20);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Output"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoRectCutCMPT::OnOutputRects));
		top_sizer->Add(btn);		
	}

	return top_sizer;
}

// void AutoRectCutCMPT::OnAddRect(wxCommandEvent& event)
// {
// 	int width = wxVariant(m_width_choice->GetString(m_width_choice->GetSelection())).GetInteger();
// 	int height = wxVariant(m_height_choice->GetString(m_height_choice->GetSelection())).GetInteger();
// 
// 	AutoRectCutOP* op = static_cast<AutoRectCutOP*>(m_editop);
// 	op->getRectMgr().insert(sm::rect(sm::vec2(0, 0), sm::vec2((float)width, (float)height)));
// 
// 	m_stage->Refresh();
// }

void AutoRectCutCMPT::OnCreateRects(wxCommandEvent& event)
{
	const ee::SymPtr& sym = std::dynamic_pointer_cast<ee::Symbol>(m_stage->GetImage()->GetSymbol());
	auto img = ee::ImageDataMgr::Instance()->GetItem(sym->GetFilepath());

	RegularRectCut cut(img->GetPixelData(), img->GetWidth(), img->GetHeight());
	cut.AutoCut();

	const std::vector<Rect>& result = cut.GetResult();
	RectMgr& rects = static_cast<AutoRectCutOP*>(m_editop)->getRectMgr();
	for (int i = 0, n = result.size(); i < n; ++i) {
		int x = result[i].x,
			y = result[i].y,
			w = result[i].w,
			h = result[i].h;
		rects.Insert(sm::rect(sm::vec2(x, y), sm::vec2(x+w, y+h)));
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	std::string msg = ee::StringHelper::Format("Left: %d, Used: %d", cut.GetLeftArea(), cut.GetUseArea());
	wxMessageBox(msg, wxT("Statics"), wxOK | wxICON_INFORMATION, this);

	img->RemoveReference();
}

void AutoRectCutCMPT::OnOutputRects(wxCommandEvent& event)
{
	const ee::SymPtr& sym = std::dynamic_pointer_cast<ee::Symbol>(m_stage->GetImage()->GetSymbol());
	auto img = ee::ImageDataMgr::Instance()->GetItem(sym->GetFilepath());

	RegularRectCut cut(img->GetPixelData(), img->GetWidth(), img->GetHeight());
	cut.AutoCut();

	std::string msg = ee::StringHelper::Format("Left: %d, Used: %d", cut.GetLeftArea(), cut.GetUseArea());
	wxMessageBox(msg, wxT("Statics"), wxOK | wxICON_INFORMATION, this);

	assert(img->GetFormat() == GPF_RGB || img->GetFormat() == GPF_RGBA8);
	int channels = img->GetFormat() == GPF_RGB ? 3 : 4;
	pimg::Cropping crop(img->GetPixelData(), img->GetWidth(), img->GetHeight(), channels);

	std::string ori_path = ee::FileHelper::GetFilePathExceptExtension(img->GetFilepath());
	const std::vector<Rect>& result = cut.GetResult();
	for (int i = 0, n = result.size(); i < n; ++i) 
	{
		const eimage::Rect& r = result[i];
		uint8_t* pixels = crop.Crop(r.x, r.y, r.x+r.w, r.y+r.h);

		std::string out_path = ee::StringHelper::Format("%s#%d#%d#%d#%d#.png", ori_path.c_str(), r.x, r.y, r.w, r.h);
		gimg_export(out_path.c_str(), pixels, r.w, r.h, GPF_RGBA8, true);
		delete[] pixels;
	}

	img->RemoveReference();
}

}