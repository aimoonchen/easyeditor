#include "ToolBarPanel.h"
#include "StagePanel.h"

#include "EditRectOP.h"
#include "EditQuadOP.h"
#include "EditChangedRectOP.h"
#include "EditChangedSectorOP.h"

namespace eicon
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	// Rect
	{
		ee::EditOP* op = new EditRectOP(stage);
		addChild(new ee::UniversalCMPT(this, "����", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}
	// Quad
	{
		ee::EditOP* op = new EditQuadOP(stage);
		addChild(new ee::UniversalCMPT(this, "�ı���", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}
 	// Changed Rect
 	{
 		ee::EditOP* op = new EditChangedRectOP(stage);
 		addChild(new ee::UniversalCMPT(this, "�ɱ�ľ���", stage->GetStageImpl(), op));
 		stage->SetEditOP(op);
 	}
	// Changed Sector
	{
		ee::EditOP* op = new EditChangedSectorOP(stage);
		addChild(new ee::UniversalCMPT(this, "�ɱ������", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}

	SetSizer(InitLayout());	
}

void ToolbarPanel::ChangeIconType(IconType type)
{
	switch (type) 
	{
	case IT_RECT:
		setChoice(0);
		break;
	case IT_QUAD:
		setChoice(1);
		break;
	case IT_CHANGED_RECT:
		setChoice(2);
		break;
	case IT_CHANGED_SECTOR:
		setChoice(3);
		break;
	}
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(InitChildrenLayout());
	return sizer;
}

}