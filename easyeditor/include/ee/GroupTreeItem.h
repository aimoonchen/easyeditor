#ifndef _EASYEDITOR_GROUP_TREE_ITEM_H_
#define _EASYEDITOR_GROUP_TREE_ITEM_H_

#include "Sprite.h"

#include <wx/treectrl.h>

namespace ee
{

class Group;

class GroupTreeItem : public wxTreeItemData
{
public:
	virtual ~GroupTreeItem() {}
	virtual GroupTreeItem* Clone() const  = 0;
	virtual bool IsGroup() const = 0;
}; // GroupTreeItem

class GroupTreeSpriteItem : public GroupTreeItem
{
public:
	GroupTreeSpriteItem(const SprPtr& spr);
	GroupTreeSpriteItem(const GroupTreeSpriteItem& item);

	virtual GroupTreeItem* Clone() const { return new GroupTreeSpriteItem(*this); }

	virtual bool IsGroup() const { return false; }

	const SprPtr& GetSprite() { return m_spr; }

private:
	SprPtr m_spr;

}; // GroupTreeSpriteItem

class GroupTreeGroupItem : public GroupTreeItem
{
public:
	GroupTreeGroupItem(Group* group);
	GroupTreeGroupItem(const GroupTreeGroupItem& item);
	virtual ~GroupTreeGroupItem();

	virtual GroupTreeItem* Clone() const { return new GroupTreeGroupItem(*this); }

	virtual bool IsGroup() const { return true; }

	Group* GetGroup() { return m_group; }

private:
	Group* m_group;

}; // GroupTreeGroupItem

}

#endif // _EASYEDITOR_GROUP_TREE_ITEM_H_