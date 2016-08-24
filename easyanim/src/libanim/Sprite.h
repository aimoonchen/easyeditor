#ifndef _EASYANIM_SPRITE_H_
#define _EASYANIM_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/AnimSprite.h>

namespace eanim
{

class Symbol;

class Sprite : public s2::AnimSprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _EASYANIM_SPRITE_H_