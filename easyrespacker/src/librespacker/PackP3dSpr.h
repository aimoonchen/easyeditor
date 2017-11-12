#ifndef _EASYRESPACKER_PACK_P3D_SPR_H_
#define _EASYRESPACKER_PACK_P3D_SPR_H_

#include "IPackNode.h"



namespace erespacker
{

class PackP3dSpr : public IPackNode
{
public:
	const IPackNode* p3d;

	bool loop;
	bool local;
	bool alone;
	bool reuse;

public:
	PackP3dSpr() {}
	PackP3dSpr(int id);

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

}; // PackP3dSpr

}

#endif // _EASYRESPACKER_PACK_P3D_SPR_H_