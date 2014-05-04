
#ifndef ESHAPE_LIBRARY_LIST_H
#define ESHAPE_LIBRARY_LIST_H

#include <drag2d.h>

namespace eshape
{
	class LibraryList : public d2d::LibraryList
	{
	public:
		LibraryList(wxWindow* parent);

		virtual void onListSelected(wxCommandEvent& event);

	}; // LibraryList
}

#endif // ESHAPE_LIBRARY_LIST_H