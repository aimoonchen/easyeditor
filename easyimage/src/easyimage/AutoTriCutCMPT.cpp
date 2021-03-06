#include "AutoTriCutCMPT.h"
#include "AutoTriCutOP.h"
#include "StagePanel.h"

#include <easyimage.h>

#include <ee/ImageSprite.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageData.h>
#include <ee/Image.h>
#include <ee/FileHelper.h>
#include <ee/panel_msg.h>

#include <gum/JsonSerializer.h>

#include <fstream>

namespace eimage
{

static const float AREA_TOLERANCE = 0.04f;
static const float PERIMETER_TOLERANCE = 0.2f;

#define TRIGGER_STEP		// step by step

AutoTriCutCMPT::AutoTriCutCMPT(wxWindow* parent, const std::string& name, 
						 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
	, m_raw(NULL)
	, m_fine(NULL)
{
	m_editop = std::make_shared<AutoTriCutOP>(stage, stage->GetStageImpl());
}

wxSizer* AutoTriCutCMPT::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Trigger"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoTriCutCMPT::Trigger));
		top_sizer->Add(btn);
	}
	top_sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Output Outline"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoTriCutCMPT::OutputOutline));
		top_sizer->Add(btn);
	}
	top_sizer->AddSpacer(20);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("step"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, wxT("Create Outline"));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(AutoTriCutCMPT::CreateOutline));
			sizer->Add(btn);
		}
		sizer->AddSpacer(5);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, wxT("Reduce Count"));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(AutoTriCutCMPT::ReduceOutlineCount));
			sizer->Add(btn);
		}
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(20);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("debug"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Trigger"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoTriCutCMPT::OnDebug));
		sizer->Add(btn);
		top_sizer->Add(sizer);
	}
	return top_sizer;
}

void AutoTriCutCMPT::Trigger(wxCommandEvent& event)
{
	Trigger();
}

void AutoTriCutCMPT::OutputOutline(wxCommandEvent& event)
{
	Trigger();

	const ee::SprConstPtr& spr = m_stage->GetImage();
	const std::shared_ptr<ee::ImageSymbol>& sym = std::dynamic_pointer_cast<const ee::ImageSymbol>>(spr->GetSymbol());
	assert(sym);
	const ee::Image* img = sym->GetImage();

	Json::Value value;
	auto op = std::dynamic_pointer_cast<AutoTriCutOP>(m_editop);

	sm::vec2 offset;
	offset.x = img->GetOriginSize().x * -0.5f;
	offset.y = img->GetOriginSize().y * -0.5f;
	std::vector<sm::vec2> vertices(op->m_fine_bound_line);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		vertices[i] += offset;
	}
	gum::JsonSerializer::Store(vertices, value["normal"]);

	std::string filepath = ee::FileHelper::GetFilenameAddTag(img->GetFilepath(), 
		OUTLINE_FILE_TAG, "json");
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void AutoTriCutCMPT::CreateOutline(wxCommandEvent& event)
{
	// step by step
	static int max_step = 5;

	const ee::SprConstPtr& spr = m_stage->GetImage();
	const std::shared_ptr<ee::ImageSymbol>& sym = std::dynamic_pointer_cast<const ee::ImageSymbol>>(spr->GetSymbol());
	assert(sym);
	auto img_data = ee::ImageDataMgr::Instance()->GetItem(sym->GetFilepath());

	auto op = std::dynamic_pointer_cast<AutoTriCutOP>(m_editop);
	m_raw = new ExtractOutlineRaw(img_data->GetPixelData(), img_data->GetWidth(), img_data->GetHeight());
	m_raw->CreateBorderLineAndMerge();
	op->m_raw_bound_line = m_raw->GetBorderLine();
	op->m_raw_bound_points = m_raw->GetBorderPoints();
	op->m_raw_bound_line_merged = m_raw->GetBorderLineMerged();

	m_fine = new ExtractOutlineFine(op->m_raw_bound_line, op->m_raw_bound_line_merged);
	m_fine->CreateOutline(AREA_TOLERANCE, PERIMETER_TOLERANCE, max_step++);
	op->m_fine_bound_line = m_fine->GetResult();

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void AutoTriCutCMPT::ReduceOutlineCount(wxCommandEvent& event)
{
	if (m_fine)
	{
		m_fine->ReduceOutlineCount(AREA_TOLERANCE, PERIMETER_TOLERANCE);
		auto op = std::dynamic_pointer_cast<AutoTriCutOP>(m_editop);
		op->m_fine_bound_line = m_fine->GetResult();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void AutoTriCutCMPT::Trigger()
{
#ifdef TRIGGER_STEP
	static int max_step = 5;
#endif
	const ee::SprConstPtr& spr = m_stage->GetImage();
	const std::shared_ptr<ee::ImageSymbol>& sym = std::dynamic_pointer_cast<const ee::ImageSymbol>>(spr->GetSymbol());
	assert(sym);
	auto img_data = ee::ImageDataMgr::Instance()->GetItem(sym->GetFilepath());

	auto op = std::dynamic_pointer_cast<AutoTriCutOP>(m_editop);
	ExtractOutlineRaw raw(img_data->GetPixelData(), img_data->GetWidth(), img_data->GetHeight());
	raw.CreateBorderLineAndMerge();
	op->m_raw_bound_line = raw.GetBorderLine();
	op->m_raw_bound_points = raw.GetBorderPoints();
	op->m_raw_bound_line_merged = raw.GetBorderLineMerged();

	ExtractOutlineFine fine(op->m_raw_bound_line, op->m_raw_bound_line_merged);
#ifdef TRIGGER_STEP
	fine.Trigger(AREA_TOLERANCE, PERIMETER_TOLERANCE, max_step++);
#else
	fine.Trigger(AREA_TOLERANCE, PERIMETER_TOLERANCE);
#endif
	op->m_fine_bound_line = fine.GetResult();

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void AutoTriCutCMPT::OnDebug(wxCommandEvent& event)
{
	const ee::SprConstPtr& spr = m_stage->GetImage();
	const std::shared_ptr<ee::ImageSymbol>& sym = std::dynamic_pointer_cast<const ee::ImageSymbol>>(spr->GetSymbol());
	assert(sym);
	auto img_data = ee::ImageDataMgr::Instance()->GetItem(sym->GetFilepath());

	auto op = std::dynamic_pointer_cast<AutoTriCutOP>(m_editop);
	ExtractOutlineRaw raw(img_data->GetPixelData(), img_data->GetWidth(), img_data->GetHeight());
	raw.CreateBorderLineAndMerge();
	raw.CreateBorderConvexHull();
	op->m_raw_bound_line = raw.GetConvexHull();
	op->m_raw_bound_points = raw.GetBorderPoints();
	op->m_raw_bound_line_merged = raw.GetBorderLineMerged();
}

}