#ifndef _EASYSHAPE_DRAW_CURVE_OP_H_
#define _EASYSHAPE_DRAW_CURVE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <vector>

namespace ee { class EditPanelImpl; }

namespace eshape
{

class DrawCurveOP : public ee::ZoomViewOP
{
public:
	DrawCurveOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

protected:
	CU_VEC<sm::vec2> m_curve;

private:
	sm::vec2 m_first_pos;

	bool m_start_draw;

	bool m_straight_mode;

}; // DrawCurveOP

}

#endif // _EASYSHAPE_DRAW_CURVE_OP_H_