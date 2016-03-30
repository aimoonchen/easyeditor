#include <gl/glew.h>

#include "SpriteBlend.h"
#include "ShaderMgr.h"
#include "SpriteRenderer.h"
#include "ScreenCache.h"
#include "RenderContextStack.h"
#include "Sprite.h"
#include "ImageSymbol.h"
#include "GL.h"
#include "Math2D.h"
#include "BlendShader.h"
#include "EE_ShaderLab.h"
#include "SpriteShader.h"

#include <dtex_facade.h>

#include <assert.h>

namespace ee
{

SpriteBlend* SpriteBlend::m_instance = NULL;

SpriteBlend* SpriteBlend::Instance()
{
	if (!m_instance) {
		m_instance = new SpriteBlend();
	}
	return m_instance;
}

SpriteBlend::SpriteBlend()
{
}

void SpriteBlend::Draw(const Sprite* spr, const Matrix& mt) const
{
	assert(spr->rp->shader.blend != BM_NULL);

	ShaderMgr::Instance()->Flush();

	RenderContextStack* rc = RenderContextStack::Instance();

	Vector ori_offset;
	float ori_scale;
	rc->GetModelView(ori_offset, ori_scale);

	int ori_width, ori_height;
	rc->GetProjection(ori_width, ori_height);

	rc->SetModelView(Vector(0, 0), 1);
	int edge = dtexf_c1_get_texture_size();
	rc->SetProjection(edge, edge);
	GL::Viewport(0, 0, edge, edge);

	DrawSprToTmp(spr, mt);

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	GL::Viewport(0, 0, ori_width, ori_height);

	DrawTmpToScreen(spr, mt);
}

void SpriteBlend::DrawSprToTmp(const Sprite* spr, const Matrix& mt) const
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	BlendShader* shader = static_cast<BlendShader*>(mgr->GetShader(ShaderMgr::BLEND));

	dtexf_c1_bind();
	dtexf_c1_clear(0, -2, 2, 0);

	mgr->SetShader(ShaderMgr::BLEND);
	BlendMode mode = spr->rp->shader.blend;
	shader->SetMode(BlendModes::Instance()->GetNameENFromMode(mode));

	const_cast<Sprite*>(spr)->rp->shader.blend = BM_NULL;
	SpriteRenderer::Instance()->Draw(spr, spr, RenderParams(mt), false);
	const_cast<Sprite*>(spr)->rp->shader.blend = mode;

	mgr->Commit();
	ShaderLab::Instance()->Flush();

	dtexf_c1_unbind();
}

void SpriteBlend::DrawTmpToScreen(const Sprite* sprite, const Matrix& mt) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	Rect r = sprite->GetSymbol().GetSize();

	Vector vertices[4];
	vertices[0] = Vector(r.xmin, r.ymin);
	vertices[1] = Vector(r.xmin, r.ymax);
	vertices[2] = Vector(r.xmax, r.ymax);
	vertices[3] = Vector(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math2D::TransVector(vertices[i], t);
	}

	Vector texcoords[4];
	int edge = dtexf_c1_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i] - sprite->GetPosition();
		texcoords[i].x = texcoords[i].x / edge + 0.5f;
		texcoords[i].y = texcoords[i].y / edge + 0.5f;
	}

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->SetShader(ShaderMgr::SPRITE);
	SpriteShader* shader = static_cast<SpriteShader*>(mgr->GetShader(ShaderMgr::SPRITE));
	shader->SetColor(RenderColor());
	shader->Draw(vertices, texcoords, dtexf_c1_get_texture_id());
}

}