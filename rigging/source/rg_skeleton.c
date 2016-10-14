#include "rg_skeleton.h"
#include "rg_joint.h"

#include <stdlib.h>

struct rg_skeleton {
	int joint_count;
	struct rg_joint** joints;
};

static void (*RENDER_FUNC)(void* sym, float x, float y, float angle, float sx, float sy);

void 
rg_skeleton_init(void (*render_func)(void* sym, float x, float y, float angle, float sx, float sy)) {
	RENDER_FUNC = render_func;
}

struct rg_skeleton* 
rg_skeleton_create(struct rg_joint** joints, int count) {
	struct rg_skeleton* sk = malloc(sizeof(*sk));
	if (!sk) {
		return NULL;
	}
	sk->joint_count = count;
	sk->joints = joints;
	return sk;
}

void 
rg_skeleton_release(struct rg_skeleton* sk) {
	free(sk);
}

void 
rg_rigging_draw(const struct rg_skeleton* sk, const void* ud) {
	for (int i = 0; i < sk->joint_count; ++i) {
		const struct rg_joint* joint = sk->joints[i];

		struct rg_joint_pose world;
		rg_local2world(&joint->world_pose, &joint->skin.local, &world);

		RENDER_FUNC(joint->skin.ud, world.trans[0], world.trans[1], world.rot, world.scale[0], world.scale[1]);
	}
}