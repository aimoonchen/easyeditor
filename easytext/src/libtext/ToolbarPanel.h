#ifndef _EASYTEXT_TOOLBAR_PANEL_H_
#define _EASYTEXT_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

#include <memory>

namespace etext
{

class StagePanel;
class Sprite;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage, const std::shared_ptr<Sprite>& spr);

protected:
	virtual wxSizer* InitLayout();

private:
	void InitSizeLayout(wxSizer* top_sizer);
	void InitFontLayout(wxSizer* top_sizer);
	void InitLayoutLayout(wxSizer* top_sizer);

	void OnChangeSize(wxCommandEvent& event);

	void OnChangeFont(wxCommandEvent& event);
	void OnChangeFontSize(wxCommandEvent& event);
	void OnChangeFontColor(wxCommandEvent& event);

	void OnChangeEdge(wxCommandEvent& event);
	void OnChangeEdgeSize(wxCommandEvent& event);
	void OnChangeEdgeColor(wxCommandEvent& event);	

	void OnChangeAlign(wxCommandEvent& event);
	void OnChangeSpace(wxCommandEvent& event);

private:
	std::shared_ptr<Sprite> m_spr;

	wxTextCtrl *m_width, *m_height;	

	wxChoice* m_font;
	wxTextCtrl* m_font_size;

	wxCheckBox* m_edge;
	wxTextCtrl* m_edge_size;

	wxChoice *m_align_hori, *m_align_vert;
	wxTextCtrl *m_space_h, *m_space_v;

	wxChoice* m_tid;

}; // ToolbarPanel

}

#endif // _EASYTEXT_TOOLBAR_PANEL_H_