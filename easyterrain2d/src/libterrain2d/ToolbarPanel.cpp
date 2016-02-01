#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "CreateRegionCMPT.h"
#include "WaveVerticesCMPT.h"
#include "MoveTexcoordsCMPT.h"
#include "BlendTextureCMPT.h"

namespace eterrain2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	AddChild(new CreateRegionCMPT(this, wxT("��������"), stage));
	AddChild(m_wave_cmpt = new WaveVerticesCMPT(this, wxT("���㲨��"), stage));
	AddChild(m_move_uv_cmpt = new MoveTexcoordsCMPT(this, wxT("�����ƶ�"), stage));
	AddChild(m_blend_tex_cmpt = new BlendTextureCMPT(this, wxT("������"), stage));
	SetSizer(InitLayout());
}

void ToolbarPanel::SetControlersValue(const OceanMesh* ocean)
{
	m_wave_cmpt->SetControlersValue(ocean);
	m_move_uv_cmpt->SetControlersValue(ocean);
	m_blend_tex_cmpt->SetControlersValue(ocean);
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(InitChildrenLayout());
	return sizer;
}

}