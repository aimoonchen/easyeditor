#include "UIList.h"
#include "widget_id.h"

#include <easycomplex.h>

namespace eui
{
namespace list
{

UIList::UIList()
{
	m_clipbox.xmin = m_clipbox.ymin = -200;
	m_clipbox.xmax = m_clipbox.ymax =  200;

	m_vertical = m_horizontal = false;
	m_hori_count = m_vert_count = 0;
	m_hori_space = m_vert_space = 0;

	m_item_spr = NULL;
}

bool UIList::InsertSprite(ee::Sprite* sprite, int idx)
{
	if (!sprite) {
		return false;
	}
	assert(m_items.size() == m_hori_count * m_vert_count);
	if (m_item_spr && &sprite->GetSymbol() != &m_item_spr->GetSymbol()) {
		return false;
	}
	if (m_hori_count > 1 && m_vert_count > 1) {
		return false;
	}

	if (m_hori_count ==  0 && m_vert_count == 0) {
		sprite->Retain();
		m_item_spr = sprite;
		m_items.push_back(m_item_spr->Clone());
		m_hori_count = m_vert_count = 1;
		return true;
	}

	ee::Vector base_pos = m_items[0]->GetPosition();
	ee::Vector new_pos = sprite->GetPosition();
	if (m_vert_count == 1 && m_hori_count == 1) {
		if (base_pos == new_pos) {
			return false;
		}
		float dx = fabs(base_pos.x - new_pos.x);
		float dy = fabs(base_pos.y - new_pos.y);
		if (dx > dy) {
			m_hori_space = dx;
		} else {
			m_vert_space = dy;
		}
	} else if (m_hori_count == 1) {
		if (base_pos.x == new_pos.x) {
			return false;
		} else {
			m_hori_space = fabs(base_pos.x - new_pos.x);
		}
	} else if (m_vert_count == 1) {
		assert(m_hori_count != 1);
		if (base_pos.y == new_pos.y) {
			return false;
		} else {
			m_vert_space = fabs(base_pos.y - new_pos.y);
		}
	}

	Filling();

	return true;
}

bool UIList::ClearAllSprite()
{
	bool ret = !m_items.empty();

	if (m_item_spr) {
		m_item_spr->Release();
		m_item_spr = NULL;
	}

	for_each(m_items.begin(), m_items.end(), ee::ReleaseObjectFunctor<ee::Sprite>());
	m_items.clear();

	return ret;
}

void UIList::TraverseSprites(ee::Visitor& visitor) const
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(m_items[i], has_next);
		if (!has_next) break;
	}
}

void UIList::StoreToFile(const char* filename) const
{
	std::string name = filename;
	name = name.substr(0, name.find_last_of('_'));

	std::string dir = ee::FileHelper::GetFileDir(filename);

	// items complex
	ecomplex::Symbol items_complex;
	for_each(m_items.begin(), m_items.end(), ee::RetainObjectFunctor<ee::Sprite>());
	items_complex.m_sprites = m_items;
	for (int i = 0, n = items_complex.m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = items_complex.m_sprites[i];
		spr->name = "item" + ee::StringHelper::ToString(i+1);
		spr->Retain();
	}
	std::string items_path = name + "_items_complex[gen].json";
	items_complex.SetFilepath(items_path);
	ecomplex::FileStorer::Store(items_path.c_str(), &items_complex);

	// wrapper complex
	ecomplex::Sprite items_sprite(&items_complex);
	items_sprite.name = "anchor";
	ecomplex::Symbol wrapper_complex;
	wrapper_complex.m_clipbox = m_clipbox;
	wrapper_complex.m_sprites.push_back(&items_sprite);
	items_sprite.Retain();
	std::string top_path = name + "_wrapper_complex[gen].json";
	wrapper_complex.SetFilepath(top_path);
	ecomplex::FileStorer::Store(top_path.c_str(), &wrapper_complex);

	// ui
	std::string ui_path = filename;
	Json::Value value;
	value["items filepath"] = ee::FileHelper::GetRelativePath(dir, items_path).ToStdString();
	value["wrapper filepath"] = ee::FileHelper::GetRelativePath(dir, top_path).ToStdString();
	value["type"] = get_widget_desc(ID_LIST);
	value["horizontal"] = m_horizontal;
	value["vertical"] = m_vertical;
	value["clipbox"]["w"] = m_clipbox.Width();
	value["clipbox"]["h"] = m_clipbox.Height();
	value["clipbox"]["x"] = m_clipbox.xmin;
	value["clipbox"]["y"] = m_clipbox.ymax;
	value["hori space"] = m_hori_space;
	value["vert space"] = m_vert_space;
	value["hori count"] = m_hori_count;
	value["vert count"] = m_vert_count;
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(ui_path.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void UIList::LoadFromFile(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string type = value["type"].asString();
	if (!is_widget(type, ID_LIST)) {
		return;
	}

	const Json::Value& cb_val = value["clipbox"];
	m_clipbox.xmin = cb_val["x"].asDouble();
	m_clipbox.ymax = cb_val["y"].asDouble();
	m_clipbox.xmax = m_clipbox.xmin + cb_val["w"].asDouble();
	m_clipbox.ymin = m_clipbox.ymax - cb_val["h"].asDouble();

	m_horizontal = value["horizontal"].asBool();
	m_vertical = value["vertical"].asBool();

	m_hori_count = value["hori count"].asInt();
	m_vert_count = value["vert count"].asInt();

	m_hori_space = value["hori space"].asDouble();
	m_vert_space = value["vert space"].asDouble();

	std::string items_filepath = value["items filepath"].asString();
	items_filepath = ee::FileHelper::GetAbsolutePathFromFile(filename, items_filepath);
	ecomplex::Symbol items_complex;
	items_complex.LoadFromFile(items_filepath);
	for (int i = 0, n = items_complex.m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = items_complex.m_sprites[i];
		spr->Retain();
		m_items.push_back(spr);
	}

	if (!items_complex.m_sprites.empty()) {
		m_item_spr = items_complex.m_sprites[0]->Clone();
	}
}

bool UIList::ReFilling()
{
	if (!m_item_spr) {
		return false;
	}

	for_each(m_items.begin(), m_items.end(), ee::ReleaseObjectFunctor<ee::Sprite>());
	m_items.clear();

	m_items.push_back(m_item_spr->Clone());

	m_hori_count = m_vert_count = 1;

	Filling();

	return true;
}

bool UIList::Arrange(const ee::Sprite* spr)
{
	if (m_items.empty()) {
		return false;
	}

	int idx = -1;
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		if (m_items[i] == spr) {
			idx = i;
			break;
		}
	}
	assert(idx != -1);
	int row = idx / m_hori_count,
		col = idx % m_hori_count;
	if (idx == 0) {
		m_item_spr->SetTransform(m_items[0]->GetPosition(), m_item_spr->GetAngle());
	}
	if (row > 0) {
		float space = fabs(spr->GetPosition().y - m_item_spr->GetPosition().y) / row;
		if (space > 0) {
			m_vert_space = space;
		}
	}
	if (col > 0) {
		float space = fabs(spr->GetPosition().x - m_item_spr->GetPosition().x) / col;
		if (space > 0) {
			m_hori_space = space;
		}
	}
	return Arrange(m_hori_space, m_vert_space);
}

void UIList::EnableHori(bool enable)
{
	m_horizontal = enable;
}

void UIList::EnableVert(bool enable)
{
	m_vertical = enable;
}

bool UIList::Filling()
{
	if (!m_item_spr) {
		return false;
	}
	if (m_hori_count == 0 && m_vert_count == 0 ||
		m_hori_count > 1 && m_vert_count > 1) {
		return false;
	}

	bool ret = false;
	assert((m_hori_count == 1 && m_vert_count == 1) ||
		((m_hori_count == 1 || m_vert_count == 1) &&
		(m_hori_count > 1 || m_vert_count > 1)));

	for_each(m_items.begin(), m_items.end(), ee::ReleaseObjectFunctor<ee::Sprite>());
	m_items.clear();

	ee::Vector base = m_item_spr->GetPosition();
	ee::Rect item_r = m_item_spr->GetRect();
	float hw = item_r.Width() * 0.5f;
	float hh = item_r.Height() * 0.5f;

	ee::Rect region = m_clipbox;
	if (m_horizontal) {
		region.xmax += m_hori_space * 2;
	}
	if (m_vertical) {
		region.ymin -= m_vert_space * 2;
	}

	ee::Vector pos = base;
	m_hori_count = m_vert_count = 0;
	while (true) {
		bool new_line = false;
		if (pos.y - hh < region.ymin) {
			break;
		}
		int count = 0;
		while (true) {
			if (pos.x + hw > region.xmax) {
				pos.x = base.x;
				pos.y -= m_vert_space;
				break;
			} else {
				new_line = true;
				ee::Sprite* spr = m_item_spr->Clone();
				spr->SetTransform(pos, spr->GetAngle());
				m_items.push_back(spr);
				ret = true;
				pos.x += m_hori_space;
				++count;
			}
			if (m_hori_space == 0) {
				break;
			}
		}
		if (new_line) {
			new_line = false;
			++m_vert_count;
		}
		m_hori_count = count;
		if (m_hori_space == 0) {
			pos.y -= m_vert_space;
		}
		if (m_vert_space == 0) {
			break;
		}
	}

	return ret;
}

bool UIList::Arrange(float hori_space, float vert_space)
{
	assert(m_hori_count >= 1 && m_vert_count >= 1);
	assert(m_vert_count * m_hori_count == m_items.size());

	m_hori_space = hori_space;
	m_vert_space = vert_space;
	ee::Vector pos = m_items[0]->GetPosition();
	float x_base = pos.x;
	int count = 0;
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		ee::Sprite* spr = m_items[i];
		spr->SetTransform(pos, spr->GetAngle());
		pos.x += m_hori_space;

		++count;
		if (count >= m_hori_count) {
			pos.x = x_base;
			pos.y -= m_vert_space;
			count = 0;
		}
	}

	return true;
}

}
}