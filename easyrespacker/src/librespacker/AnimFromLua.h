#ifndef _EASYRESPACKER_ANIM_FROM_LUA_H_
#define _EASYRESPACKER_ANIM_FROM_LUA_H_

#include "PackAnimation.h"

namespace erespacker
{

class AnimFromLua
{
public:
	static void Unpack(lua_State* L, PackAnimation* anim);

private:
	static void UnpackComponents(lua_State* L, PackAnimation* anim);
	static void UnpackFrames(lua_State* L, PackAnimation* anim);

	static void UnpackFrame(lua_State* L, PackAnimation::Frame& frame);
	static void UnpackPart(lua_State* L, PackAnimation::Part& part);

}; // AnimFromLua

}

#endif // _EASYRESPACKER_ANIM_FROM_LUA_H_