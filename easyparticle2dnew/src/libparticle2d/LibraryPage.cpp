#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

namespace eparticle2d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Particle2D"))
{
	initLayout();
	m_list->setFileter(FILE_TAG);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_particle2d);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("����Particle2D�ļ�"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString& filepath = filenames[i];
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->insert(symbol);
			symbol->release();
			m_canvas->resetViewport();
		}
	}
}


}