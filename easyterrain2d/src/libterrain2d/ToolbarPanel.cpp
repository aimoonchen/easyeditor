#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "CreateRegionCMPT.h"
#include "WaveVerticesCMPT.h"

namespace eterrain2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new CreateRegionCMPT(this, wxT("��������"), stage));
	addChild(new WaveVerticesCMPT(this, wxT("���㲨��"), stage));
	SetSizer(initLayout());
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}