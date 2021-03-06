#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

namespace etext
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Text")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_TEXTBOX));
}

bool LibraryPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return sym->Type() == s2::SYM_TEXTBOX;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string type = ee::SymbolFile::Instance()->Tag(s2::SYM_TEXTBOX);
	ee::LibraryPage::OnAddPress(type);
}

}