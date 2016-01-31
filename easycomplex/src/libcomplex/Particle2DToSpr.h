#ifndef _LIBCOMPLEX_PARTICLE2D_TO_SPR_H_
#define _LIBCOMPLEX_PARTICLE2D_TO_SPR_H_


#include <easyrespacker.h>

struct p2d_emitter_cfg;

namespace ecomplex
{

class Particle2DToSpr
{
public:
	static ee::Sprite* Trans(const librespacker::PackParticle2D* p2d);

private:
	static p2d_emitter_cfg* LoadConfig(const librespacker::PackParticle2D* p2d);

}; // Particle2DToSpr

}

#endif // _LIBCOMPLEX_PARTICLE2D_TO_SPR_H_