#include "StagePanel.h"

#include <ee0/StagePage.h>

namespace eone
{

StagePanel::StagePanel(wxWindow* parent)
	: wxAuiNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER)
	, m_last_page(nullptr)
{
	Connect(GetId(), wxEVT_AUINOTEBOOK_PAGE_CHANGING, 
		wxAuiNotebookEventHandler(StagePanel::OnPageChanging));
	Connect(GetId(), wxEVT_AUINOTEBOOK_PAGE_CHANGED,
		wxAuiNotebookEventHandler(StagePanel::OnPageChanged));
}

ee0::StagePage* StagePanel::GetCurrentStagePage() const
{
	return dynamic_cast<ee0::StagePage*>(GetCurrentPage());
}

void StagePanel::OnPageChanging(wxAuiNotebookEvent& event)
{
	auto page = GetCurrentStagePage();
	if (page) {
		m_last_page = page;
	}
}

void StagePanel::OnPageChanged(wxAuiNotebookEvent& event)
{
	if (!m_last_page) {
		return;
	}

	auto page = GetCurrentStagePage();
	GD_ASSERT(page, "null page");

	ee0::VariantSet vars;

	ee0::Variant var;
	var.m_type = ee0::VT_PVOID;
	var.m_val.pv = &page->GetSubjectMgr();
	vars.SetVariant("sub_mgr", var);

	m_last_page->GetSubjectMgr().NotifyObservers(ee0::MSG_STAGE_PAGE_CHANGING, vars);

	m_last_page = nullptr;
}

}