#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "EditRectOP.h"
#include "EditQuadOP.h"
#include "EditChangedRectOP.h"
#include "EditChangedSectorOP.h"

#include <ee/UniversalCMPT.h>

namespace eicon
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	// Rect
	{
		auto op = std::make_shared<EditRectOP>(stage);
		AddChild(new ee::UniversalCMPT(this, "����", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}
	// Quad
	{
		auto op = std::make_shared<EditQuadOP>(stage);
		AddChild(new ee::UniversalCMPT(this, "�ı���", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}
 	// Changed Rect
 	{
		auto op = std::make_shared<EditChangedRectOP>(stage);
 		AddChild(new ee::UniversalCMPT(this, "�ɱ�ľ���", stage->GetStageImpl(), op));
 		stage->SetEditOP(op);
 	}
	// Changed Sector
	{
		auto op = std::make_shared<EditChangedSectorOP>(stage);
		AddChild(new ee::UniversalCMPT(this, "�ɱ������", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}

	SetSizer(InitLayout());	
}

void ToolbarPanel::ChangeIconType(IconType type)
{
	switch (type) 
	{
	case IT_RECT:
		SetChoice(0);
		break;
	case IT_QUAD:
		SetChoice(1);
		break;
	case IT_CHANGED_RECT:
		SetChoice(2);
		break;
	case IT_CHANGED_SECTOR:
		SetChoice(3);
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