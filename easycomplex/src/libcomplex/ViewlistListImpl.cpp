#include "ViewlistListImpl.h"

namespace ecomplex
{

ViewlistListImpl::ViewlistListImpl(wxWindow* wnd, d2d::EditPanelImpl* stage, 
								   d2d::MultiSpritesImpl* sprites_impl)
	: m_open_symbol(wnd, stage, sprites_impl)
{
}

void ViewlistListImpl::OnSelected(d2d::ViewlistList* list, int idx)
{
	list->OnSelected(idx);
}

void ViewlistListImpl::OnDoubleClicked(d2d::ViewlistList* list, int idx)
{
	m_open_symbol.Open(list->QuerySprite(idx));
}

}