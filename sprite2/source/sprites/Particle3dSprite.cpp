#include "Particle3dSprite.h"

#include <stddef.h>

namespace s2
{

Particle3dSprite* Particle3dSprite::Clone() const
{
	return new Particle3dSprite(*this);
}

}