#ifndef _ESHAPE_LIBRARY_LIST_H_
#define _ESHAPE_LIBRARY_LIST_H_



namespace eshape
{

class StagePanel;
class ToolbarPanel;

class LibraryList : public ee::LibraryList
{
public:
	LibraryList(wxWindow* parent);

	virtual void onListSelected(wxCommandEvent& event);

	void setContext(StagePanel* stage, ToolbarPanel* toolbar) {
		m_stage = stage;
		m_toolbar = toolbar;
	}

private:
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // LibraryList

}

#endif // _ESHAPE_LIBRARY_LIST_H_