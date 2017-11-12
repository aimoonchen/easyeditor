#ifndef _EASYDB_STAGE_CANVAS_H_
#define _EASYDB_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace edb
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* editPanel);
	virtual ~StageCanvas();

protected:
	virtual void OnDrawSprites() const override;

private:
	StagePanel* m_stage_panel;

}; // StageCanvas

}

#endif // _EASYDB_STAGE_CANVAS_H_