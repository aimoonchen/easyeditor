#ifndef _EASYSPRPACKER_PACK_MESH_H_
#define _EASYSPRPACKER_PACK_MESH_H_

#include "PackNode.h"

#include <sprite2/MeshSymbol.h>

#include <vector>

namespace emesh { class Symbol; }

namespace esprpacker
{

class MeshShape;

class PackMesh : public PackNode
{
public:
	PackMesh(const std::shared_ptr<emesh::Symbol>& sym);
	~PackMesh();

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const;	

private:
	const PackNode* m_base;

	const MeshShape* m_mesh;

}; // PackMesh

}

#endif // _EASYSPRPACKER_PACK_MESH_H_