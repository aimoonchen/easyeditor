#include "Task.h"
#include "FileIO.h"

#include <ee/SymbolMgr.h>
#include <ee/FetchAllVisitor.h>
#include <ee/ViewlistPanel.h>
#include <ee/GroupTreePanel.h>
#include <ee/SymbolType.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>
#include <easycoco.h>

#include <sprite2/SymType.h>
#include <sprite2/ResetActorFlagVisitor.h>
#include <sprite2/CreateActorsVisitor.h>
#include <sprite2/SprVisitorParams.h>

#include <fstream>

namespace ecomplex
{

static const char* GROUP_TAG = "group";

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
	, m_viewlist(NULL)
{
	InitLayout();

	m_library->LoadFromConfig();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	int type = ee::SymbolFile::Instance()->Type(filepath);
	switch (type)
	{
	case s2::SYM_COMPLEX:
		FileIO::LoadFromFile(this, filepath);
		LoadGroupTree(filepath);
		break;
	case ee::SYM_PSD:
		FileIO::LoadFromFile(this, filepath);
		break;
	default:
		FileIO::LoadFromFile(this, filepath);
	}

	// create actors
	auto& children = m_stage->GetSymbol()->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		auto spr = children[i];

		s2::ResetActorFlagVisitor v0;
		spr->Traverse(v0, s2::SprVisitorParams());

		s2::CreateActorsVisitor v1;
		spr->Traverse(v1, s2::SprVisitorParams());
	}
}

void Task::Store(const char* filepath) const
{
	FileIO::StoreToFile(this, filepath);
	StoreGroupTree(filepath);
	m_stage->OnSave();
}

bool Task::IsDirty() const
{
	return m_stage->IsEditDirty();
}

void Task::GetAllSprite(std::vector<ee::SprPtr>& sprs) const
{
	m_stage->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);
	wxWindow* right = InitLayoutRight(right_split);

	left_split->SetSashGravity(0.2f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new ecomplex::LibraryPanel(split);
	m_property = new ecomplex::PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new ecomplex::StagePanel(parent, m_parent, m_property, m_library);
	m_property->SetEditPanel(m_stage->GetStageImpl());
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_viewlist = new ee::ViewlistPanel(split);
	m_viewlist->SetListImpl(new ViewlistListImpl(m_stage, m_stage->GetStageImpl(), m_stage));
	dynamic_cast<SelectSpritesOP*>(m_stage->GetEditOP())->SetOpenSymbolDialogViewlist(m_viewlist);

	m_grouptree = new ee::GroupTreePanel(split, m_stage, m_stage->GetKeyState());

	split->SetSashGravity(0.5f);
	split->SplitHorizontally(m_viewlist, m_grouptree);

	return split;
}

void Task::LoadGroupTree(const char* filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (!value[GROUP_TAG].isNull()) {
		m_grouptree->LoadFromFile(value[GROUP_TAG]);
	} else {
		std::map<std::string, std::vector<ee::SprPtr> > map_actions;
		std::vector<ee::SprPtr> others;
		ecoco::Utility::GroupSpritesFromTag(m_stage->GetSymbol()->GetAllChildren(), map_actions, others);
		m_grouptree->InitGroups(map_actions);
	}
}

void Task::StoreGroupTree(const char* filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_grouptree->StoreToFile(value[GROUP_TAG]);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();
}

}