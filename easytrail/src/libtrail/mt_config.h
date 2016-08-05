#ifndef _EASYTRAIL_MT_CONFIG_H_
#define _EASYTRAIL_MT_CONFIG_H_

namespace etrail
{

enum MT_PROPERTY
{
	MT_COUNT = 0,
	MT_LIFETIME_BEGIN,
	MT_LIFETIME_OFFSET,
	MT_FADEOUT_TIME,

	MT_SCALE,
	
	MT_LINE_WIDTH,
	MT_ACUITY,
};

static const int MAX_COMPONENTS = 256;

}

#endif // _EASYTRAIL_MT_CONFIG_H_