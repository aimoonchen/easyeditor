#ifndef _EPBIN_EPE_PACK_PICTURE_H_
#define _EPBIN_EPE_PACK_PICTURE_H_

#include "../IPackNode.h"

#include <vector>

struct lua_State;

namespace epbin
{
namespace epe
{

class PackPicture : public IPackNode
{
public:
	PackPicture(lua_State* L, uint32_t id);

	virtual size_t Size() const;
	virtual void Store(std::ofstream& fout) const;

public:
	struct Quad
	{
		uint8_t texid;
		uint16_t texture_coord[8];
		int32_t screen_coord[8];
	};

	const std::vector<Quad>& GetQuads() const { return m_quads; }

private:
	void Load(lua_State* L);

private:
	std::vector<Quad> m_quads;

}; // PackPicture

}
}

#endif // _EPBIN_EPE_PACK_PICTURE_H_