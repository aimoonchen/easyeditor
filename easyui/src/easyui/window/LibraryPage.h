#ifndef _EASYUI_WINDOW_LIBRARY_PAGE_H_
#define _EASYUI_WINDOW_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eui
{
namespace window
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name, 
		const std::string& filter);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	std::string m_filter;

}; // LibraryPage 

}
}

#endif // _EASYUI_WINDOW_LIBRARY_PAGE_H_