#ifndef _EASYICON_STAGE_CANVAS_H_
#define _EASYICON_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eicon
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, wxGLContext* glctx,
		d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites);

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_stage;

	d2d::ISprite* m_edited;
	const d2d::MultiSpritesImpl* m_sprite_impl;

	d2d::ISprite* m_bg;

}; // StageCanvas

}

#endif // _EASYICON_STAGE_CANVAS_H_
