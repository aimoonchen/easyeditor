#ifndef _SG_STAGE_CANVAS_H_
#define _SG_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace ee { class Symbol; class Sprite; }

namespace sg
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

	void SetBackground(ee::Symbol* background);

	static sm::vec2 TransToBirdView(const sm::vec2& pos);
	static sm::vec2 TransToFlatView(const sm::vec2& pos);

protected:
	virtual void OnDrawSprites() const override;

private:
	void DrawBackground() const;
	void DrawGuideLines() const;
	void DrawGrass() const;
	void DrawGrids() const;
	void DrawSprites() const;
	void DrawArrow() const;
	void DrawAttackRegion() const;

private:
	StagePanel* m_stage;

	ee::SprPtr m_background;

}; // StageCanvas

}

#endif // _SG_STAGE_CANVAS_H_