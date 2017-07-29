#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <ee/SpriteFactory.h>
#include <ee/SymbolType.h>

#include <gum/trans_color.h>
#include <gum/TextboxLoader.h>

namespace etext
{

Sprite::Sprite()
	: m_export(false)
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::TextboxSprite(spr)
	, ee::Sprite(spr)
	, m_tid(spr.m_tid)
	, m_export(spr.m_export)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::TextboxSprite::operator = (spr);
	ee::Sprite::operator = (spr);

	m_tid = spr.m_tid;
	m_export = spr.m_export;

	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::TextboxSprite(sym)
	, ee::Sprite(sym)
	, m_export(false)
{
}

bool Sprite::Update(const s2::UpdateParams& up) 
{ 
	return m_text.find("dynamic") != std::string::npos;
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& text_val = val["text"];

	m_text	 = text_val["text"].asString();
	m_tid	 = text_val["tid"].asString();
	m_export = text_val["export"].asBool();

	gum::TextboxLoader loader(m_tb);
	loader.LoadJson(text_val);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	ee::Symbol* sym = VI_DOWNCASTING<ee::Symbol*>(m_sym);
	if (sym->GetFilepath().empty()) {
		val["filepath"] = ee::SYM_TEXT_TAG;
	}

	Json::Value text_val;

	text_val["width"]			= m_tb.width;
	text_val["height"]			= m_tb.height;

	text_val["font"]			= m_tb.font_type;
	text_val["font_size"]		= m_tb.font_size;
	text_val["font_color"]		= gum::color2str(m_tb.font_color, s2::RGBA);

	text_val["edge"]			= m_tb.has_edge;
	text_val["edge_size"]		= m_tb.edge_size;
	text_val["edge_color"]		= gum::color2str(m_tb.edge_color, s2::RGBA);

	text_val["align_hori"]		= m_tb.align_hori;
	text_val["align_vert"]		= m_tb.align_vert;

	text_val["space_hori"]		= m_tb.space_hori;
	text_val["space_vert"]		= m_tb.space_vert;	

	text_val["text"]			= m_text;
	text_val["tid"]				= m_tid;
	text_val["export"]          = m_export;

	text_val["overflow"]		= m_tb.overflow;

	text_val["richtext"]		= m_tb.richtext;

	val["text"]					= text_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}