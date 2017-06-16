#include "Task.h"
#include "StagePanel.h"
#include "FileIO.h"
#include "Context.h"
#include "TreeCtrl.h"
#include "Database.h"

#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>
#include <ee/panel_msg.h>

#include <wx/splitter.h>

namespace edb
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

void Task::LoadFromFile(const char* filename)
{
	if (!ee::FileHelper::IsFileExist(filename)) {
		throw ee::Exception("File: %s don't exist!", filename);
	}

//	FileIO::Load(filename);

	Context* ctx = Context::Instance();
	ctx->stage->GetDB()->Load(filename);
	ctx->tree->Build(*ctx->stage->GetDB());
}

void Task::StoreToFile(const char* filename) const
{
//	FileIO::Store(filename);

	Context::Instance()->stage->GetDB()->Store(filename);
}

void Task::Clear()
{
	ee::ClearPanelSJ::Instance()->Clear();
}

void Task::InitLayout()
{
	wxSplitterWindow* splitter = new wxSplitterWindow(m_parent);

	Context* context = Context::Instance();

	TreeCtrl* tree = context->tree = new TreeCtrl(splitter);
	StagePanel* stage = context->stage = new StagePanel(splitter, m_parent, NULL);

	splitter->SetSashGravity(0.4f);
	splitter->SplitVertically(tree, stage);
}

}