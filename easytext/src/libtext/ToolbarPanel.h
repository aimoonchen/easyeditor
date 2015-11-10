#ifndef _EASYTEXT_TOOLBAR_PANEL_H_
#define _EASYTEXT_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace etext
{

class StagePanel;
class Sprite;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage, Sprite* spr);
	virtual ~ToolbarPanel();

protected:
	virtual wxSizer* initLayout();

private:
	void OnChangeSize(wxCommandEvent& event);

	void OnChangeFont(wxCommandEvent& event);
	void OnChangeFontSize(wxCommandEvent& event);
	void OnChangeColor(wxCommandEvent& event);

	void OnChangeAlign(wxCommandEvent& event);

private:
	Sprite* m_spr;

	wxTextCtrl *m_width, *m_height;	

	wxChoice* m_font;
	wxTextCtrl* m_font_size;

	wxCheckBox* m_edge;

	wxChoice *m_align_hori, *m_align_vert;

	wxChoice* m_tid;

}; // ToolbarPanel

}

#endif // _EASYTEXT_TOOLBAR_PANEL_H_