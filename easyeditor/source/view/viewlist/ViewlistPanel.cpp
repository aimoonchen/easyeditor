#include "ViewlistPanel.h"
#include "ViewlistList.h"

namespace ee
{

ViewlistPanel::ViewlistPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_list(NULL)
{
	InitLayout();
}

void ViewlistPanel::Clear()
{
	m_list->Clear();
}

void ViewlistPanel::Insert(const SprPtr& spr, int idx)
{
	m_list->Insert(spr, idx);
}

void ViewlistPanel::SetListImpl(ViewlistListImpl* impl)
{
	m_list->SetImpl(impl);	
}

void ViewlistPanel::EnableObserve(bool enable)
{
	m_list->EnableObserve(enable);
}

void ViewlistPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" View List"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);

	m_list = new ViewlistList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

}