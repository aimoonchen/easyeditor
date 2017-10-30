#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <sprite2/SymType.h>

namespace etexture
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Texture")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_TEXTURE));
}

bool LibraryPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return sym->Type() == s2::SYM_TEXTURE;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = wxT("*_") + ee::SymbolFile::Instance()->Tag(s2::SYM_TEXTURE) + wxT(".json");
	wxFileDialog dlg(this, wxT("����texture�ļ�"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filename = filenames[i].ToStdString();
			try {
				auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
				sym->RefreshThumbnail(filename);
				m_list->Insert(sym);
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}