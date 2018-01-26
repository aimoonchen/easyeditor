#include "frame/Task.h"
#include "view/LibraryPanel.h"
#include "view/StagePanel.h"
#include "view/StageCanvas.h"
#include "view/SceneTreeCtrl.h"
#include "view/DetailPanel.h"
#include "editop/NodeArrangeOP.h"
#include "tools/Serializer.h"
#include "data/CompEditor.h"

#include <easynode3d/CamControlOP.h>

#include <node3/ComponentFactory.h>

#include <wx/aui/auibook.h>

namespace eone
{

Task::Task(wxFrame* frame)
	: m_frame(frame)
{
	m_mgr.SetManagedWindow(frame);

	InitLayout();
	InitCallback();
}

Task::~Task()
{
}

void Task::Load(const char* filename)
{
	Serializer::LoadFroimJson(filename, m_stage);

	m_tree->ExpandAll();
}

void Task::Store(const char* filename) const
{
	Serializer::StoreToJson(filename, m_stage);
}

bool Task::IsDirty() const
{
	return false;
}

void Task::InitLayout()
{
	m_mgr.AddPane(CreateLibraryPanel(),
		wxAuiPaneInfo().Name("Library").Caption("Library").
		Left().MinSize(wxSize(100, 0)));
	
	m_mgr.GetPane("Stage");

	m_mgr.AddPane(CreateStagePanel(),
		wxAuiPaneInfo().Name("Stage").Caption("Stage").
		CenterPane().PaneBorder(false));

	m_mgr.AddPane(CreateTreePanel(),
		wxAuiPaneInfo().Name("Tree").Caption("Tree").
		Right().Row(1).MinSize(200, 0).PaneBorder(false));

	m_mgr.AddPane(CreateDetailPanel(),
		wxAuiPaneInfo().Name("Detail").Caption("Detail").
		Right().MinSize(300, 0).PaneBorder(false));

	m_mgr.Update();
}

void Task::InitCallback()
{
	n3::ComponentFactory::Instance()->AddCreator(CompEditor::TYPE_NAME,
		[](n3::SceneNodePtr& node, const rapidjson::Value& val)
	{
		auto& comp = node->AddComponent<CompEditor>();
		comp.LoadFromJson(val);
	});
}

wxWindow* Task::CreateLibraryPanel()
{
	m_library = new LibraryPanel(m_frame);
	return m_library;
}

wxWindow* Task::CreateStagePanel()
{
	// create the notebook off-window to avoid flicker
	wxSize client_size = m_frame->GetClientSize();

	wxAuiNotebook* ctrl = new wxAuiNotebook(m_frame, wxID_ANY,
		wxPoint(client_size.x, client_size.y),
		wxSize(430, 200),
		wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);
	ctrl->Freeze();

	m_stage = new StagePanel(m_frame, m_frame, m_library);
	auto canvas = std::make_shared<StageCanvas>(m_stage);
	m_stage->SetCanvas(canvas);
	m_stage->SetEditOP(std::make_shared<NodeArrangeOP>(*m_stage));

	ctrl->AddPage(m_stage, ("New 3d"));

	ctrl->Thaw();

	return ctrl;
}

wxWindow* Task::CreateTreePanel()
{
	m_tree = new SceneTreeCtrl(m_frame, m_stage->GetSubjectMgr());
	return m_tree;
}

wxWindow* Task::CreateDetailPanel()
{
	return new DetailPanel(m_frame, m_stage->GetSubjectMgr());
}

}