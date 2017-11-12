#ifndef _EASYANIM_LAYERS_PROPERTY_WIDGET_H_
#define _EASYANIM_LAYERS_PROPERTY_WIDGET_H_

#include <ee/Observer.h>

#include <wx/panel.h>

namespace eanim
{

class LayersPropertyWidget : public wxPanel, public ee::Observer
{
public:
	LayersPropertyWidget(wxWindow* parent);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void InitLayout();

	void OnAddLayer(wxCommandEvent& event);
	void OnDelLayer(wxCommandEvent& event);
//	void onPlay(wxCommandEvent& event);
	void OnChangeFps(wxSpinEvent& event);

private:
	wxSpinCtrl* m_fps;

}; // LayersPropertyWidget

}

#endif // _EASYANIM_LAYERS_PROPERTY_WIDGET_H_