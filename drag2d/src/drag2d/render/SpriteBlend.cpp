#include "SpriteBlend.h"
#include "BlendShader.h"
#include "ShaderMgr.h"
#include "SpriteRenderer.h"
#include "ScreenCache.h"
#include "RenderContextStack.h"

#include "dataset/ISprite.h"
#include "dataset/ImageSymbol.h"
#include "view/Camera.h"
#include "render/GL.h"

#include <dtex_facade.h>

namespace d2d
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

void SpriteBlend::Draw(const ISprite* sprite, const Matrix& mt) const
{ 
	assert(sprite->GetBlendMode() != BM_NORMAL);

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

	DrawSprToTmp(sprite, mt);

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	GL::Viewport(0, 0, ori_width, ori_height);

	DrawTmpToScreen(sprite, mt);
}

void SpriteBlend::DrawSprToTmp(const ISprite* sprite, const Matrix& mt) const
{
	ShaderMgr* mgr = ShaderMgr::Instance();

	dtexf_c1_clear();
	dtexf_c1_bind();

	mgr->SpriteBlend();
	mgr->SetBlendMode(sprite->GetBlendMode());

	BlendShader* blend_shader = mgr->GetBlendShader();
	blend_shader->SetBaseTexID(ScreenCache::Instance()->GetTexID());

	SpriteRenderer::Instance()->DrawWithoutBlend(sprite, sprite, mt);

	dtexf_c1_unbind();

	mgr->SetSpriteShader(0);
}

void SpriteBlend::DrawTmpToScreen(const ISprite* sprite, const Matrix& mt) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	Rect r = sprite->GetSymbol().GetSize();

	Vector vertices[4];
	vertices[0] = Vector(r.xMin, r.yMin);
	vertices[1] = Vector(r.xMin, r.yMax);
	vertices[2] = Vector(r.xMax, r.yMax);
	vertices[3] = Vector(r.xMax, r.yMin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math::transVector(vertices[i], t);
	}

	Vector texcoords[4];
	int edge = dtexf_c1_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i] - sprite->GetPosition();
		texcoords[i].x = texcoords[i].x / edge + 0.5f;
		texcoords[i].y = texcoords[i].y / edge + 0.5f;
	}

	ShaderMgr* mgr = ShaderMgr::Instance();

	mgr->sprite();
	mgr->Draw(vertices, texcoords, dtexf_c1_get_texture_id());
}

}