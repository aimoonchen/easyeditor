#ifndef _EASYIMAGE_PIXEL_DIFF_OP_H_
#define _EASYIMAGE_PIXEL_DIFF_OP_H_

#include <ee/ZoomViewOP.h>

namespace eimage
{

class StagePanel;

class PixelDiffOP : public ee::ZoomViewOP
{
public:
	PixelDiffOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	virtual bool OnActive() override;

private:
	StagePanel* m_stage;

//	ee::ImageSprite *m_left, *m_right;

}; // PixelDiffOP 

}

#endif // _EASYIMAGE_PIXEL_DIFF_OP_H_