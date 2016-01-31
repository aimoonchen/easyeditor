#include "Symbol.h"
#include "Sprite.h"
#include "GTxt.h"

namespace etext
{

Symbol::Symbol()
{
	m_bg_style.fill = true;
	m_bg_style.color.Set(0.7f, 0.7f, 0.7f, 0.7f);
}

Symbol::~Symbol()
{
}

void Symbol::ReloadTexture() const
{
}

void Symbol::Draw(const ee::Matrix& mt, const ee::ColorTrans& color, 
				  const ee::Sprite* spr, const ee::Sprite* root) const
{
	const ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	if (setting.visible_label_bg) {
		DrawBackground(spr, mt);
	} 
 	if (setting.visible_label_text) {
 		DrawText(spr, mt, color.multi, color.add);
 	}
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	if (sprite) {
		const Sprite* font = static_cast<const Sprite*>(sprite);
		int w, h;
		font->GetSize(w, h);
		return ee::Rect(w, h);
	} else {
		return ee::Rect(m_width, m_height);
	}
}

void Symbol::LoadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_width = value["width"].asInt();
	m_height = value["height"].asInt();

	m_font = value["font"].asInt();
	m_font_size = value["font_size"].asInt();
	m_font_color = value["font_color"].asString();

	m_edge = value["edge"].asBool();
	m_edge_size = value["edge_size"].asDouble();
	m_edge_color = value["edge_color"].asString();

	m_space_hori = value["space_hori"].asDouble();
	m_space_vert = value["space_vert"].asDouble();

	m_align_hori = (HoriAlignType)(value["align_hori"].asInt());
	m_align_vert = (VertAlignType)(value["align_vert"].asInt());
}

void Symbol::DrawBackground(const ee::Sprite* sprite, const ee::Matrix& mt) const
{
	if (!sprite) {
		return;
	}

	if (const Sprite* font = dynamic_cast<const Sprite*>(sprite)) {
		int w, h;
		font->GetSize(w, h);
		ee::PrimitiveDraw::DrawRect(mt, w * 0.5f, h * 0.5f, m_bg_style);
	}
}

void Symbol::DrawText(const ee::Sprite* sprite, const ee::Matrix& mt,
					  const ee::Colorf& mul, const ee::Colorf& add) const
{
	if (!sprite) {
		return;
	}

	if (const Sprite* font = dynamic_cast<const Sprite*>(sprite)) {
		GTxt::Instance()->Draw(font, mt, mul, add);
	}
}

}