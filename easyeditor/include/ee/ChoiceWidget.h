#ifndef _EASYEDITOR_CHOICE_WIDGET_H_
#define _EASYEDITOR_CHOICE_WIDGET_H_

#include "DynamicWidget.h"

namespace ee
{

class ChoiceWidget : public DynamicWidget
{
public:
	virtual std::string GetValue() const override;

	virtual bool IsChanged() const override;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const DynamicInfo& info) override;

	void SpecialLoad(const std::string& title, const std::string& key,
		const std::vector<std::string>& items, const std::string& default_item);

protected:
	virtual void Load(const Json::Value& value) override;

private:
	struct Item
	{
		std::string value;
		std::string title;
	};

	static const int MAX_ITEMS = 100;

private:
	int m_default;

	std::vector<Item> m_choices;

	wxChoice* m_ctrl;

	int m_ori_val;

}; // ChoiceWidget

}

#endif // _EASYEDITOR_CHOICE_WIDGET_H_