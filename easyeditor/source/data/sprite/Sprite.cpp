#include "dev_config.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "Exception.h"
#include "SpriteFactory.h"
#include "SpriteObserver.h"
#include "SpritePropertySetting.h"
#include "SpriteIO.h"
#include "SpritePool.h"

#include <sprite2/Sprite.h>
#include <sns/NodeSpr.h>

#ifdef OPEN_SCREEN_CACHE
#include "render/SpriteRenderer.h"
#include "render/EE_SP.h"
#endif // OPEN_SCREEN_CACHE

namespace ee
{

Sprite::Sprite()
	: s2::Sprite()
	, m_perspective(0, 0)
	, m_anchor(false)
	, m_observer(NULL)
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, m_perspective(spr.m_perspective)
	, m_tag(spr.m_tag)
	, m_anchor(spr.m_anchor)
	, m_observer(NULL)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	
	m_perspective = spr.m_perspective;

	m_tag    = spr.m_tag;
	m_anchor = spr.m_anchor;

	m_observer = NULL;

	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, m_perspective(0, 0)
	, m_anchor(false)
	, m_observer(NULL)
{
}

Sprite::~Sprite()
{
	SpritePool::Instance()->Remove(*this);
}

void Sprite::SetSymbol(const s2::SymPtr& sym)
{
	SpritePool::Instance()->Remove(*this);
	s2::Sprite::SetSymbol(sym);
}

void Sprite::ClearUserData(bool deletePtr)
{
	delete m_ud, m_ud = NULL;
}

void Sprite::SetPosition(const sm::vec2& pos)
{
	PartialUpdate pu;
	pu.Begin();

	if (m_observer) {
		m_observer->OnSetPosition(*this, pos);
	}

	s2::Sprite::SetPosition(pos);

	pu.End();
}

void Sprite::SetAngle(float angle)
{
	PartialUpdate pu;
	pu.Begin();

	if (m_observer) {
		m_observer->OnSetAngle(*this, angle);
	}

	s2::Sprite::SetAngle(angle);

	pu.End();
}

void Sprite::SetScale(const sm::vec2& scale)
{
	PartialUpdate pu;
	pu.Begin();

	if (m_observer) {
		m_observer->OnSetScale(*this, scale);
	}

	s2::Sprite::SetScale(scale);

	pu.End();
}

void Sprite::SetShear(const sm::vec2& shear)
{
	PartialUpdate pu;
	pu.Begin();

	if (m_observer) {
		m_observer->OnSetShear(*this, shear);
	}

	s2::Sprite::SetShear(shear);

	pu.End();
}

void Sprite::SetOffset(const sm::vec2& offset)
{
	PartialUpdate pu;
	pu.Begin();

	if (m_observer) {
		m_observer->OnSetOffset(*this, offset);
	}

	s2::Sprite::SetOffset(offset);

	pu.End();
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::SpriteIO spr_io;
	spr_io.Load(val, shared_from_this(), dir.c_str());
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::SpriteIO spr_io;
	spr_io.Store(val, shared_from_this(), dir.c_str());
}

void Sprite::Load(const sns::NodeSpr* spr)
{
	ee::SpriteIO spr_io;
	spr_io.Load(spr->GetBaseInfo(), shared_from_this());
}

PropertySetting* Sprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, 
		std::dynamic_pointer_cast<ee::Sprite>(shared_from_this()));
}

/************************************************************************/
/* class Sprite::PartialUpdate                                          */
/************************************************************************/

void Sprite::PartialUpdate::Begin()
{
#ifdef OPEN_SCREEN_CACHE
	m_find = SpatialPartition::Instance()->Remove(this);
	if (m_find) {
		SpriteRenderer::InvalidRect(this);
	}
#endif // OPEN_SCREEN_CACHE
}

void Sprite::PartialUpdate::End()
{
#ifdef OPEN_SCREEN_CACHE
	if (m_find) {
		SpriteRenderer::InvalidRect(this);
		SpatialPartition::Instance()->Insert(this);
	}
#endif // OPEN_SCREEN_CACHE
}

//////////////////////////////////////////////////////////////////////////
// class SpriteCmp
//////////////////////////////////////////////////////////////////////////

SpriteCmp::SpriteCmp(Type type /*= e_file*/)
	: m_type(type)
{
}

bool SpriteCmp::operator() (const SprPtr& s0, const SprPtr& s1) const
{
	switch (m_type)
	{
	case e_file:
		return std::dynamic_pointer_cast<ee::Symbol>(s0->GetSymbol())->GetFilepath()
			 < std::dynamic_pointer_cast<ee::Symbol>(s1->GetSymbol())->GetFilepath();
	case e_x:
		return s0->GetPosition().x < s1->GetPosition().x;
	case e_y:
		return s0->GetPosition().y < s1->GetPosition().y;
	case e_x_invert:
		return s0->GetPosition().x > s1->GetPosition().x;
	case e_y_invert:
		return s0->GetPosition().y > s1->GetPosition().y;
	default:
		return std::dynamic_pointer_cast<ee::Symbol>(s0->GetSymbol())->GetFilepath()
			 < std::dynamic_pointer_cast<ee::Symbol>(s1->GetSymbol())->GetFilepath();
	}
}

}