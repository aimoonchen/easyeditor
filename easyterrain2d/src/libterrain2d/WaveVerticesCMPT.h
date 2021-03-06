#ifndef _EASYTERRAIN2D_WAVE_VERTICES_CMPT_H_
#define _EASYTERRAIN2D_WAVE_VERTICES_CMPT_H_

#include <ee/EditCMPT.h>

namespace eterrain2d
{

class StagePanel;
class OceanMesh;

class WaveVerticesCMPT : public ee::EditCMPT
{
public:
	WaveVerticesCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);

	void SetControlersValue(const OceanMesh* ocean);

protected:
	virtual wxSizer* InitLayout() override;

private:
	void OnOpenWaveChanged(wxCommandEvent& event);
	void OnReflushPress(wxCommandEvent& event);
	void OnChangeGridSize(wxSpinEvent& event);
	void OnChangeWaveParams(wxSpinEvent& event);
	void OnChangeDisplayTriangles(wxCommandEvent& event);
	void OnChangeBoundLock(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

	wxCheckBox* m_switch;

	wxSpinCtrl *m_row_spin, *m_col_spin;

	wxSpinCtrl *m_wave_speed, *m_wave_height;

	wxCheckBox* m_bound_lock;

}; // WaveVerticesCMPT

}

#endif // _EASYTERRAIN2D_WAVE_VERTICES_CMPT_H_