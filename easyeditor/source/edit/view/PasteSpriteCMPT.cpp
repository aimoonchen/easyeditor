#include "PasteSpriteCMPT.h"
#include "PasteSpriteOP.h"

namespace ee
{

PasteSpriteCMPT::PasteSpriteCMPT(wxWindow* parent, const std::string& name, 
								 wxWindow* stage_wnd, EditPanelImpl* stage, 
								 MultiSpritesImpl* sprites_impl)
	: EditCMPT(parent, name, stage)
{
	m_editop = std::make_shared<PasteSpriteOP>(stage_wnd, stage, sprites_impl, this);
}

wxSizer* PasteSpriteCMPT::InitLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("tools"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	sizer->Add(m_xMirror = new wxCheckBox(this, wxID_ANY, wxT("Horizontal Mirror")));
	sizer->AddSpacer(10);
	sizer->Add(m_yMirror = new wxCheckBox(this, wxID_ANY, wxT("Vertical Mirror")));

	return sizer;
}

}