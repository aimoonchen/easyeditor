#ifndef _LIBCOCO_UTILITY_NEW_H_
#define _LIBCOCO_UTILITY_NEW_H_

#include <drag2d.h>

namespace libcoco
{

class Utility
{
public:
	static bool IsAnchor(const d2d::ISprite* spr);

	static bool IsNameValid(const std::string& name);

}; // Utility

}

#endif // _LIBCOCO_UTILITY_NEW_H_