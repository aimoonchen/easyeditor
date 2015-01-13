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
	addChild(m_wave_cmpt = new WaveVerticesCMPT(this, wxT("���㲨��"), stage));
	addChild(m_move_uv_cmpt = new MoveTexcoordsCMPT(this, wxT("�����ƶ�"), stage));
	addChild(m_blend_tex_cmpt = new BlendTextureCMPT(this, wxT("������"), stage));
	SetSizer(initLayout());
}

void ToolbarPanel::SetControlersValue(const OceanMesh* ocean)
{
	m_wave_cmpt->SetControlersValue(ocean);
	m_move_uv_cmpt->SetControlersValue(ocean);
	m_blend_tex_cmpt->SetControlersValue(ocean);
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}