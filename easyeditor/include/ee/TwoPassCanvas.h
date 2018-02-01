#ifndef _EASYEDITOR_TWO_PASS_CANVAS_H_
#define _EASYEDITOR_TWO_PASS_CANVAS_H_

#include "StageCanvas.h"

namespace gum { class RenderTarget; }

namespace ee
{

class TwoPassCanvas : public StageCanvas
{
public:
	TwoPassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
		wxGLContext* glctx = NULL, uint32_t flag = USE_CONTEXT_STACK | HAS_2D);
	virtual ~TwoPassCanvas();

public:
	struct ScreenStyle
	{
		pt2::Color multi_col, add_col;
		bool col_grading;

		ScreenStyle() : multi_col(255, 255, 255, 255), add_col(0, 0, 0, 0), col_grading(false) {}
	};

protected:
	virtual void OnSize(int w, int h) override;
	virtual void OnDrawWhole() const override;

private:
	void DrawOnePass() const;
	void DrawTwoPass() const;

	void DrawPass1() const;
	void DrawPass2(const float* vertices, const float* texcoords, int tex_id) const;

	void DrawDirect() const;
	void DrawDRect() const;

	void DebugDraw() const;

protected:
	ScreenStyle m_scr_style;

private:
	gum::RenderTarget* m_rt;

}; // TwoPassCanvas

}

#endif // _EASYEDITOR_TWO_PASS_CANVAS_H_