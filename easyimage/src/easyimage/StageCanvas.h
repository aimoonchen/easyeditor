#ifndef _EASYIMAGE_STAGE_CANVAS_H_
#define _EASYIMAGE_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace eimage
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const override;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYIMAGE_STAGE_CANVAS_H_
