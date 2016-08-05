#ifdef __cplusplus
extern "C"
{
#endif

#ifndef motion_trail_2d_h
#define motion_trail_2d_h

#include "mt_utility.h"

#include <sm_c_vector.h>

#include <stdbool.h>

#define T2D_MODE_IMAGE	0
#define T2D_MODE_SHAPE	1

struct t2d_symbol {	
	struct mt_color color;

	union {
		// shape
		struct {
			float size;
			float acuity;
		} A;

		// symbol
		struct {
			struct mt_color color_add;
			float alpha_start, alpha_end;
			float radius;
			float scale_start, scale_end;
			void* ud;
		} B;
	} mode;
};

#define SIZEOF_T2D_SYMBOL (sizeof(struct t2d_symbol) + PTR_SIZE_DIFF)

struct t2d_particle {
	struct t2d_symbol* sym;

	float life;
	float lifetime;

	struct sm_vec2 pos;
	float angle;

	struct mt_color col_mul, col_mul_delta;
	struct mt_color col_add, col_add_delta;

	union {
		// shape
		struct {
			float size, size_delta;
		} A;

		// symbol
		struct {
			int zz;
		} B;
	} mode;

	struct t2d_particle* next;
};

#define SIZEOF_T2D_PARTICLE (sizeof(struct t2d_particle) + PTR_SIZE_DIFF * 2)

struct t2d_emitter_cfg {
	int count;

	float life_begin;
	float life_offset;

	float fadeout_time;

	int mode_type;
	union {
		// shape
		struct {
			int zz;
		} A;

		// symbol
		struct {
			int sym_count;
			struct t2d_symbol* syms;
		} B;
	} mode;

	int symbol_count;
	struct t2d_symbol* symbols;
};

#define SIZEOF_T2D_EMITTER_CFG (sizeof(struct t2d_emitter_cfg) + PTR_SIZE_DIFF)

struct t2d_emitter {
	struct t2d_particle *head;

	int particle_count;

	bool active;
	float time;

	const struct t2d_emitter_cfg* cfg;

	struct t2d_emitter* next;
};

#define SIZEOF_T2D_EMITTER (sizeof(struct t2d_emitter) + PTR_SIZE_DIFF * 3)

void t2d_init();
void t2d_regist_cb(void (*render_symbol_func)(void* symbol, float x, float y, float angle, uint8_t* mul_col, uint8_t* add_col, const void* ud),
				   void (*render_shape_func)(const float* positions, const uint32_t* colors, int count));

struct t2d_emitter* t2d_emitter_create(const struct t2d_emitter_cfg* cfg);
void t2d_emitter_release(struct t2d_emitter*);
void t2d_emitter_clear(struct t2d_emitter*);

void t2d_emitter_start(struct t2d_emitter*);
void t2d_emitter_stop(struct t2d_emitter*);

void t2d_emitter_update(struct t2d_emitter* et, float dt, struct sm_vec2* pos);
void t2d_emitter_draw(struct t2d_emitter* et, const void* ud);

#endif // motion_trail_2d_h

#ifdef __cplusplus
}
#endif