#ifndef _DRAG2D_INTERFACE_SPRITE_ARRANGE_STATE_H_
#define _DRAG2D_INTERFACE_SPRITE_ARRANGE_STATE_H_

#include <stdio.h>

#include "common/typedef.h"

namespace d2d
{

class Vector;
class AbstractAtomicOP;

class IArrangeSpriteState
{
public:
	virtual ~IArrangeSpriteState() {}
	
	virtual bool OnMousePress(const Vector& pos) { return false; }
	virtual AbstractAtomicOP* OnMouseRelease(const Vector& pos) { return NULL; }

	virtual bool OnDirectionKeyDown(DirectionType type) { return false; }

}; // IArrangeState

}

#endif // _DRAG2D_INTERFACE_SPRITE_ARRANGE_STATE_H_