#ifndef _EASYSHAPE_EDIT_POLYLINE_WITH_COPY_NODE_OP_H_
#define _EASYSHAPE_EDIT_POLYLINE_WITH_COPY_NODE_OP_H_

#include "EditPolylineOP.h"
#include "SelectNodesOP.h"

namespace eshape
{

template <typename T>
class EditPolylineWithCopyNodeOP : public EditPolylineOP<T, SelectNodesOP>
{
public:
	EditPolylineWithCopyNodeOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiShapesImpl* shapes_impl)
		: EditPolylineOP<T, SelectNodesOP>(editPanel, shapes_impl, NULL, NULL)
	{}

	virtual bool OnKeyDown(int keyCode)
	{
		if (EditPolylineOP<T, SelectNodesOP>::OnKeyDown(keyCode)) 
			return true;

		// todo EditPolylineOP 
// 		if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_V))
// 			m_selectOP->fetchSelectedNode(m_polyline);

		return false;
	}

}; // EditPolylineWithCopyNodeOP

}

#include "EditPolylineWithCopyNodeOP.h"

#endif // _EASYSHAPE_EDIT_POLYLINE_WITH_COPY_NODE_OP_H_