
#include "LibraryPage.h"
#include "LibraryList.h"

using namespace emodeling;

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryImagePage(parent)
{
	m_list = new LibraryList(this);
	InitLayout();
}