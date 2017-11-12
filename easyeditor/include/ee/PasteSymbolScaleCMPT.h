#ifndef _EASYEDITOR_PASTE_SYMBOL_SCALE_CMPT_H_
#define _EASYEDITOR_PASTE_SYMBOL_SCALE_CMPT_H_

#include "EditCMPT.h"

namespace ee
{

class LibraryPanel;

class PasteSymbolScaleCMPT : public EditCMPT
{
public:
	PasteSymbolScaleCMPT(wxWindow* parent, const std::string& name,
		wxWindow* stage_wnd, EditPanelImpl* stage, 
		LibraryPanel* library);

protected:
	virtual wxSizer* InitLayout() override;

private:
	void onChangeScale(wxSpinEvent& event);

private:
	float m_scaleVal;

}; // PasteSymbolScaleCMPT

}

#endif // _EASYEDITOR_PASTE_SYMBOL_SCALE_CMPT_H_