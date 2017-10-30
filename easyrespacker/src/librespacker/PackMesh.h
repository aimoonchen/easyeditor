#ifndef _EASYRESPACKER_PACK_MESH_H_
#define _EASYRESPACKER_PACK_MESH_H_

#include "IPackNode.h"

#include <cu/cu_stl.h>
#include <SM_Vector.h>

namespace erespacker
{

class PackMesh : public IPackNode
{
public:
	const IPackNode* base;

	CU_VEC<sm::vec2> tri_texcoords, tri_vertices;
	
public:	
	PackMesh() {}
	PackMesh(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::ImagePtr>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::ImagePtr>& images);

}; // PackMesh

}

#endif // _EASYRESPACKER_PACK_MESH_H_