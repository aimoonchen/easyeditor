#include "CopyPasteSpriteState.h"
#include "Sprite.h"
#include "SpriteSelection.h"
#include "sprite_msg.h"
#include "FetchAllVisitor.h"

#include <algorithm>

namespace ee
{

CopyPasteSpriteState::CopyPasteSpriteState(SpriteSelection* selection)
{
	std::vector<Sprite*> sprites;
	selection->Traverse(FetchAllVisitor<Sprite>(sprites));

	m_sprites.reserve(sprites.size());
	selection->Clear();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		Sprite* spr = sprites[i]->EEClone();
		m_sprites.push_back(spr);
		selection->Add(spr);
	}
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprites[i]);
	}
}

CopyPasteSpriteState::~CopyPasteSpriteState()
{
	for_each(m_sprites.begin(), m_sprites.end(), cu::RemoveRefFonctor<Sprite>());
}

void CopyPasteSpriteState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

bool CopyPasteSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	sm::vec2 offset = pos - m_last_pos;
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->Translate(offset);
	}
	m_last_pos = pos;
	return true;
}

}