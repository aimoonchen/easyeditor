#ifndef _EASYEJOY2D_SPRITE_H_
#define _EASYEJOY2D_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/DummySprite.h>

namespace eejoy2d
{

class Symbol;

class Sprite : public s2::DummySprite, public ee::Sprite
{
public:
	Sprite() {}
	Sprite(const Sprite& sprite) {}
	Sprite(Symbol* symbol);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual Sprite* EEClone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* symbol);

}; // Sprite

}

#endif // _EASYEJOY2D_SPRITE_H_