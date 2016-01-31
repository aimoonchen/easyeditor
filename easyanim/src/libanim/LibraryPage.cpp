#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

#include <easycomplex.h>
#include <easycoco.h>

namespace libanim
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, wxT("Anim"))
{
	InitLayout();
	m_list->SetFileter(libanim::FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter = ee::FileType::GetTag(ee::FileType::e_anim);
	filter = "*_" + filter + ".json";
	filter += "; *.lua";
	wxFileDialog dlg(this, wxT("����anim�ļ�"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const std::string filename = filenames[i];
			std::string type = filename.substr(filename.find_last_of(".") + 1);
			ee::StringHelper::ToLower(type);
			try {
				if (type == "json") {
					loadFromJsonFile(filename);
				} else if (type == "lua") {
					loadFromLuaFile(filename);
				}
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

void LibraryPage::loadFromJsonFile(const std::string& filename)
{
	ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filename);
	symbol->RefreshThumbnail(filename);
	m_list->Insert(symbol);
	symbol->Release();
}

void LibraryPage::loadFromLuaFile(const std::string& filename)
{
 	std::vector<std::string> texfilenames;
 	std::string name = filename.substr(0, filename.find_last_of("."));
 	texfilenames.push_back(name + ".1.ppm");
 
	libcoco::epe::ParserLuaFile parser;
 	parser.parser(filename);
 	parser.transToMemory(texfilenames);
 
 	std::vector<ee::Symbol*> symbols;
 	parser.getAllSymbols(symbols);
 	for (int i = 0, n = symbols.size(); i < n; ++i)
 		if (IsHandleSymbol(symbols[i]))
 			m_list->Insert(symbols[i]);
}

}