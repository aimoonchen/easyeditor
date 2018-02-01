#include "SceneTreeCtrl.h"
#include "SceneTreeItem.h"

#include <ee0/SubjectMgr.h>
#include <ee0/CompEditor.h>

#include <guard/check.h>

#include <queue>

namespace eone
{


BEGIN_EVENT_TABLE(SceneTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_SCENE_TREE_CTRL, SceneTreeCtrl::OnSelChanged)
	EVT_TREE_END_LABEL_EDIT(ID_SCENE_TREE_CTRL, SceneTreeCtrl::OnLabelEdited)
END_EVENT_TABLE()

SceneTreeCtrl::SceneTreeCtrl(wxWindow* parent, ee0::SubjectMgr& sub_mgr)
	: wxTreeCtrl(parent, ID_SCENE_TREE_CTRL, wxDefaultPosition, wxDefaultSize,
		wxTR_HIDE_ROOT | wxTR_EDIT_LABELS | wxTR_MULTIPLE | wxTR_NO_LINES | wxTR_DEFAULT_STYLE)
	, m_sub_mgr(&sub_mgr)
{
	SetBackgroundColour(wxColour(229, 229, 229));

	InitRoot();
	RegisterMsg(*m_sub_mgr);
}

void SceneTreeCtrl::OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants)
{
	switch (msg)
	{
	case ee0::MSG_INSERT_SCENE_NODE:
		InsertSceneNode(variants);
		break;
	case ee0::MSG_NODE_SELECTION_INSERT:
		SelectSceneNode(variants);
		break;
	case ee0::MSG_NODE_SELECTION_DELETE:
		UnselectSceneNode(variants);
		break;
	case ee0::MSG_NODE_SELECTION_CLEAR:
		UnselectAll();
		break;
	case ee0::MSG_STAGE_PAGE_CHANGING:
		DeleteAllItems();
		InitRoot();
		StagePageChanging(variants);
		break;
	}
}

void SceneTreeCtrl::Traverse(wxTreeItemId id, std::function<bool(wxTreeItemId)> func) const
{
	std::queue<wxTreeItemId> buf;
	buf.push(id);
	while (!buf.empty()) 
	{
		wxTreeItemId item = buf.front(); buf.pop();

		wxTreeItemIdValue cookie;
		wxTreeItemId id = GetFirstChild(item, cookie);
		if (id.IsOk()) 
		{
			if (item != id) 
			{
				// visit index
				bool stop = func(item);
				if (stop) {
					break;
				}
			}

			while (id.IsOk()) {
				buf.push(id);
				id = GetNextSibling(id);
			}
		} 
		else 
		{
			// visit leaf
			bool stop = func(item);
			if (stop) {
				break;
			}
		}
	}
}

void SceneTreeCtrl::InitRoot()
{
	m_root = AddRoot("ROOT");
	SetItemData(m_root, new SceneTreeItem());
}

void SceneTreeCtrl::RegisterMsg(ee0::SubjectMgr& sub_mgr)
{
	sub_mgr.RegisterObserver(ee0::MSG_INSERT_SCENE_NODE, this);
	sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_INSERT, this);
	sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_DELETE, this);
	sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_CLEAR, this);
	sub_mgr.RegisterObserver(ee0::MSG_STAGE_PAGE_CHANGING, this);
}

void SceneTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	auto id = event.GetItem();
	if (!id.IsOk()) {
		return;
	}

	auto data = (SceneTreeItem*)GetItemData(id);
	auto& node = data->GetNode();
	GD_ASSERT(node, "err scene node.");

	ee0::VariantSet vars;

	ee0::Variant var;
	var.m_type = ee0::VT_PVOID;
	var.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node);
	vars.SetVariant("node", var);

	ee0::Variant var_skip;
	var_skip.m_type = ee0::VT_PVOID;
	var_skip.m_val.pv = static_cast<Observer*>(this);
	vars.SetVariant("skip_observer", var_skip);

	if (IsSelected(id)) {
		m_sub_mgr->NotifyObservers(ee0::MSG_NODE_SELECTION_INSERT, vars);
	} else {
		m_sub_mgr->NotifyObservers(ee0::MSG_NODE_SELECTION_DELETE, vars);
	}
}

void SceneTreeCtrl::OnLabelEdited(wxTreeEvent& event)
{

}

void SceneTreeCtrl::SelectSceneNode(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != ee0::VT_EMPTY, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	bool multiple = false;
	auto var_multi = variants.GetVariant("multiple");
	if (var_multi.m_type == ee0::VT_BOOL && var_multi.m_val.bl) {
		multiple = true;
	}

	Traverse(m_root, [&](wxTreeItemId item)->bool
		{
			auto pdata = (SceneTreeItem*)GetItemData(item);
			if (pdata->GetNode() == *node) {
				SelectItem(item, !multiple);
				return true;
			} else {
				return false;
			}
		}
	);
}

void SceneTreeCtrl::UnselectSceneNode(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != ee0::VT_EMPTY, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	Traverse(m_root, [&](wxTreeItemId item)->bool
		{
			auto pdata = (SceneTreeItem*)GetItemData(item);
			if (pdata->GetNode() == *node) {
				UnselectItem(item);
				return true;
			} else {
				return false;
			}
		}
	);
}

void SceneTreeCtrl::InsertSceneNode(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != ee0::VT_EMPTY, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	wxTreeItemId parent = GetFocusedItem();
	if (!parent.IsOk()) {
		parent = m_root;
	}

	InsertSceneNode(parent, *node);

	if (parent != m_root) 
	{
		auto pdata = (SceneTreeItem*)GetItemData(parent);
		pdata->GetNode()->AddChild(*node);
		(*node)->SetParent(pdata->GetNode());
		Expand(parent);
	}
}

void SceneTreeCtrl::InsertSceneNode(wxTreeItemId parent, const n0::SceneNodePtr& node)
{
	auto item = new SceneTreeItem(node);

	auto pdata = (SceneTreeItem*)GetItemData(parent);
	auto pos = pdata->GetChildrenNum();
	pdata->AddChild(item);

	auto& ceditor = node->GetComponent<ee0::CompEditor>();
	wxTreeItemId id = InsertItem(parent, pos, ceditor.GetName());
	SetItemData(id, item);

	for (auto& child : node->GetAllChildren()) {
		InsertSceneNode(id, std::dynamic_pointer_cast<n0::SceneNode>(child));
	}
}

void SceneTreeCtrl::StagePageChanging(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("sub_mgr");
	GD_ASSERT(var.m_type != ee0::VT_EMPTY, "no var in vars: sub_mgr");
	GD_ASSERT(var.m_val.pv, "err scene node");
	m_sub_mgr = static_cast<ee0::SubjectMgr*>(var.m_val.pv);

	RegisterMsg(*m_sub_mgr);
}

}