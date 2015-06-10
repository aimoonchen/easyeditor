#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "ArrangeSpriteImpl.h"

#include <easycomplex.h>

namespace eui
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::PropertySettingPanel* property,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_symbols_cfg(this, library)
	, m_viewlist(NULL)
	, m_group_panel(NULL)
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property, NULL, 
		d2d::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	m_canvas = new StageCanvas(this);
}

void StagePanel::clear()
{
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::removeSprite(sprite);
	m_viewlist->Remove(sprite);
	m_group_panel->Remove(sprite);
	m_anchor_mgr.Remove(sprite);
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	m_viewlist->Insert(sprite);
	m_anchor_mgr.Insert(sprite);
}

bool StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	bool ret = d2d::SpritesPanelImpl::resetSpriteOrder(sprite, up);
	m_viewlist->Reorder(sprite, up);
	return ret;
}

void StagePanel::InitConfig()
{
	m_symbols_cfg.LoadConfig();
}

}