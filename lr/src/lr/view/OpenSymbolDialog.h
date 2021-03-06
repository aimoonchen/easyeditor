#ifndef _OPEN_SYMBOL_DIALOG_H_
#define _OPEN_SYMBOL_DIALOG_H_

#include <ee/Sprite.h>

namespace etexture { class Sprite; }
namespace ee { class EditPanelImpl; class MultiSpritesImpl; }

class wxWindow;

namespace lr
{

class OpenSymbolDialog
{
public:
	OpenSymbolDialog(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiSpritesImpl* sprites_impl);

	void Open(const ee::SprPtr& spr);

private:
	void UpdateShapeFromETexture(etexture::Sprite* spr);

private:
	wxWindow* m_wnd;

	ee::EditPanelImpl* m_stage;

	ee::MultiSpritesImpl* m_sprites_impl;

}; // OpenSymbolDialog

}

#endif // _OPEN_SYMBOL_DIALOG_H_