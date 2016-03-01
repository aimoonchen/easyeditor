#ifndef _EASYEDITOR_ORTHO_CANVAS_H_
#define _EASYEDITOR_ORTHO_CANVAS_H_

#include "TwoPassCanvas.h"

namespace ee
{

class OrthoCanvas : public TwoPassCanvas
{
public:
	OrthoCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
		wxGLContext* glctx = NULL, bool use_context_stack = true);

protected:
	virtual void OnSize(int w, int h);

}; // OrthoCanvas

}

#endif // _EASYEDITOR_ORTHO_CANVAS_H_