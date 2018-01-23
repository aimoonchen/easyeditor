#include "DrawPencilLineCMPT.h"
#include "DrawPencilLineOP.h"

namespace eshape
{

DrawPencilLineCMPT::DrawPencilLineCMPT(wxWindow* parent, const std::string& name,
									   wxWindow* stage_wnd,ee::EditPanelImpl* stage)
	: ee::OneFloatValueCMPT(parent, name, stage, "Simplify", 0, 100, 30, 0.1f)
{
	m_editop = std::make_shared<DrawPencilLineOP>(stage_wnd, stage, this);
}

}