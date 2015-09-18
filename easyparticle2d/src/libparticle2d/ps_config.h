#ifndef _EASYPARTICLE2D_PS_CONFIG_H_
#define _EASYPARTICLE2D_PS_CONFIG_H_

namespace eparticle2d
{

enum PS_PROPERTY
{
	PS_CAPACITY = 0,
	PS_COUNT,
	PS_EMISSION_TIME,
	PS_LIFE_TIME,
	PS_POSITION_X,
	PS_POSITION_Y,
	PS_DIRECTION,

	PS_GRAVITY,
	PS_SPEED,
	PS_TANGENTIAL_ACCEL,
	PS_RADIAL_ACCEL,

	PS_START_RADIUS,
	PS_END_RADIUS,
	PS_DIRECTION_SPEED,

	PS_COS_AMPLITUDE,
	PS_COS_FREQUENCY,

	PS_SCALE,
	PS_ANGLE,
	PS_ALPHA

}; // PS_PROPERTY

static const int PARTICLE_CAP = 1000;

static const int MAX_COMPONENTS = 256;

} 

#endif // _EASYPARTICLE2D_PS_CONFIG_H_