#ifndef _EASYSHADER_SLIDER_CTRL_H_
#define _EASYSHADER_SLIDER_CTRL_H_

#include "SliderItem.h"

#include <wx/wx.h>

#include <vector>

namespace ee { class StageCanvas; }

namespace eshader 
{

class Shader;
class Uniform;

class SliderCtrl : public wxPanel
{
public:
	SliderCtrl(wxPanel* parent, const std::string& title, const std::string& name,
		Shader* shader, Uniform* uniform, const std::vector<SliderItemInt>& items, 
		float slider_accuracy, const std::shared_ptr<ee::StageCanvas>& canvas);
	SliderCtrl(wxPanel* parent, const std::string& title, const std::string& name,
		Shader* shader, Uniform* uniform, const std::vector<SliderItemFloat>& items, 
		float slider_accuracy, const std::shared_ptr<ee::StageCanvas>& canvas);

	void GetValue(double values[16]) const;

private:
	template <typename T>
	void InitLayout(const std::string& title, const std::vector<SliderItem<T> >& items);

	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxCommandEvent& event);

	void UpdateUniformValue();

private:
	struct Item
	{
		wxSlider* slider;
		wxTextCtrl* text;
		std::string name;
	};

private:
	std::string m_name;

	float m_slider_accuracy;

	std::vector<Item> m_items;

	Shader* m_shader;
	Uniform* m_uniform;

	std::shared_ptr<ee::StageCanvas> m_canvas = nullptr;

}; // SliderCtrl

}

#endif // _EASYSHADER_SLIDER_CTRL_H_