#ifndef _EASYUI_STAGE_PANEL_H_
#define _EASYUI_STAGE_PANEL_H_

#include <drag2d.h>

namespace eui
{

class TopPannels;
class IStagePage;

class StagePanel : public wxPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels);

	void Clear();

private:
	void InitLayout();
	void InitTabPages(wxSizer* sizer);

	void OnPageChanged(wxNotebookEvent& event);

private:
	wxTopLevelWindow* m_frame;

	TopPannels* m_top_pannels;

	wxNotebook* m_notebook;

	std::vector<IStagePage*> m_pages;

	DECLARE_EVENT_TABLE()

}; // StagePanel

}

#endif // _EASYUI_STAGE_PANEL_H_
