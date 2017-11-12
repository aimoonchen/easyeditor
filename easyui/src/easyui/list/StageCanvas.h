#ifndef _EASYUI_LIST_STAGE_CANVAS_H_
#define _EASYUI_LIST_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace eui
{
namespace list
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const override;

private:
	void DrawGuideLines() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}
}

#endif // _EASYUI_LIST_STAGE_CANVAS_H_