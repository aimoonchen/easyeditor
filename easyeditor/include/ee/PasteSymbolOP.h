#ifndef _EASYEDITOR_PASTE_SYMBOL_H_
#define _EASYEDITOR_PASTE_SYMBOL_H_

#include "ZoomViewOP.h"

#include <SM_Vector.h>

namespace ee
{

class LibraryPanel;

class PasteSymbolOP : public ZoomViewOP
{
public:
	PasteSymbolOP(wxWindow* wnd, EditPanelImpl* stage, 
		LibraryPanel* library, float* pScale = NULL);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

protected:
	LibraryPanel* m_library;

	float* m_scale;

	sm::vec2 m_pos;

}; // PasteSymbolOP

}

#endif // _EASYEDITOR_PASTE_SYMBOL_H_