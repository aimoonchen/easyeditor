#ifndef _EASYPARTICLE3D_LANGUAGE_H_
#define _EASYPARTICLE3D_LANGUAGE_H_

namespace eparticle3d
{

enum {
	LK_CENTER = 0,
	LK_OFFSET,
	LK_MIN,
	LK_MAX,
	LK_RANGE,
	LK_SPEED,
	LK_START,
	LK_END,

	LK_NAME,
	LK_PACKAGE,
	LK_COUNT,
	LK_LAYER,
	LK_EMISSION_TIME,
	LK_LIFE,
	LK_HORI,
	LK_VERT,
	LK_LINEAR_SPEED,
	LK_ANGULAR_SPEED,
	LK_DISTURBANCE,
	LK_GRAVITY,
	LK_LINEAR_ACC,
	LK_INERTIA,
	LK_FADEOUT_TIME,
	LK_BOUNCE,
	LK_ADDITIVE_BLAND,
	LK_START_RADIUS,
	LK_ORIENT_MOVEMENT,
	LK_ORIENT_PARENT,

	LK_REMOVE,
	LK_REMOVE_ALL,
	LK_COMPONENTS,

	LK_SCALE,
	LK_ROTATE,
	LK_COL_MUL,
	LK_COL_ADD,
	LK_ALPHA,
	LK_BIND_PS,
};

static const char* EN[] = {
	"center",
	"offset",
	"min",
	"max",
	"range (pixel)",
	"speed (pixel)",
	"start",
	"end",

	"Name",
	"Package",	
	"Count",
	"Layer",
	"Emission Time (ms)",
	"Life (ms)",
	"Horizontal (deg)",
	"Vertical (deg)",
	"Speed (pixel)",
	"Angular Speed (degree)",
	"Disturbance",
	"Gravity (pixel)",
	"Linear Acc (pixel)",
	"Inertia",
	"Fadeout Time (ms)",
	"Bounce",
	"Additive Blend",
	"Start Radius (pixel)",
	"Orient to Movement",
	"Orient to Parent",

	"Remove",
	"Remove All",
	"Components",

	"Scale (%)",
	"Rotate (deg)",
	"Col Mul",
	"Col Add",
	"Alpha",
	"Bind PS",
};

static const char* CN[] = {
	"����",
	"ƫ��",
	"��С",
	"���",
	"����",
	"�ٶ�",
	"��ʼ",
	"����",

	"����",
	"����",
	"����",
	"��",
	"��������(����)",
	"��������(����)",
	"ˮƽ����Ƕ�",
	"��ֱ����Ƕ�",
	"���ٶ�",
	"���ٶ�",
	"�Ŷ�",
	"����",
	"���Լ��ٶ�",
	"ת������",
	"����ʱ��",
	"����",
	"������Ⱦ",
	"��ʼ�뾶",
	"�����˶�����",
	"���򸸽ڵ㷽��",

	"ɾ��",
	"ɾ��ȫ��",
	"���",

	"����(%)",
	"��ת",
	"��ɫ",
	"��ɫ",
	"͸����",
	"�󶨷�����",
};

static const char** LANG = CN;

}

#endif // _EASYPARTICLE3D_LANGUAGE_H_