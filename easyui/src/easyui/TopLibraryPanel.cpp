#include "TopLibraryPanel.h"

#include <easycomplex.h>
#include <easytext.h>

namespace eui
{

TopLibraryPanel::TopLibraryPanel(wxWindow* parent)
	: wxPanel(parent)
{
	InitLayout();
	EnableUILibrary(true);
}

void TopLibraryPanel::EnableUILibrary(bool enable)
{
	wxSizer* sizer = GetSizer();
	if (enable) {
		sizer->Show((size_t)0);
		sizer->Hide((size_t)1);
	} else {
		sizer->Show((size_t)1);
		sizer->Hide((size_t)0);
	}
}

void TopLibraryPanel::Clear()
{
	m_library_ui->Clear();
	m_library_raw->Clear();
}

void TopLibraryPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_library_ui = new d2d::LibraryPanel(this);
	sizer->Add(m_library_ui, 1, wxEXPAND);

	m_library_raw = new d2d::LibraryPanel(this);
	wxWindow* nb = m_library_raw->GetNotebook();
	m_library_raw->AddPage(new d2d::LibraryImagePage(nb));
	m_library_raw->AddPage(new ecomplex::LibraryPage(nb));
	m_library_raw->AddPage(new etext::LibraryPage(nb));
//	m_library_raw->AddPage(new d2d::LibraryFontBlankPage(nb));
	sizer->Add(m_library_raw, 1, wxEXPAND);

	SetSizer(sizer);

	Layout();
}

}