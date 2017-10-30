#ifndef _EASYSCALE9_TOOLBAR_PANEL_H_
#define _EASYSCALE9_TOOLBAR_PANEL_H_

#include <wx/wx.h>

#include <memory>

namespace ee { class EditPanel; }

namespace escale9
{

class Symbol;

class ToolbarPanel : public wxPanel
{
public:
	ToolbarPanel(wxWindow* parent, ee::EditPanel* stage,
		const std::shared_ptr<Symbol>& sym);

private:
	void InitLayout();

	void onChangeSize(wxSpinEvent& event);

private:
	ee::EditPanel* m_stage;

	std::shared_ptr<Symbol> m_sym;

	wxSpinCtrl *m_wSpin, *m_hSpin;

}; // ToolbarPanel

}

#endif // _EASYSCALE9_TOOLBAR_PANEL_H_