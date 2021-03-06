#include "Task.h"
#include "UIStagePage.h"
#include "TopStagePanel.h"
#include "TopLibraryPanel.h"
#include "TopToolbarPanel.h"
#include "Frame.h"

#include <ee/SymbolMgr.h>
#include <ee/PropertySettingPanel.h>
#include <ee/ViewlistPanel.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <fstream>

namespace eui
{

Task::Task(Frame* frame)
	: m_root(NULL)
	, m_frame(frame)
{
	InitLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filename)
{
	UIStagePage* stage = NULL;
	int type = ee::SymbolFile::Instance()->Type(filename);
	if (type == ee::SYM_UIWND) 
	{
		stage = m_top_pannels.stage->SetPage(TopStagePanel::PAGE_WND);
	} 
	else 
	{
		assert(type == ee::SYM_UI);

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filename);
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		std::string stype = value["type"].asString();
		if (stype == "list") {
			stage = m_top_pannels.stage->SetPage(TopStagePanel::PAGE_LIST);
		} else {
			stage = m_top_pannels.stage->SetPage(TopStagePanel::PAGE_WRAPPER);
		}
	}
	if (stage) {
		stage->LoadFromFile(filename);
	}
}

void Task::Store(const char* filename) const
{
	UIStagePage* stage = m_top_pannels.stage->GetSelectedPage();
	if (stage) {
		stage->StoreToFile(filename);
	}
}

const ee::EditPanel* Task::GetEditPanel() const 
{ 
	return m_top_pannels.stage->GetSelectedPage();
}

UIStagePage* Task::GetSelectedStagePage()
{
	return m_top_pannels.stage->GetSelectedPage();
}

void Task::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(m_frame);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* right = InitLayoutRight(right_split);
	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);

	left_split->SetSashGravity(0.12f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_top_pannels.library = new TopLibraryPanel(split);
	m_top_pannels.property = new ee::PropertySettingPanel(split);

	split->SetSashGravity(0.6f);
	split->SplitHorizontally(m_top_pannels.library, m_top_pannels.property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_top_pannels.stage = new TopStagePanel(parent, m_frame, &m_top_pannels);
	return m_top_pannels.stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_top_pannels.viewlist = new ee::ViewlistPanel(split);
	m_top_pannels.toolbar = new TopToolbarPanel(split);

	split->SetSashGravity(0.8f);
	split->SplitHorizontally(m_top_pannels.viewlist, m_top_pannels.toolbar);

	return split;
}

}