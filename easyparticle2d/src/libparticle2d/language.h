#ifndef _EASYPARTICLE2D_LANGUAGE_H_
#define _EASYPARTICLE2D_LANGUAGE_H_

namespace eparticle2d
{

enum {
	LK_CENTER = 0,
	LK_OFFSET,
	LK_X,
	LK_Y,
	LK_START,
	LK_END,

	LK_COUNT,
	LK_EMISSION_TIME,
	LK_LIFE,
	LK_POSITION,
	LK_DIRECTION,
	LK_MODE_GRAVITY,
	LK_MODE_RADIUS,
	LK_MODE_SPD_COS,

	LK_GRAVITY,
	LK_SPEED,
	LK_TANGENTIAL_ACCEL,
	LK_RADIAL_ACCEL,
	LK_ROTATIOIN_IS_DIR,

	LK_START_RADIUS,
	LK_END_RADIUS,
	LK_DIRECTION_SPEED,

	LK_SPD_COS_AMPLITUDE,
	LK_SPD_COS_FREQUENCY,

	LK_REMOVE,
	LK_REMOVE_ALL,
	LK_COMPONENTS,
	LK_SCALE,
	LK_ANGLE,
	LK_COL_MUL_START,
	LK_COL_MUL_END,
	LK_COL_ADD_START,
	LK_COL_ADD_END,
	LK_ALPHA,
};

static const char* EN[] = {
	"center",
	"offset",
	"X",
	"Y",
	"Start",
	"End",

	"Count",
	"Emission Time (ms)",
	"Life (ms)",
	"Position",
	"Direction (degree)",
	"Gravity Mode",
	"Radius Mode",
	"Spd Cos Mode",

	"Gravity",
	"Speed",
	"Tangential Accel",
	"Radial Accel",
	"Rotation is Dir",

	"Start Radius",
	"End Radius",
	"Direction Speed",

	"Spd Cos Amplitude",
	"Spd Cos Frequency",

	"Remove",
	"Remove All",
	"Components",
	"Scale",
	"Angle",
	"Col Mul Start",
	"Col Mul End",
	"Col Add Start",
	"Col Add End",
	"Alpha",
};

static const char* CN[] = {
	"����",
	"ƫ��",
	"X",
	"Y",
	"��ʼ",
	"����",

	"����",
	"��������(����)",
	"��������(����)",
	"λ��",
	"����(�Ƕ�)",
	"һ��ģʽ",
	"��תģʽ",
	"�ٶ���������ģʽ",

	"����",
	"�ٶ�",
	"������ٶ�",
	"������ٶ�",
	"�����˶�����",

	"��ʼ�뾶",
	"�����뾶",
	"��ת�ٶ�",

	"�����ٶ����",
	"�����ٶ�Ƶ��",

	"ɾ��",
	"ɾ������",
	"���",
	"����",
	"�Ƕ�",
	"��ʼ��ɫ",
	"������ɫ",
	"��ʼ��ɫ",
	"������ɫ",
	"͸����",
};

static const char** LANG = CN;

}

#endif // _EASYPARTICLE2D_LANGUAGE_H_