#include "RenderTarget.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Shape.h"
#include "Exception.h"
#include "SpriteRenderer.h"

#include <unirender/RenderTarget.h>
#include <unirender/Texture.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <sprite2/RenderCtxStack.h>
#include <gum/RenderContext.h>

namespace ee
{

RenderTarget::RenderTarget()
	: m_impl(NULL)
{
}

RenderTarget::RenderTarget(int width, int height)
{
	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
	m_impl = new ur::RenderTarget(rc, width, height);
}

RenderTarget::~RenderTarget()
{
	delete m_impl;
}

void RenderTarget::ChangeSize(int width, int height)
{
	if (m_impl && 
		m_impl->GetTexture()->Width() == width &&
		m_impl->GetTexture()->Height() == height) {
		return;
	}

	delete m_impl;

	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
	m_impl = new ur::RenderTarget(rc, width, height);
}

void RenderTarget::DrawSprite(const Sprite* spr, bool clear, float dx, float dy)
{
	DrawFBO(spr, clear, GetWidth(), GetHeight(), dx, dy, 1);
}

void RenderTarget::DrawSprite(const Sprite* spr, bool clear, int width, int height, 
							  float dx, float dy, float scale)
{
	DrawFBO(spr, clear, width, height, dx, dy, scale);
}

void RenderTarget::DrawShape(const Shape* shape, bool clear, int width, int height)
{
	DrawFBO(shape, clear, width, height);
}

void RenderTarget::DrawSymbol(const Symbol* sym, bool whitebg, float scale)
{
	DrawFBO(sym, whitebg, scale);
}

void RenderTarget::ReadPixels(unsigned char* pixels, int width, int height) const
{
	m_impl->Bind();
	gum::RenderContext::Instance()->GetImpl()->ReadPixels(pixels, 0, 0, width, height);
	m_impl->Unbind();
}

int RenderTarget::GetTexID() const 
{ 
	if (!m_impl) {
		return 0;
	}

	return m_impl->GetTexture()->ID();
}

// int RenderTarget::GetFboID() const 
// { 
// 	if (!m_impl) {
// 		return 0;
// 	}
// }

int RenderTarget::GetWidth() const 
{ 
	if (!m_impl) {
		return 0;
	}

	return m_impl->GetTexture()->Width();
}

int RenderTarget::GetHeight() const 
{ 
	if (!m_impl) {
		return 0;
	}

	return m_impl->GetTexture()->Height();
}

void RenderTarget::DrawFBO(const Symbol* sym, bool whitebg, float scale)
{
	m_impl->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);

	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();

	rc->SetClearFlag(ur::MASKC);
	if (whitebg) {
		rc->Clear(0xffffffff);
	} else {
		rc->Clear(0);
	}	

	sm::rect rect = sym->GetBounding();
	sm::vec2 sz = rect.Size();
	int w = static_cast<int>(sz.x * scale),
		h = static_cast<int>(sz.y * scale);

	s2::RenderCtxStack::Instance()->Push(s2::RenderContext(w, h, w, h));

	s2::RenderParams params;
	sm::vec2 center = rect.Center();
	float dx = -center.x;
	float dy = center.y;
	params.mt.Translate(dx * scale, dy * scale, 0);
	ee::SpriteRenderer::Instance()->Draw(sym, params, sm::vec2(0, 0), 0.0f, sm::vec2(scale, -scale));

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	mgr->FlushShader();

	m_impl->Unbind();

	s2::RenderCtxStack::Instance()->Pop();
}

void RenderTarget::DrawFBO(const Sprite* spr, bool clear, int width, int height, float dx, float dy, float scale)
{
	m_impl->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);

	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();

	if (clear) {
		rc->SetClearFlag(ur::MASKC);
		rc->Clear(0);
	}

	s2::RenderCtxStack::Instance()->Push(s2::RenderContext(width, height, width, height));

	s2::RenderParams params;
	params.mt.Scale(scale, -scale, 1);
	params.mt.Translate(-dx, dy, 0);
	params.set_shader = false;
	SpriteRenderer::Instance()->Draw(spr, params);

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	mgr->FlushShader();

	m_impl->Unbind();

	s2::RenderCtxStack::Instance()->Pop();
}

void RenderTarget::DrawFBO(const Shape* shape, bool clear, int width, int height)
{
	m_impl->Bind();

	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();

	if (clear) {
		rc->SetClearFlag(ur::MASKC);
		rc->Clear(0);
	}

	s2::RenderCtxStack::Instance()->Push(s2::RenderContext(width, height, width, height));

	shape->Draw(sm::mat4::Scaled(1, -1, 1));

	m_impl->Unbind();

	s2::RenderCtxStack::Instance()->Pop();
}

}