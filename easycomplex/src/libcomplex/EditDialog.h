#ifndef _EASYCOMPLEX_EDIT_DIALOG_H_
#define _EASYCOMPLEX_EDIT_DIALOG_H_

#include <wx/dialog.h>

#include <memory>

class wxGLContext;

namespace ee { class LibraryPanel; class ViewlistPanel; class CrossGuides; }

namespace ecomplex
{

class Symbol;
class PropertySettingPanel;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, const std::shared_ptr<Symbol>& sym, 
		wxGLContext* glctx, ee::CrossGuides* guides = NULL);
	virtual ~EditDialog();

private:
	void InitLayout(wxGLContext* glctx, ee::CrossGuides* guides);
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent, wxGLContext* glctx, ee::CrossGuides* guides);
	wxWindow* InitLayoutRight(wxWindow* parent);

	void OnCloseEvent(wxCloseEvent& event);

	void LoadSymbolInfo();

private:
	ee::LibraryPanel* m_library;
	PropertySettingPanel* m_property;
	StagePanel* m_stage;
	ee::ViewlistPanel* m_viewlist;

	std::shared_ptr<Symbol> m_sym;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYCOMPLEX_EDIT_DIALOG_H_