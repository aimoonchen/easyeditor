#ifndef _DRAG2D_EDIT_DIALOG_H_
#define _DRAG2D_EDIT_DIALOG_H_



namespace ecomplex
{

class Symbol;
class PropertySettingPanel;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, Symbol* symbol, 
		wxGLContext* glctx);

private:
	void InitLayout(wxGLContext* glctx);
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent, wxGLContext* glctx);
	wxWindow* InitLayoutRight(wxWindow* parent);

	void OnCloseEvent(wxCloseEvent& event);

	void LoadSymbolInfo();

private:
	ee::LibraryPanel* m_library;
	PropertySettingPanel* m_property;
	StagePanel* m_stage;
	ee::ViewlistPanel* m_viewlist;

	Symbol* m_symbol;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _DRAG2D_EDIT_DIALOG_H_