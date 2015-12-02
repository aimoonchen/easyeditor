#include "ViewlistList.h"
#include "ViewlistListImpl.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"

#include "message/subject_id.h"
#include "message/SelectSpriteSJ.h"
#include "message/ReorderSpriteSJ.h"
#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"
#include "message/ClearSpriteSJ.h"
#include "message/QuerySelectedSpriteLayerSJ.h"
#include "message/ClearPanelSJ.h"

#include <algorithm>

namespace d2d
{

ViewlistList::ViewlistList(wxWindow* parent)
	: VerticalImageList(parent, "viewlist", false)
	, m_impl(NULL)
	, m_selected_spr(NULL)
{
	RegistSubject(SelectSpriteSJ::Instance());
	RegistSubject(ReorderSpriteSJ::Instance());
	RegistSubject(InsertSpriteSJ::Instance());
	RegistSubject(RemoveSpriteSJ::Instance());
	RegistSubject(ClearSpriteSJ::Instance());
	RegistSubject(QuerySelectedSpriteLayerSJ::Instance());
	RegistSubject(ClearPanelSJ::Instance());
}

ViewlistList::~ViewlistList()
{
	if (m_impl) {
		delete m_impl;
	}

	Clear();
}

void ViewlistList::OnListSelected(wxCommandEvent& event)
{
	if (m_impl) {
		m_impl->OnSelected(this, event.GetInt());
	} else {
		OnSelected(event.GetInt());
	}
}

void ViewlistList::OnListDoubleClicked(wxCommandEvent& event)
{
	if (m_impl) {
		m_impl->OnDoubleClicked(this, event.GetInt());
	} else {
		OnSelected(event.GetInt());
	}
}

void ViewlistList::SetImpl(ViewlistListImpl* impl)
{
	if (m_impl) {
		delete m_impl;
	}
	m_impl = impl;
}

void ViewlistList::OnSelected(int idx)
{
	if (d2d::ISprite* spr = QuerySprite(idx)) {
		OnSelected(spr);
	}
}

d2d::ISprite* ViewlistList::QuerySprite(int idx)
{
	d2d::ISprite* spr = NULL;
	if (idx >= 0 && idx < m_sprites.size()) {
		spr = m_sprites[idx];
	}
	return spr;
}

void ViewlistList::Clear()
{
	VerticalImageList::Clear();

	if (m_selected_spr) {
		m_selected_spr->Release();
		m_selected_spr = NULL;
	}

	for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<ISprite>());
	m_sprites.clear();
}

void ViewlistList::Insert(ISprite* sprite, int idx)
{
	if (!sprite) {
		return;
	}
	sprite->Retain();

	ListItem* item = const_cast<ISymbol*>(&sprite->GetSymbol());
	if (idx < 0 || idx >= m_sprites.size()) {
		VerticalImageList::Insert(item, 0);
		m_sprites.insert(m_sprites.begin(), sprite);
	} else {
		int order = m_sprites.size() - idx;
		VerticalImageList::Insert(item, order);
		m_sprites.insert(m_sprites.begin() + order, sprite);
	}
}

void ViewlistList::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SELECT_SPRITE:
		{
			SelectSpriteSJ::Params* p = (SelectSpriteSJ::Params*)ud;
			Select(p->spr, p->clear);
		}
		break;
	case MSG_REORDER_SPRITE:
		{
			ReorderSpriteSJ::Params* p = (ReorderSpriteSJ::Params*)ud;
			Reorder(p->spr, p->up);
		}
		break;
	case MSG_INSERT_SPRITE:
		{
			InsertSpriteSJ::Params* p = (InsertSpriteSJ::Params*)ud;
			Insert(p->spr, p->idx);
		}
		break;
	case MSG_REMOVE_SPRITE:
		Remove((ISprite*)ud);
		break;
	case MSG_CLEAR_SPRITE: case MSG_CLEAR_PANEL:
		Clear();
		break;
	case MSG_QUERY_SPR_LAYER:
		{
			int* layer = (int*)ud;
			*layer = GetSelectedIndex();
		}
		break;
	}
}

void ViewlistList::OnKeyDown(wxKeyEvent& event)
{
	int curr_idx = this->GetSelection();
	VerticalImageList::OnKeyDown(event);

	switch (event.GetKeyCode())
	{
	case WXK_PAGEUP:
		SetSelection(curr_idx);
		OnSelected(curr_idx);
		ReorderSelected(true);
		break;
	case WXK_PAGEDOWN:
		SetSelection(curr_idx);
		OnSelected(curr_idx);
		ReorderSelected(false);
		break;
	case WXK_DELETE:
		RemoveSelected();
		break;
	}
}

int ViewlistList::GetSelectedIndex() const
{
	return GetItemCount() - 1 - GetSelection();
}

void ViewlistList::OnSelected(d2d::ISprite* spr)
{
	m_selected_spr = spr;
	m_selected_spr->Retain();

	bool add = GetKeyState(WXK_CONTROL);
	SelectSpriteSJ::Instance()->Select(spr, !add);
}

int ViewlistList::QuerySprIdx(const ISprite* spr) const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (m_sprites[i] == spr) {
			return i;
		}
	}
	return -1;
}

void ViewlistList::ReorderSelected(bool up)
{
	if (!m_selected_spr) {
		return;
	}

	Reorder(m_selected_spr, up);

	ReorderSpriteSJ::Instance()->Reorder(m_selected_spr, up, this);
}

void ViewlistList::Select(ISprite* spr, bool clear)
{
	int idx = QuerySprIdx(spr);
	if (idx >= 0) {
		SetSelection(idx);
	}
}

void ViewlistList::Reorder(const ISprite* sprite, bool up)
{
	int i = QuerySprIdx(sprite);
	if (i < 0) {
		return;
	}

	int n = m_sprites.size();
	if (up)
	{
		int pos = i - 1;
		if (pos >= 0)
		{
			std::swap(m_sprites[i], m_sprites[pos]);
			Swap(i, pos);
			SetSelection(pos);
		}
	}
	else
	{
		int pos = i + 1;
		if (pos < n)
		{
			std::swap(m_sprites[i], m_sprites[pos]);
			Swap(i, pos);
			SetSelection(pos);
		}
	}
}

void ViewlistList::Remove(ISprite* sprite)
{
	int idx = QuerySprIdx(sprite);
	if (idx < 0) {
		return;
	}
	VerticalImageList::Remove(idx);

	sprite->Release();
	m_sprites.erase(m_sprites.begin() + idx);
}

void ViewlistList::RemoveSelected()
{
	int idx = GetSelection();
	VerticalImageList::Remove(idx);

	RemoveSpriteSJ::Instance()->Remove(m_sprites[idx], this);

	m_sprites[idx]->Release();
	m_sprites.erase(m_sprites.begin() + idx);
}

} // d2d