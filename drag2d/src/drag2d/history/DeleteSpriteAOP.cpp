#include "DeleteSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "dataset/ISprite.h"
#include "view/MultiSpritesImpl.h"

namespace d2d
{

DeleteSpriteAOP::DeleteSpriteAOP(const std::vector<ISprite*>& sprites, 
								 MultiSpritesImpl* sprites_impl)
	: m_sprites_impl(sprites_impl)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i) 
	{
		sprites[i]->retain();
		m_sprites.push_back(sprites[i]);
	}
}

DeleteSpriteAOP::~DeleteSpriteAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->release();
	}
}

void DeleteSpriteAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites_impl->insertSprite(m_sprites[i]);
	}
}

void DeleteSpriteAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites_impl->removeSprite(m_sprites[i]);
	}
}

Json::Value DeleteSpriteAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_DELETE;
	return val;
}

}