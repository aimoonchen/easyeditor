#include "StagePanel.h"
#include "StageCanvas.h"
#include "ResourceMgr.h"

namespace sg
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: EditPanel(parent, frame)
	, SpritesPanelImpl(this, library)
	, m_is_flat(false)
	, m_resource(new ResourceMgr(this, library))
{
	m_canvas = new StageCanvas(this);
	m_row = m_col = m_edge = 0;

	m_level = 1;
}

StagePanel::~StagePanel()
{
	delete m_resource;
}

void StagePanel::clear()
{
	EditPanel::clear();
	SpritesPanelImpl::clearSprites();
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	sprite->setTransform(fixSpriteLocation(sprite->getPosition()), sprite->getAngle());
	if (sprite->getPosition().isValid()) {
		d2d::SpritesPanelImpl::insertSprite(sprite);
	}
}

void StagePanel::transCoordsToGridPos(const d2d::Vector& pos, int& row, int& col) const
{
	d2d::Vector p;
	if (m_is_flat) {
		p = pos;
	} else {
		p = StageCanvas::transToFlatView(pos);		
	}
	col = std::max(0.0f, std::min(p.x / m_edge, (float)(m_col - 1)));
	row = std::max(0.0f, std::min(p.y / m_edge, (float)(m_row - 1)));
}

void StagePanel::transGridPosToCoords(int row, int col, d2d::Vector& pos) const
{
	pos.x = m_edge * col + m_edge * 0.5f;
	pos.y = m_edge * row + m_edge * 0.5f;
	if (!m_is_flat) {
		pos = StageCanvas::transToBirdView(pos);
	}
}

void StagePanel::updateAllSpritesLocation()
{
	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		s->setTransform(fixSpriteLocation(s->getPosition()), s->getAngle());
	}
}

void StagePanel::setPerspective(bool is_flat) 
{ 
	if (m_is_flat == is_flat) {
		return;
	}

	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];

		int row, col;
		transCoordsToGridPos(sprite->getPosition(), row, col);
		m_is_flat = !m_is_flat;

		d2d::Vector pos;
		transGridPosToCoords(row, col, pos);
		m_is_flat = !m_is_flat;

		sprite->setTransform(pos, sprite->getAngle());
	}

 	m_is_flat = is_flat; 
}

void StagePanel::changeSpritesLevel(bool up)
{
	std::vector<d2d::ISprite*> sprites;
	getSpriteSelection()->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		wxString filepath = sprite->getSymbol().getFilepath();
		if (!filepath.Contains("lv")) {
			continue;
		}

		int s = filepath.find("lv") + 2;
		int e = filepath.find('_', s-1);
		if (s == -1 || e == -1) {
			continue;
		}

		wxString sLevel = filepath.substr(s, e - s);
		double level;
		sLevel.ToDouble(&level);
		level = up ? level + 1 : level - 1;

		wxString newfilepath = filepath.substr(0, s) + wxString::FromDouble(level) + filepath.substr(e);

		if (!wxFileName::FileExists(newfilepath)) {
			continue;
		}

		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(newfilepath);
		if (symbol) {
			sprite->setSymbol(symbol);
		}
		symbol->release();
	}
}

d2d::Vector StagePanel::fixSpriteLocation(const d2d::Vector& pos) const
{
	int row, col;
	transCoordsToGridPos(pos, row, col);

	d2d::Vector ret;
	transGridPosToCoords(row, col, ret);
	return ret;
}

}