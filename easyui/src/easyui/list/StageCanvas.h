#ifndef _EASYUI_LIST_STAGE_CANVAS_H_
#define _EASYUI_LIST_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eui
{
namespace list
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawGuideLines() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}
}

#endif // _EASYUI_LIST_STAGE_CANVAS_H_