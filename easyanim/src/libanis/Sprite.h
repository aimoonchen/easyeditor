#ifndef _LIBANIS_SPRITE_H_
#define _LIBANIS_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/DummySprite.h>

namespace libanis
{

class Symbol;

class Sprite : public s2::DummySprite, public ee::Sprite
{
public:
	Sprite(Symbol* symbol);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual bool Update(const s2::RenderParams& params, float dt);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual Sprite* EEClone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* symbol);

private:
	int m_idx;

}; // Sprite

}

#endif // _LIBANIS_SPRITE_H_