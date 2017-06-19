#include "WarnList.h"
#include "Database.h"
#include "NodeType.h"
#include "LeafNode.h"
#include "MainList.h"

#include <map>

namespace edb
{

BEGIN_EVENT_TABLE(WarnList, wxTreeCtrl)
	EVT_TREE_ITEM_ACTIVATED(ID_CTRL, WarnList::OnItemActivated)
END_EVENT_TABLE()

WarnList::WarnList(wxWindow* parent, MainList* main_list, const Database& db)
	: wxTreeCtrl(parent, ID_CTRL, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_HIDE_ROOT)
	, m_main_list(main_list)
	, m_db(db)
{
	SetBackgroundColour(wxColour(229, 229, 229));
}

void WarnList::Build(const Database& db)
{
	DeleteAllItems();

	m_root = AddRoot("warn");

	BuildNodeErr(db);
	BuildDup(db);

	ExpandAll();
}

void WarnList::BuildNodeErr(const Database& db)
{
	wxTreeItemId ref_id = AppendItem(m_root, "���ô���");
	wxTreeItemId nouse_id = AppendItem(m_root, "����");
	const std::vector<Node*>& nodes = db.GetNodes();
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		Node* node = nodes[i];
		if (node->Type() != NODE_LEAF) {
			continue;
		}
		LeafNode* leaf = static_cast<LeafNode*>(node);
		if (leaf->IsRefError()) {
			AppendItem(ref_id, leaf->GetPath());
		}
		if (leaf->IsNoUse()) {
			AppendItem(nouse_id, leaf->GetPath());
		}
	}
}

void WarnList::BuildDup(const Database& db)
{
	wxTreeItemId id = AppendItem(m_root, "�ظ�");

	const std::multimap<std::string, int>& map_md5 = db.GetMD5Map();
	if (map_md5.size() < 2) {
		return;
	}

	std::vector<int> dup_list;
	std::multimap<std::string, int>::const_iterator 
		itr0 = map_md5.begin(),
		itr1 = itr0;
	++itr1;
	for ( ; itr1 != map_md5.end(); ++itr0, ++itr1) 
	{
		if (itr0->first == itr1->first) {
			dup_list.push_back(itr0->second);
		} else if (!dup_list.empty()) {
			dup_list.push_back(itr0->second);
			InsertDupList(db, dup_list, id);
			dup_list.clear();
		}
	}
	if (!dup_list.empty()) {
		InsertDupList(db, dup_list, id);
	}
}

void WarnList::InsertDupList(const Database& db, const std::vector<int>& list, wxTreeItemId parent)
{
	wxTreeItemId id = AppendItem(parent, wxString::FromDouble(list.size()));
	for (int i = 0, n = list.size(); i < n; ++i) 
	{
		const Node* node = db.Fetch(list[i]);
		if (node->Type() == NODE_INDEX) {
			continue;
		}

		const LeafNode* leaf = static_cast<const LeafNode*>(node);
		AppendItem(id, leaf->GetPath());
	}
}

void WarnList::OnItemActivated(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	wxString text = GetItemText(id);
	int node_id = m_db.QueryByPath(m_db.GetDirPath() + "\\" + text.ToStdString());
	m_main_list->OnSelected(node_id);
}

}