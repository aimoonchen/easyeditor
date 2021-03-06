#include "LibraryPanel.h"
#include "LibraryPage.h"
#include "LibraryList.h"
#include "LibraryImagePage.h"
#include "LibraryPanelDropTarget.h"
#include "SymbolMgr.h"
#include "ImageSymbol.h"
#include "panel_msg.h"
#include "FetchAllVisitor.h"
#include "subject_id.h"

namespace ee
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_selected(NULL)
{
	InitLayout();

//	SetDropTarget(new LibraryPanelDropTarget(this));

	RegistSubject(ClearPanelSJ::Instance());
}

void LibraryPanel::OnPageChanged(wxBookCtrlEvent& event)
{
	m_selected = m_pages[event.GetSelection()];
	m_selected->OnActive();
}

void LibraryPanel::OnPageChanging(wxBookCtrlEvent& event)
{
}

void LibraryPanel::Clear()
{
	bool dirty = false;
	for (size_t i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->Clear();
		dirty = true;
	}
	LoadFromConfig();

	if (dirty) {
		Refresh(true);
	}
}

void LibraryPanel::ReloadTexture() const
{
	for (size_t i = 0, n = m_pages.size(); i < n; ++i)
		m_pages[i]->ReloadTexture();
}

void LibraryPanel::AddPage(LibraryPage* page, const char* name)
{
	if (m_notebook->GetPageCount() == 0)
		m_selected = page;

	if (name != NULL) {
		m_notebook->AddPage(page, name);
	} else {
		m_notebook->AddPage(page, page->GetPageName().c_str());
	}
	m_pages.push_back(page);
}

SymPtr LibraryPanel::GetSymbol(int index/* = -1*/) const
{
	if (m_selected)
		return m_selected->GetSymbol(index);
	else
		return nullptr;
}

void LibraryPanel::LoadFromSymbolMgr(const SymbolMgr& mgr)
{
	std::vector<SymPtr> syms;
	mgr.Traverse(FetchAllRefVisitor<Symbol>(syms));
	for (size_t i = 0, n = syms.size(); i < n; ++i) {
		LoadSymbol(syms[i]);
	}
}

void LibraryPanel::LoadSymbol(const SymPtr& sym)
{
	for (size_t j = 0, m = m_pages.size(); j < m; ++j)
	{
		LibraryPage* page = m_pages[j];
		if (page->IsHandleSymbol(sym))
		{
			sym->RefreshThumbnail(sym->GetFilepath());
			page->GetList()->Insert(sym);
			break;
		}
	}
}

void LibraryPanel::Traverse(RefVisitor<ListItem>& visitor) const
{
	if (m_selected)
		m_selected->Traverse(visitor);
}

bool LibraryPanel::AddSymbol(const SymPtr& sym)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) 
	{
		LibraryPage* page = m_pages[i];
		if (page->IsHandleSymbol(sym)) {
			page->AddItem(sym);
			return true;
		}
	}
	return false;
}

void LibraryPanel::LoadFromConfig()
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->LoadFromConfig();
	}
}

void LibraryPanel::LoadDefaultSymbol()
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->LoadDefaultSymbol();
	}
}

void LibraryPanel::SetCurrPage(int idx)
{
	if (idx >= 0 && idx < static_cast<int>(m_pages.size())) {
		m_notebook->SetSelection(idx);
	}
}

void LibraryPanel::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_CLEAR_PANEL:
		Clear();
		break;
	}
}

void LibraryPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_notebook = new wxNotebook(this, wxID_ANY);
	Connect(m_notebook->GetId(), wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler(LibraryPanel::OnPageChanged));
	Connect(m_notebook->GetId(), wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, wxBookCtrlEventHandler(LibraryPanel::OnPageChanging));

	sizer->Add(m_notebook, 1, wxEXPAND);

	SetSizer(sizer);
}

}