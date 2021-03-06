#include "NodeFromLua.h"
#include "LuaDataHelper.h"

#include <ee/Math2D.h>

#include <math.h>
#include <assert.h>

namespace erespacker
{

float NodeFromLua::TransTime(int time)
{
	return time * 0.001f;
}

float NodeFromLua::TransDegree(int deg)
{
	return deg * SM_DEG_TO_RAD;
}

float NodeFromLua::TransFloatX100(int f)
{
	return f * 0.01f;
}

void NodeFromLua::UnpackVertices(CU_VEC<sm::vec2>& vertices, lua_State* L, 
								 const std::string& name, bool reverse_y, int scale)
{
	int num = LuaDataHelper::GetIntField(L, (name + "_num").c_str());
	vertices.clear();
	vertices.resize(num);
	lua_getfield(L, -1, name.c_str());
	int len = lua_rawlen(L, -1);
	assert(len == num * 2);
	for (int i = 0; i <= len; ++i) 
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		int screen = static_cast<int>(lua_tonumber(L, -1));
		if (i % 2) {
			vertices[(i - 1) / 2].x = (float)screen / scale;
		} else {
			vertices[(i - 1) / 2].y = (float)screen / scale;
		}
		if (reverse_y) {
			vertices[(i - 1) / 2].y = -vertices[(i - 1) / 2].y;
		}
		lua_pop(L, 1);
	}
}

}