#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/Sprite.h>
#include <ee/SymbolMgr.h>
#include <ee/EditOP.h>
#include <ee/Symbol.h>
#include <ee/LibraryPanel.h>
#include <ee/SpriteFactory.h>
#include <ee/panel_msg.h>
#include <ee/ImageSymbol.h>
#include <ee/Image.h>

namespace eimage
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, m_image(NULL)
{
	m_left = m_right = NULL;

	SetCanvas(new StageCanvas(this));
	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::~StagePanel()
{
	if (m_image) {
		m_image->RemoveReference();
	}
}

void StagePanel::SetImage(const std::string& filepath)
{
	if (m_image) {
		m_image->RemoveReference();
	}

	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	if (sym) {
		SetImage(sym);
	}
}

void StagePanel::SetImage(const ee::SymPtr& sym)
{
	ee::ImageSymbol* img_sym = static_cast<ee::ImageSymbol*>(sym);
	const sm::i16_vec2& ori_sz = img_sym->GetImage()->GetOriginSize();
	const sm::i16_rect& clip_r = img_sym->GetImage()->GetClippedRegion();

	sm::vec2 offset;
	offset.x = ori_sz.x * 0.5f - clip_r.xmin;
	offset.y = ori_sz.y * 0.5f - clip_r.ymin;

	auto spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->Translate(offset);
	m_image = spr;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	GetEditOP()->Clear();
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, ee::LibraryPanel* library)
	: ee::CombinedDropTarget(stage)
	, m_stage(stage)
	, m_library(library)
{
}

void StagePanel::StageDropTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	wxString sType = text.substr(0, text.find(","));
	wxString sIndex = text.substr(text.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	ee::Symbol* sym = m_library->GetSymbol(index);
	if (sym) {
		m_stage->SetImage(sym);
	}

// 	// todo for diff
// 	// fixme
// 	sm::vec2 pos = m_stage->transPosScreenToProject(x, y);
// 	auto spr = ee::SpriteFactory::Instance()->Create(sym);
// 	sm::rect r = spr->getSymbol().getSize();
// 	if (pos.x < 0) {
// 		spr->setTransform(sm::vec2(-r.Width() * 0.5f - 10, 0.0f), 0);
// 		m_stage->m_left = spr;
// 	} else {
// 		spr->setTransform(sm::vec2(r.Width() * 0.5f + 10, 0.0f), 0);
// 		m_stage->m_right = spr;
// 	}
}

void StagePanel::StageDropTarget::
OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if (filenames.IsEmpty()) {
		return;
	}

	std::string filename = filenames[0];
	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
	m_stage->SetImage(sym);
}

}