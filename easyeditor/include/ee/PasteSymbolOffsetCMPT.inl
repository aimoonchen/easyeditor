#ifndef _EASYEDITOR_PASTE_SYMBOL_OFFSET_CMPT_INL_
#define _EASYEDITOR_PASTE_SYMBOL_OFFSET_CMPT_INL_

#include <wx/spinctrl.h>

namespace ee
{

template <typename TOperation>
PasteSymbolOffsetCMPT<TOperation>::
PasteSymbolOffsetCMPT(wxWindow* parent, const std::string& name,
					  wxWindow* stage_wnd, EditPanelImpl* stage, 
					  MultiSpritesImpl* sprites_impl, 
					  LibraryPanel* library)
	: EditCMPT(parent, name, editPanel)
{
	m_editop = std::make_shared<TOperation>(stage_wnd, stage, sprites_impl, library, this);
}

template <typename TOperation>
sm::vec2 PasteSymbolOffsetCMPT<TOperation>::GetOffset() const
{
	return sm::vec2(m_xspin->GetValue(), m_yspin->GetValue());
}

template <typename TOperation>
wxSizer* PasteSymbolOffsetCMPT<TOperation>::InitLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("�Զ�ƫ��"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxBoxSizer* xSizer = new wxBoxSizer(wxHORIZONTAL);
	xSizer->Add(new wxStaticText(this, wxID_ANY, wxT("ˮƽƫ��: ")));
	m_xspin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -200, 200, 0);
	xSizer->Add(m_xspin);
	sizer->Add(xSizer);

	wxBoxSizer* ySizer = new wxBoxSizer(wxHORIZONTAL);
	ySizer->Add(new wxStaticText(this, wxID_ANY, wxT("��ֱƫ��: ")));
	m_yspin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -200, 200, 0);
	ySizer->Add(m_yspin);
	sizer->Add(ySizer);

	return sizer;
}

}

#endif // _EASYEDITOR_PASTE_SYMBOL_OFFSET_CMPT_INL_