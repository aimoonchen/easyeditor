#ifndef _SPRITE2_COMPLEX_SPRITE_H_
#define _SPRITE2_COMPLEX_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class ComplexSymbol;

class ComplexSprite : public Sprite
{
public:
	ComplexSprite();
	
	virtual bool Update(float dt) const;
	
private:
	ComplexSymbol* m_sym;

}; // ComplexSprite

}

#endif // _SPRITE2_COMPLEX_SPRITE_H_