#ifndef _EASYRESPACKER_PACK_MASK_H_
#define _EASYRESPACKER_PACK_MASK_H_

#include "IPackNode.h"

#include <SM_Vector.h>

namespace erespacker
{

class PackMask : public IPackNode
{
public:
	const IPackNode *base, *mask;
	
public:	
	PackMask() {}
	PackMask(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const override;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::ImagePtr>& images) override;

	virtual int SizeOfPackToBin() const override;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const override;
	virtual int SizeOfUnpackFromBin() const override;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::ImagePtr>& images) override;

}; // PackMask

}

#endif // _EASYRESPACKER_PACK_MASK_H_