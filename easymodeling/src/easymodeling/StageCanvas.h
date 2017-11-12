#ifndef _EASYMODELING_STAGE_CANVAS_H_
#define _EASYMODELING_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/Visitor.h>

namespace emodeling
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
	void DrawGuideLines() const;
	void DrawSprites() const;

	void DrawCrossLine() const;
	void DrawLines() const;

private:
	class DrawJointVisitor : public ee::Visitor<Joint>
	{
	public:
		virtual void Visit(Joint* joint, bool& next) override;
	};

private:
	StagePanel* m_stage_panel;

}; // StageCanvas

}

#endif // _EASYMODELING_STAGE_CANVAS_H_
