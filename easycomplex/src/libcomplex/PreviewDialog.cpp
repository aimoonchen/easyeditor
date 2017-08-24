#include "PreviewDialog.h"
#include "PreviewStage.h"
#include "PreviewCanvas.h"
#include "PreviewEditOP.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/Visitor.h>
#include <ee/Blackboard.h>

#include <easyanim.h>

namespace ecomplex
{

class InitVisitor : public ee::Visitor<ee::Sprite>
{
public:
	virtual void Visit(ee::Sprite* spr, bool& next) {
		if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(spr)) {
			anim->SetLoop(false);
		}
	}
}; // InitVisitor

PreviewDialog::PreviewDialog(wxWindow* parent, wxGLContext* glctx,
							 const std::vector<ee::Sprite*>& sprs)
	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_sprs(sprs)
	, m_control(0.033f)
{
	InitLayout(glctx);

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	data.particle3d_loop = false;

	InitVisitor init;
	bool next;
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		ee::Sprite* spr = m_sprs[i];
		init.Visit(spr, next);
		dynamic_cast<ee::Symbol*>(spr->GetSymbol())->Traverse(init);
	}

	ee::Blackboard::Instance()->visible_audio = false;
}

PreviewDialog::~PreviewDialog()
{
	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	data.particle3d_loop = true;

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		if (const libanim::Sprite* anim = dynamic_cast<const libanim::Sprite*>(m_sprs[i])) {
			const_cast<libanim::Sprite*>(anim)->SetLoop(true);
		}
	}

	ee::Blackboard::Instance()->visible_audio = true;
}

void PreviewDialog::InitLayout(wxGLContext* glctx)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	BuildEditPanel(sizer, glctx);
	SetSizer(sizer);
}

void PreviewDialog::BuildEditPanel(wxSizer* sizer, wxGLContext* glctx)
{
	PreviewStage* stage = new PreviewStage(this, this, m_control);
	stage->SetEditOP(new PreviewEditOP(stage, stage->GetStageImpl(), m_sprs));
	stage->SetCanvas(new PreviewCanvas(stage, stage->GetStageImpl(), m_sprs, glctx));
	sizer->Add(stage, 1, wxEXPAND);
}

}