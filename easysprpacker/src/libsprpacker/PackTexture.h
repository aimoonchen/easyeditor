#ifndef _EASYSPRPACKER_PACK_TEXTURE_H_
#define _EASYSPRPACKER_PACK_TEXTURE_H_

#include "PackNode.h"

#include <vector>

namespace etexture { class Symbol; }

namespace esprpacker
{

class PackTexture : public PackNode
{
public:
	PackTexture(const etexture::Symbol* sym);
	virtual ~PackTexture();

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const;	

private:
	std::vector<const PackNode*> m_polys;

}; // PackTexture

}

#endif // _EASYSPRPACKER_PACK_TEXTURE_H_