#ifndef _EASYMODELING_SELECT_JOINT_OP_H_
#define _EASYMODELING_SELECT_JOINT_OP_H_

#include "SelectBodyOP.h"

#include <ee/SelectionSet.h>

namespace ee { class PropertySettingPanel; }

namespace emodeling
{

class Joint;
class StagePanel;

class SelectJointOP : public SelectBodyOP
{
public:
	SelectJointOP(wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, 
		ee::EditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	void SetPropertyPanel(ee::PropertySettingPanel* property) { 
		m_property_panel = property; 
	}

private:
	class DrawSelectedVisitor : public ee::Visitor<ee::Sprite>
	{
	public:
		virtual void Visit(const ee::SprPtr& spr, bool& next) override;
	}; // DrawSelectedVisitor

private:
	ee::PropertySettingPanel* m_property_panel;

	Joint* m_selected;
	Joint* m_mouseOn;

	sm::vec2 m_first_pos;

public:
	ee::SelectionSet<Joint> jointSelection;

//		std::vector<Joint*> selectedJoints;

}; // SelectJointOP

}

#endif // _EASYMODELING_SELECT_JOINT_OP_H_
