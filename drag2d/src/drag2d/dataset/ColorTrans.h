#ifndef _DRAG2D_COLOR_TRANS_H_
#define _DRAG2D_COLOR_TRANS_H_

#include "common/Color.h"

namespace d2d
{

struct ColorTrans
{
	Colorf multi, add;
	Colorf r, g, b;

	ColorTrans();

}; // ColorTrans

inline ColorTrans::ColorTrans()
	: multi(1, 1, 1, 1)
	, add(0, 0, 0, 0)
	, r(1, 0, 0, 0)
	, g(0, 1, 0, 0)
	, b(0, 0, 1, 0)
{
}

}

#endif // _DRAG2D_COLOR_TRANS_H_
