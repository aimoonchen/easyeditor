#ifndef _EASYTEXTURE_EDIT_DIALOG_H_
#define _EASYTEXTURE_EDIT_DIALOG_H_



namespace etexture
{

class Symbol;
class Sprite;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx, 
		Sprite* edited, const ee::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialog();

private:
	void InitLayout(wxGLContext* glctx, ee::Sprite* edited, 
		const ee::MultiSpritesImpl* sprite_impl);

	void OnCloseEvent(wxCloseEvent& event);

	void InitCamera(ee::Camera* cam, ee::Sprite* spr) const;

private:
	Symbol* m_symbol;

	ee::EditPanel* m_stage;

	bool m_visible_tex_edge;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYTEXTURE_EDIT_DIALOG_H_