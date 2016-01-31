#include "SelectSpritesOP.h"
#include "StagePanel.h"

#include "dataset/GroupHelper.h"
#include "view/typedef.h"

namespace lr
{

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* spritesImpl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(stage_wnd, stage, spritesImpl, callback)
	, m_open_symbol(stage_wnd, stage, spritesImpl)
{
	stage->SetCursor(wxCursor(wxCURSOR_PENCIL));

	m_first_press.SetInvalid();
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (ee::SelectSpritesOP::OnKeyDown(keyCode)) {
		return true;
	}

	if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'g' || keyCode == 'G'))
	{
		GroupSelection();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'b' || keyCode == 'B'))
	{
		BreakUpSelection();
		return true;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDown(x, y)) return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->PointQuery(pos);

	if (m_first_press.IsValid()) {
		stage->Pathfinding(m_first_press, pos);
		m_first_press.SetInvalid();
	} else {
		m_first_press = pos;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (selected) {
		m_open_symbol.Open(selected);
	}

	return false;
}

void SelectSpritesOP::GroupSelection()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));

	ee::Sprite* group = GroupHelper::Group(sprites);

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sprites[i];
		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->Release();
	}

	ee::InsertSpriteSJ::Instance()->Insert(group);
	group->Release();
}

void SelectSpritesOP::BreakUpSelection()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		ee::Sprite* spr = sprites[i];
		if (spr->GetSymbol().GetFilepath() != GROUP_TAG) {
			continue;
		}

		std::vector<ee::Sprite*> children;
		GroupHelper::BreakUp(spr, children);
 		for (int j = 0, m = children.size(); j < m; ++j) {
			ee::Sprite* spr = children[j];
 			ee::InsertSpriteSJ::Instance()->Insert(spr);
			spr->Release();
 		}

		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->Release();
	}
}

}