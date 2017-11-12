#ifndef _EASYEDITOR_CHECK_BOX_WIDGET_H_
#define _EASYEDITOR_CHECK_BOX_WIDGET_H_

#include "DynamicWidget.h"

namespace ee
{

class CheckBoxWidget : public DynamicWidget
{
public:
	virtual std::string GetValue() const override;

	virtual bool IsChanged() const override;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const DynamicInfo& info) override;

protected:
	virtual void Load(const Json::Value& value) override;
	
private:
	bool m_default;	
	bool m_has_default;

	wxCheckBox* m_ctrl;

	bool m_ori_val;

}; // CheckBoxWidget

}

#endif // _EASYEDITOR_CHECK_BOX_WIDGET_H_