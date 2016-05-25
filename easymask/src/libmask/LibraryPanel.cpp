#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

namespace emask
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(m_base_page = new LibraryPage(nb, "�ײ�"));
	AddPage(m_mask_page = new LibraryPage(nb, "���ֲ�"));
}

}