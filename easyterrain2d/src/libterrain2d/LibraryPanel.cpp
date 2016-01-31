#include "LibraryPanel.h"

#include <wx/notebook.h>

namespace eterrain2d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
}

}