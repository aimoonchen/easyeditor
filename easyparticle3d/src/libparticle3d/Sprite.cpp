#include "Sprite.h"
#include "Symbol.h"
//#include "ParticleSystem.h"
#include "SpritePropertySetting.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <ps_3d_buffer.h>
#include <sprite2/RenderParams.h>
#include <gum/P3dSprLoader.h>

namespace eparticle3d
{

Sprite::Sprite()
{
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::Particle3dSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& p_val = val["particle3d"];

	const Json::Value& d_val = p_val["dir"];
	float x = d_val["x"].asDouble(),
		  y = d_val["y"].asDouble(),
		  z = d_val["z"].asDouble(),
		  w = d_val["w"].asDouble();
	m_dir = sm::Quaternion(x, y, z, w);

	gum::P3dSprLoader loader(this);
	loader.LoadJson(val, dir);
}

void Sprite::Store(Json::Value& val, const std::string& _dir) const
{
	ee::Sprite::Store(val);

	Json::Value p_val;

	sm::vec4 dir = m_dir.ToVector();
	Json::Value d_val;
	d_val["x"] = dir.x;
	d_val["y"] = dir.y;
	d_val["z"] = dir.z;
	d_val["w"] = dir.w;
	p_val["dir"] = d_val;

	p_val["loop"] = m_loop;
	p_val["local_mode_draw"] = m_local;
	p_val["alone"] = m_alone;
	p_val["reuse"] = m_reuse;

	p_val["start_radius"] = m_start_radius;

	val["particle3d"] = p_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

bool Sprite::IsLoop() const
{
	if (m_spr) {
		return m_spr->et->loop;
	} else {
		return false;
	}
}

void Sprite::OnActive()
{
	// removed from buffer
	if (!m_spr) {
		CreateSpr();		
	}
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}