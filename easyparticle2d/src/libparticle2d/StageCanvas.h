#ifndef _EASYPARTICLE2D_STAGE_CANVAS_H_
#define _EASYPARTICLE2D_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace eparticle2d
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

protected:
	virtual void OnDrawSprites() const override;

private:
	void DrawBackground() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYPARTICLE2D_STAGE_CANVAS_H_
