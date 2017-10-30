#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>

namespace etext
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
{
	SetEditOP(new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(this, GetStageImpl(), this));
	SetCanvas(new StageCanvas(this));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx,
					   ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
{
	SetEditOP(new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(this, GetStageImpl(), this));
	SetCanvas(new StageCanvas(this, glctx, edited, bg_sprites));	
}

StagePanel::~StagePanel()
{	
}

}