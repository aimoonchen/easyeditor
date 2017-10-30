#ifndef _LIBANIM_PREVIEW_DIALOG_H_
#define _LIBANIM_PREVIEW_DIALOG_H_

#include <wx/dialog.h>

#include <memory>

class wxGLContext;

namespace ee { class EditPanel; }

namespace libanim
{

class Symbol;

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, const std::shared_ptr<Symbol>& sym,
		wxGLContext* glctx);
	~PreviewDialog();

private:
	void InitLayout(wxGLContext* glctx);

	void BuildToolBar(wxSizer* topSizer);
	void BuildEditPanel(wxSizer* topSizer, wxGLContext* glctx);

	void OnSetLoop(wxCommandEvent& event);
	void OnSetStop(wxCommandEvent& event);

private:
	ee::EditPanel* m_stage;

	const std::shared_ptr<Symbol> m_sym;

}; // PreviewDialog

}

#endif // _LIBANIM_PREVIEW_DIALOG_H_