#include "PackParticle3D.h"

#include "Particle3DToLuaString.h"
#include "Particle3DFromLua.h"

#include "Particle3DToBin.h"
#include "Particle3DFromBin.h"

namespace erespacker
{

PackParticle3D::PackParticle3D(int id)
	: IPackNode(id)
{
}

void PackParticle3D::PackToLuaString(ebuilder::CodeGenerator& gen, 
									 const ee::TexturePacker& tp,
									 float scale) const
{
	return Particle3DToLuaString::Pack(this, gen);
}

void PackParticle3D::UnpackFromLua(lua_State* L, const std::vector<ee::ImagePtr>& images)
{
	Particle3DFromLua::Unpack(L, this);
}

int PackParticle3D::SizeOfPackToBin() const
{
	return Particle3DToBin::Size(this);
}

void PackParticle3D::PackToBin(uint8_t** ptr, 
							   const ee::TexturePacker& tp,
							   float scale) const
{
	Particle3DToBin::Pack(this, ptr);
}

int PackParticle3D::SizeOfUnpackFromBin() const
{
	return Particle3DFromBin::Size(this);
}

void PackParticle3D::UnpackFromBin(uint8_t** ptr, const std::vector<ee::ImagePtr>& images)
{
	Particle3DFromBin::Unpack(ptr, this);
}

}