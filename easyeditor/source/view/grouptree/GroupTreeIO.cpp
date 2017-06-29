#include "GroupTreeIO.h"
#include "GroupTreeCtrl.h"
#include "Sprite.h"
#include "MultiSpritesImpl.h"
#include "FetchAllVisitor.h"

#include <queue>

namespace ee
{

GroupTreeIO::GroupTreeIO(GroupTreeCtrl* tree, MultiSpritesImpl* sprites_impl)
	: m_tree(tree)
	, m_sprites_impl(sprites_impl)
{
}

void GroupTreeIO::Load(const Json::Value& value)
{
	std::vector<Sprite*> sprs;
	m_sprites_impl->TraverseSprites(FetchAllVisitor<Sprite>(sprs));

	std::vector<Node> nodes;
	LoadToNodes(value, nodes);

	wxTreeItemId root_id = m_tree->GetRootID();

	std::string parent_name = "root";
	wxTreeItemId parent_id = root_id;

	std::queue<std::pair<std::string, wxTreeItemId> > candidates;
	candidates.push(std::make_pair(parent_name, parent_id));
	
	while (!candidates.empty())
	{
		std::pair<std::string, wxTreeItemId> candidate = candidates.front(); candidates.pop();

 		std::vector<Node>::iterator itr = nodes.begin();
 		for ( ; itr != nodes.end(); ) {
 			const Node& node = *itr;
 			if (node.parent == candidate.first) {
				if (!node.spr.empty()) {
					Sprite* spr = NULL;

					for (int i = 0, n = sprs.size(); i < n; ++i) 
					{
						std::string name;
						s2::SprNameMap::Instance()->IDToStr(sprs[i]->GetName(), name);
						if (node.name == name) {
							spr = sprs[i];
							break;
						}
					}

					if (spr) {
						wxTreeItemId id = m_tree->AddSprite(candidate.second, spr);
						if (id != root_id) {
							candidates.push(std::make_pair(node.name, id));
						}
					}
				} else {
					wxTreeItemId id = m_tree->AddNode(node.name, candidate.second, 
						node.visible, node.editable);
					candidates.push(std::make_pair(node.name, id));
				}
				itr = nodes.erase(itr);
			} else {
				++itr;
			}
 		}

		if (nodes.empty()) {
			break;
		}
	}
}

void GroupTreeIO::LoadToNodes(const Json::Value& value, std::vector<Node>& nodes)
{
	int idx = 0;
	Json::Value node_val = value["node"][idx++];
	while (!node_val.isNull()) {
		Node node;
		node.is_leaf = node_val["leaf"].asBool();
		node.name = node_val["name"].asString();
		node.parent = node_val["parent"].asString();
		if (node_val.isMember("sprite")) {
			const Json::Value& spr_val = node_val["sprite"];
			if (spr_val.isString()) {
				node.spr = spr_val.asString();
			}
		}
		if (!node_val["visible"].isNull()) {
			node.visible = node_val["visible"].asBool();
		} else {
			node.visible = true;
		}
		if (!node_val["editable"].isNull()) {
			node.editable = node_val["editable"].asBool();
		} else {
			node.editable = true;
		}

		if (node.name != "root") {
			nodes.push_back(node);
		}
		node_val = value["node"][idx++];
	}
}

}