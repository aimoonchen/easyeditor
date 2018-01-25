#include "Task.h"

#include <easyicon.h>

#include <ee/SymbolMgr.h>
#include <ee/panel_msg.h>

namespace eicon
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
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
	auto icon = FileIO::LoadFromFile(filename);
	m_toolbar->ChangeIconType(get_icon_type(
		dynamic_cast<eicon::Icon*>(icon.get())->GetIconDesc()));
	m_stage->GetSymbol()->SetIcon(icon);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void Task::Store(const char* filename) const
{
	auto icon = dynamic_cast<const Icon*>(m_stage->GetSymbol()->GetIcon().get());
	FileIO::StoreToFile(filename, *icon);
}

bool Task::IsDirty() const
{
	return false;
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	m_library = new LibraryPanel(left_splitter);
	m_stage = new StagePanel(left_splitter, m_parent, m_library);

	left_splitter->SetSashGravity(0.2f);
	left_splitter->SplitVertically(m_library, m_stage);

	m_toolbar = new ToolbarPanel(right_splitter, static_cast<StagePanel*>(m_stage));
	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, m_toolbar);

	m_root = right_splitter;
}

}