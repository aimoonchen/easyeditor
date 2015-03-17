#include "Task.h"

#include <easypseudo3d.h>

namespace epseudo3d
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filename)
{
	epseudo3d::StageCanvas3D* canvas = static_cast<epseudo3d::StageCanvas3D*>(m_stage->getCanvas());
	canvas->LoadScene(filename);
}

void Task::store(const char* filename) const
{
	epseudo3d::StageCanvas3D* canvas = static_cast<epseudo3d::StageCanvas3D*>(m_stage->getCanvas());
	canvas->StoreScene(filename);
}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
// 	m_library->clear();
// 	m_stage->clear();
// 	m_library->Refresh();
// 	m_stage->Refresh();
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

// void Task::InitLayout()
// {
// 	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
// 	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);
// 
// 	m_library = new LibraryPanel(left_splitter);
// 	m_stage = new StagePanel(left_splitter, m_parent, m_library);
// 	m_library->setCanvas(m_stage->getCanvas());
// 
// 	left_splitter->SetSashGravity(0.2f);
// 	left_splitter->SplitVertically(m_library, m_stage);
// 
// 	ToolbarPanel* toolbar = new ToolbarPanel(right_splitter, static_cast<StagePanel*>(m_stage));
// 	right_splitter->SetSashGravity(0.85f);
// 	right_splitter->SplitVertically(left_splitter, toolbar);
// 
// 	m_root = right_splitter;
// }

void Task::InitLayout()
{
	m_stage = new d2d::EditPanel(m_parent, m_parent);

//	m_stage->setCanvas(new epseudo3d::StageCanvas3D(m_stage));
//	m_stage->setEditOP(new e3d::ControlCameraOP(m_stage));

	m_stage->setCanvas(new epseudo3d::StageCanvas2D(m_stage));
	m_stage->setEditOP(new epseudo3d::Proj2DEditOP(m_stage));
}

}