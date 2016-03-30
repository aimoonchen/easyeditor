#include "PS.h"
#include "ParticleSystem.h"

#include <ee/RenderColor.h>
#include <ee/SpriteRenderer.h>

#include <ps_2d.h>

#include <time.h>

namespace eparticle2d
{

PS* PS::m_instance = NULL;

PS* PS::Instance()
{
	if (!m_instance) {
		m_instance = new PS();
	}
	return m_instance;
}

PS::PS()
	: m_time(0)
{
	Init();
}

void PS::UpdateTime()
{
	static clock_t last = 0;
	if (last == 0) {
		last = clock();
	} else {
		clock_t curr = clock();
		if (curr != last) {
			float dt = (float)(curr - last) / CLOCKS_PER_SEC;
			m_time += dt;
			last = curr;
		}
	}
}

static void 
render_func(void* symbol, float* mat, float x, float y, float angle, float scale, 
            struct ps_color4f* mul_col, struct ps_color4f* add_col, const void* ud)
{
	ee::Matrix mt;
	if (ud) {
		mt = *(ee::Matrix*)ud;
	} else {
		float* m = (float*)mt.GetElements();
		m[0] = mat[0];
		m[1] = mat[1];
		m[4] = mat[2];
		m[5] = mat[3];
		m[12] = mat[4];
		m[13] = mat[5];		
	}

	ee::Symbol* sym = static_cast<ee::Symbol*>(symbol);

	ee::RenderParams trans;
	trans.mt = mt;
	memcpy(&trans.color.multi, mul_col, sizeof(*mul_col));
	memcpy(&trans.color.add, add_col, sizeof(*add_col));

	ee::SpriteRenderer::Instance()->Draw(sym, trans, ee::Vector(x, y), angle, scale, scale, 0, 0);
}

void PS::Init()
{
	p2d_init();
	p2d_regist_cb(render_func);	
}

}