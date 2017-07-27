#ifndef _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_
#define _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_

#include <wx/dialog.h>

class wxCheckBox;

namespace eanim
{

class KeyFrame;

class FrameLerpDialog : public wxDialog
{
public:
	FrameLerpDialog(wxWindow* parent, KeyFrame* frame);

	void Store();

private:
	void InitLayout();

	wxSizer* InitCircleLayout();
	wxSizer* InitSpiralLayout();
	wxSizer* InitWiggleLayout();

private:
	KeyFrame* m_frame;

	wxCheckBox* m_pos_circle;
	wxSpinCtrl* m_circle_scale;	

	wxCheckBox* m_pos_spiral;
	wxSpinCtrl *m_spiral_angle_begin, *m_spiral_angle_end;
	wxSpinCtrl* m_spiral_scale;

	wxCheckBox* m_pos_wiggle;
	wxSpinCtrl *m_wiggle_freq, *m_wiggle_amp;

}; // FrameLerpDialog

}

#endif // _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_