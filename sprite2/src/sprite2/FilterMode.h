#ifndef _SPRITE2_FILTER_MODE_H_
#define _SPRITE2_FILTER_MODE_H_

namespace s2
{

enum FilterMode
{
	FM_NULL					= 0,

	FM_EDGE_DETECTION		= 10,
	FM_RELIEF,
	FM_OUTLINE,

	FM_GRAY					= 20,
	FM_BLUR,
	FM_GAUSSIAN_BLUR,

	FM_HEAT_HAZE			= 30,
	FM_SHOCK_WAVE,
	FM_SWIRL,

}; // FilterMode

}

#endif // _SPRITE2_FILTER_MODE_H_