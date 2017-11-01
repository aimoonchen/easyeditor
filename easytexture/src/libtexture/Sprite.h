#ifndef _EASYTEXTURE_SPRITE_H_
#define _EASYTEXTURE_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/TextureSprite.h>

namespace etexture
{

class Symbol;

class Sprite : public s2::TextureSprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYTEXTURE_SPRITE_H_