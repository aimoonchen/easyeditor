
#ifndef ESHAPE_FRAME_H
#define ESHAPE_FRAME_H

#include <wx/wx.h>

namespace eshape
{
	class Task;

	class Frame : public wxFrame
	{
	public:
		Frame(const wxString& title);

	private:
		void onNew(wxCommandEvent& event);
		void onOpen(wxCommandEvent& event);
		void onSave(wxCommandEvent& event);
		void onSaveAs(wxCommandEvent& event);

		void onQuit(wxCommandEvent& event);

		void onCodeLove2d(wxCommandEvent& event);

		void initMenuBar();

		wxMenu* initFileBar();
		wxMenu* initCodesBar();
		wxMenu* initHelpBar();

		void clear();

		void setCurrFilename();

	private:
		Task* m_task;

		wxString m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame
}

#endif // ESHAPE_FRAME_H