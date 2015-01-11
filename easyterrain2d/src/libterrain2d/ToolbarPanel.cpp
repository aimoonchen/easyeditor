#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "CreateRegionCMPT.h"
#include "WaveVerticesCMPT.h"
#include "MoveTexcoordsCMPT.h"
#include "BlendTextureCMPT.h"

namespace eterrain2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new CreateRegionCMPT(this, wxT("��������"), stage));
	addChild(new WaveVerticesCMPT(this, wxT("���㲨��"), stage));
	addChild(new MoveTexcoordsCMPT(this, wxT("�����ƶ�"), stage));
	addChild(new BlendTextureCMPT(this, wxT("������"), stage));
	SetSizer(initLayout());
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}