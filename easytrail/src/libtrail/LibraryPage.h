#ifndef _EASYTRAIL_LIBRARY_PAGE_H_
#define _EASYTRAIL_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace etrail
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYTRAIL_LIBRARY_PAGE_H_