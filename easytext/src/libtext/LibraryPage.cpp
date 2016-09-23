#include "LibraryPage.h"
#include "config.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/FileType.h>

namespace etext
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Text")
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return dynamic_cast<Symbol*>(sym) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string type = ee::FileType::GetTag(ee::FILE_TEXT);
	ee::LibraryPage::OnAddPress(type);
}

}