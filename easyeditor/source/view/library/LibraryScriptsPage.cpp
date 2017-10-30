#include "LibraryScriptsPage.h"
#include "LibraryList.h"
#include "ExceptionDlg.h"
#include "ScriptsSymbol.h"
#include "SymbolMgr.h"
#include "SymbolFile.h"
#include "SymbolType.h"
#include "Exception.h"
#include "StageCanvas.h"

namespace ee
{

LibraryScriptsPage::LibraryScriptsPage(wxWindow* parent)
	: LibraryPage(parent, "Scripts")
{
	InitLayout();
	m_list->SetFileter("scripts");
}

bool LibraryScriptsPage::IsHandleSymbol(const SymPtr& sym) const
{
	return sym->Type() == SYM_SCRIPTS;
}

bool LibraryScriptsPage::LoadFromConfig()
{
	return LibraryPage::LoadFromConfig("library_script");
}

void LibraryScriptsPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter = SymbolFile::Instance()->Tag(SYM_SCRIPTS);
	filter = wxT("*_") + filter + wxT(".lua");
	wxFileDialog dlg(this, wxT("����scripts�ļ�"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			try {
				std::string filepath = filenames[i].ToStdString();
				auto sym = SymbolMgr::Instance()->FetchSymbol(filepath);
				m_list->Insert(sym);
			} catch (Exception& e) {
				ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}