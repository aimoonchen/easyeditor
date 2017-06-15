#ifndef _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_
#define _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_

#include "PackNode.h"

#include <sprite2/Particle3dSprite.h>

namespace eparticle3d { class Sprite; }

namespace esprpacker
{

class PackParticle3dSpr : public PackNode
{
public:
	PackParticle3dSpr(const eparticle3d::Sprite* spr);
	virtual ~PackParticle3dSpr();

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

	bool Equal(const eparticle3d::Sprite* spr) const;

private:
	void Init(const eparticle3d::Sprite* spr);

private:
	const PackNode* m_sym;

	bool m_loop;
	bool m_local;
	bool m_alone;
	s2::Particle3dSprite::ReuseType m_reuse;

	float m_start_radius;

}; // PackParticle3dSpr

}

#endif // _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_