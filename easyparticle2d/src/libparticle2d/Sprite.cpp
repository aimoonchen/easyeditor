#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <ee/Sprite.h>

#include <ps_2d.h>
#include <sprite2/Particle2d.h>
#include <gum/P2dSprLoader.h>

namespace eparticle2d
{

Sprite::Sprite()
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::Particle2dSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::Particle2dSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::Particle2dSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	gum::P2dSprLoader loader(*this);
	loader.LoadJson(val, dir.c_str());
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value p_val;
	p_val["loop"] = IsLoop();
	p_val["local_mode_draw"] = IsLocal();

	val["particle2d"] = p_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}