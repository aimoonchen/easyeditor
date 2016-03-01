#include "ArrangeSpriteImpl.h"
#include "Math2D.h"
#include "Config.h"
#include "SettingData.h"
#include "EditPanelImpl.h"
#include "MultiSpritesImpl.h"
#include "PropertySettingPanel.h"
#include "Camera.h"
#include "SpriteSelection.h"
#include "DeleteSpriteAOP.h"
#include "CombineAOP.h"
#include "TranslateSpriteAOP.h"
#include "ScaleSpriteAOP.h"
#include "ShearSpriteAOP.h"
#include "PrimitiveDraw.h"
#include "panel_msg.h"
#include "RGBColorSettingDlg.h"
#include "HSLColorSettingDlg.h"
#include "AlphaSettingDlg.h"
#include "FetchAllVisitor.h"
#include "sprite_msg.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"
#include "ScaleSpriteState.h"
#include "ShearSpriteState.h"
#include "OffsetSpriteState.h"
#include "MoveSpriteState.h"
#include "CopyPasteSpriteState.h"
#include "PerspectiveSpriteState.h"

namespace ee
{

const float ArrangeSpriteImpl::CTRL_NODE_RADIUS = 10.0f;
const float ArrangeSpriteImpl::MAX_CTRL_NODE_RADIUS = 10.0f;

ArrangeSpriteImpl::ArrangeSpriteImpl(wxWindow* wnd, EditPanelImpl* stage,
									 MultiSpritesImpl* sprites_impl,
									 PropertySettingPanel* property,
									 const ArrangeSpriteConfig& cfg) 
	: m_wnd(wnd)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
	, m_property_panel(property)
	, m_align(sprites_impl)
	, m_op_state(NULL)
	, m_cfg(cfg)
	, m_popup(wnd, stage, sprites_impl, sprites_impl->GetSpriteSelection())
{
	m_align.SetOpen(cfg.is_auto_align_open);

	m_selection = sprites_impl->GetSpriteSelection();
	m_selection->Retain();

	m_left_down_pos.SetInvalid();
	m_right_down_pos.SetInvalid();

	m_ctrl_node_radius = CTRL_NODE_RADIUS;
}

ArrangeSpriteImpl::~ArrangeSpriteImpl()
{
	m_selection->Release();
	delete m_op_state;
}

void ArrangeSpriteImpl::OnKeyDown(int keycode)
{
	if (m_stage->GetKeyState(WXK_SHIFT)) {
		OnSpriteShortcutKey(keycode);
		return;
	}

	switch (keycode)
	{
	case WXK_DELETE:
		OnDeleteKeyDown();
		break;
	case WXK_PAGEUP:
		UpOneLayer();
		break;
	case WXK_PAGEDOWN:
		DownOneLayer();
		break;
	case 'a': case 'A':
		OnDirectionKeyDown(e_left);
		break;
	case 'd': case 'D':
		OnDirectionKeyDown(e_right);
		break;
	case 's': case 'S':
		OnDirectionKeyDown(e_down);
		break;
	case 'w': case 'W':
		OnDirectionKeyDown(e_up);
		break;
	case '[':
		DownOneLayer();
		break;
	case ']':
		UpOneLayer();
		break;
	case 'm' : case 'M':
		ChangeOPState(new MoveSpriteState(m_selection));
		break;
	case WXK_SPACE:
		OnSpaceKeyDown();
		break;

// 		// for debug
// 	case 'O':
// 		ShaderMgr::Instance()->SetBufferData(true);
// 		break;
// 	case 'C':
// 		ShaderMgr::Instance()->SetBufferData(false);
// 		break;
	}
}

void ArrangeSpriteImpl::OnKeyUp(int keyCode)
{
	if (m_property_panel)
	{
		m_property_panel->EnablePropertyGrid(true);
		m_property_panel->UpdatePropertyGrid();
	}
}

void ArrangeSpriteImpl::OnMouseLeftDown(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
	m_left_down_pos = pos;

	m_align.SetInvisible();

	Sprite* selected = NULL;
	if (m_selection->Size() == 1)
	{
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) {
		if (m_op_state) {
			m_op_state->OnMousePress(pos);
		}
		return;
	}

	// copy & paste
	if (m_stage->GetKeyState(WXK_ALT))
	{
		ChangeOPState(new CopyPasteSpriteState(m_selection, selected));
	}

	// offset
	if (m_cfg.is_offset_open)
	{
		Vector offset = GetSprOffset(selected);
		if (Math2D::GetDistance(offset, pos) < m_ctrl_node_radius) {
			ChangeOPState(CreateOffsetState(selected));
			return;
		}
	}

	// scale
	if (m_cfg.is_deform_open && !m_stage->GetKeyState(WXK_SHIFT))
	{
		Vector ctrlNodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (Math2D::GetDistance(ctrlNodes[i], pos) < m_ctrl_node_radius)
			{
				SpriteCtrlNode::Node cn;
				cn.pos = ctrlNodes[i];
				cn.type = SpriteCtrlNode::Type(i);
				ChangeOPState(CreateScaleState(selected, cn));
				return;
			}
		}
	}

	// perspective
	if (m_cfg.is_deform_open && m_stage->GetKeyState(WXK_SHIFT))
	{
		Vector ctrl_node[4];
		SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_node);
		for (int i = 0; i < 4; ++i) {
			if (Math2D::GetDistance(ctrl_node[i], pos) < m_ctrl_node_radius) {
				SpriteCtrlNode::Node cn;
				cn.pos = ctrl_node[i];
				cn.type = SpriteCtrlNode::Type(i);
				ChangeOPState(CreatePerspectiveState(selected, cn));
				return;
			}
		}
	}

	// translate
	ChangeOPState(CreateTranslateState(m_selection, pos));

	m_op_state->OnMousePress(pos);
}

void ArrangeSpriteImpl::OnMouseLeftUp(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state) 
	{
		m_op_state->OnMouseRelease(pos);
		ChangeOPState(NULL);
	}

	if (!m_selection->IsEmpty()) {
		Vector p;
		p.SetInvalid();
		ChangeOPState(CreateTranslateState(m_selection, p));
	}

	if (m_cfg.is_auto_align_open &&
		Config::Instance()->GetSettings().open_sprite_capture
		&& m_align.IsOpen() 
		&& !m_selection->IsEmpty()
		&& m_left_down_pos != pos)
	{
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		m_align.Align(sprites);
		SetCanvasDirtySJ::Instance()->SetDirty();
	}

	if (m_property_panel)
	{
		m_property_panel->EnablePropertyGrid(true);
		m_property_panel->UpdatePropertyGrid();
	}
}

void ArrangeSpriteImpl::OnMouseRightDown(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
	m_right_down_pos = pos;

	Sprite* selected = NULL;
	if (m_selection->Size() == 1)
	{
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return;

	// shear
	if (m_cfg.is_deform_open)
	{
		Vector ctrlNodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (Math2D::GetDistance(ctrlNodes[i], pos) < m_ctrl_node_radius)
			{
				SpriteCtrlNode::Node cn;
				cn.pos = ctrlNodes[i];
				cn.type = SpriteCtrlNode::Type(i);
				ChangeOPState(CreateShearState(selected, cn));
				return;
			}
		}
	}

	// rotate
	if (m_cfg.is_rotate_open) {
		ChangeOPState(CreateRotateState(m_selection, pos));
	}
}

void ArrangeSpriteImpl::OnMouseRightUp(int x, int y)
{
	if (!m_right_down_pos.IsValid()) {
		return;
	}

	Vector pos = m_stage->TransPosScrToProj(x, y);
	if (pos == m_right_down_pos)
	{
		wxMenu menu;
		SetRightPopupMenu(menu, x, y);
		m_stage->PopupMenu(&menu, x, y);
	}
	else if (m_op_state)
	{
		m_op_state->OnMouseRelease(pos);
		ChangeOPState(NULL);
	}

	if (m_property_panel)
	{
		m_property_panel->EnablePropertyGrid(true);
		m_property_panel->UpdatePropertyGrid();
	}
}

void ArrangeSpriteImpl::OnMouseMove(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state && m_op_state->OnMouseMove(pos))
	{
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnMouseDrag(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state && m_op_state->OnMouseDrag(pos))
	{
		SetCanvasDirtySJ::Instance()->SetDirty();
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnPopMenuSelected(int type)
{
	m_popup.OnRightPopupMenu(type);
}

void ArrangeSpriteImpl::OnDraw(const Camera& cam) const
{
	m_ctrl_node_radius = std::min(CTRL_NODE_RADIUS * cam.GetScale(), MAX_CTRL_NODE_RADIUS);
	if (m_ctrl_node_radius / cam.GetScale() < 5) {
		m_ctrl_node_radius = 0;
	}

	if (m_cfg.is_deform_open && m_selection->Size() == 1)
	{
		Sprite* selected = NULL;
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		selected = sprites[0];

		Rect r = selected->GetRect();
		float max_e = std::max(r.Width(), r.Height());
		if (m_ctrl_node_radius > max_e * 0.1f) {
			m_ctrl_node_radius = 0;
		} else {
			if (m_stage->GetKeyState(WXK_SHIFT)) 
			{
				Vector ctrl_nodes[4];
				SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_nodes);
				for (int i = 0; i < 4; ++i)
					PrimitiveDraw::DrawCircle(ctrl_nodes[i], m_ctrl_node_radius, true, 2, Colorf(0.2f, 0.8f, 0.2f));
			}
			else
			{
				Vector ctrl_nodes[8];
				SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrl_nodes);
				for (int i = 0; i < 4; ++i)
					PrimitiveDraw::DrawCircle(ctrl_nodes[i], m_ctrl_node_radius, false, 2, Colorf(0.2f, 0.8f, 0.2f));
				for (int i = 4; i < 8; ++i)
					PrimitiveDraw::DrawCircle(ctrl_nodes[i], m_ctrl_node_radius, true, 2, Colorf(0.2f, 0.8f, 0.2f));
			}

			if (m_cfg.is_offset_open)
			{
				Vector offset = GetSprOffset(selected);
				PrimitiveDraw::DrawCircle(offset, m_ctrl_node_radius, true, 2, Colorf(0.8f, 0.2f, 0.2f));
			}
		}
	}

	m_align.Draw();
}

void ArrangeSpriteImpl::Clear()
{
}

Sprite* ArrangeSpriteImpl::QueryEditedSprite(const Vector& pos) const
{
	Sprite* selected = NULL;
	if (m_cfg.is_deform_open && m_selection->Size() == 1)
	{
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return NULL;

	if (m_cfg.is_offset_open)
	{
		Vector offset = GetSprOffset(selected);
		if (Math2D::GetDistance(offset, pos) < m_ctrl_node_radius) {
			return selected;
		}
	}

	if (m_cfg.is_deform_open && !m_stage->GetKeyState(WXK_SHIFT))
	{
		Vector ctrl_nodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrl_nodes);
		for (int i = 0; i < 8; ++i) {
			if (Math2D::GetDistance(ctrl_nodes[i], pos) < m_ctrl_node_radius) {
				return selected;
			}
		}
	}

	if (m_cfg.is_deform_open && m_stage->GetKeyState(WXK_SHIFT))
	{
		Vector ctrl_nodes[4];
		SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_nodes);
		for (int i = 0; i < 4; ++i) {
			if (Math2D::GetDistance(ctrl_nodes[i], pos) < m_ctrl_node_radius) {
				return selected;
			}
		}
	}

	return NULL;
}

bool ArrangeSpriteImpl::IsSelectionEmpty() const
{
	return m_selection->IsEmpty();
}

void ArrangeSpriteImpl::OnDirectionKeyDown(DirectionType type)
{
	if (!m_op_state) return;

	bool dirty = m_op_state->OnDirectionKeyDown(type);
	if (dirty)
	{
		SetCanvasDirtySJ::Instance()->SetDirty();
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnSpaceKeyDown()
{
	std::vector<Sprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	if (sprites.empty()) {
		return;
	}

	CombineAOP* comb = new CombineAOP();
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		Sprite* sprite = sprites[i];

		comb->Insert(new TranslateSpriteAOP(sprite, -sprite->GetPosition()));
		comb->Insert(new ScaleSpriteAOP(sprite, Vector(1, 1), sprite->GetScale()));
		comb->Insert(new ShearSpriteAOP(sprite, Vector(0, 0), sprite->GetShear()));
		//comb->Insert(new OffsetSpriteAOP(sprite, Vector(0, 0), sprite->getOffset()));

		sprite->SetTransform(Vector(0, 0), 0);
		sprite->SetScale(Vector(1, 1));
		sprite->SetShear(Vector(0, 0));
		//sprite->setOffset(Vector(0, 0));
	}
	EditAddRecordSJ::Instance()->Add(comb);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void ArrangeSpriteImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	m_popup.SetRightPopupMenu(menu, x, y);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateTranslateState(SpriteSelection* selection, const Vector& first_pos) const
{
	return new TranslateSpriteState(selection, first_pos);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateRotateState(SpriteSelection* selection, const Vector& first_pos) const
{
	return new RotateSpriteState(selection, first_pos);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateScaleState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new ScaleSpriteState(sprite, ctrl_node);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateShearState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new ShearSpriteState(sprite, ctrl_node);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateOffsetState(Sprite* sprite) const
{
	return new OffsetSpriteState(sprite);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreatePerspectiveState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new PerspectiveSpriteState(sprite, ctrl_node);
}

void ArrangeSpriteImpl::ChangeOPState(ArrangeSpriteState* state)
{
	if (m_op_state) {
		delete m_op_state;
	}
	m_op_state = state;
}

void ArrangeSpriteImpl::OnDeleteKeyDown()
{
	// add to history
	std::vector<Sprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	EditAddRecordSJ::Instance()->Add(new DeleteSpriteAOP(sprites));

	m_sprites_impl->ClearSelectedSprite();
}

void ArrangeSpriteImpl::UpOneLayer()
{
	std::vector<Sprite*> selected;
	m_selection->Traverse(FetchAllVisitor<Sprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		ReorderSpriteSJ::Instance()->Reorder(selected[i], true);
	}
}

void ArrangeSpriteImpl::DownOneLayer()
{
	std::vector<Sprite*> selected;
	m_selection->Traverse(FetchAllVisitor<Sprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		ReorderSpriteSJ::Instance()->Reorder(selected[i], false);
	}
}

Vector ArrangeSpriteImpl::GetSprOffset(const Sprite* spr) const
{
	Vector offset = spr->GetPosition() + spr->GetOffset();
	return offset;
}

void ArrangeSpriteImpl::OnSpriteShortcutKey(int keycode)
{
	std::vector<Sprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	if (sprites.empty()) {
		return;
	}

	Vector proj_pos = sprites[0]->GetPosition();
	Vector screen_pos = m_stage->TransPosProjToScr(proj_pos);
	wxPoint pos(screen_pos.x, screen_pos.y);

	// editable
	if (keycode == 'e' || keycode == 'E')
	{
		bool editable = !sprites[0]->editable;
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			ee::Sprite* spr = sprites[i];
			spr->editable = editable;
		}
	}
	// visible
	else if (keycode == 's' || keycode == 'S')
	{
		bool visible = !sprites[0]->visiable;
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			ee::Sprite* spr = sprites[i];
			spr->visiable = visible;
		}
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
	// hori mirror
	else if (keycode == 'h' || keycode == 'H') 
	{
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			Sprite* spr = sprites[i];
			spr->SetMirror(!spr->GetMirrorX(), spr->GetMirrorY());
		}
		SetCanvasDirtySJ::Instance()->SetDirty();
	} 
	// vert mirror
	else if (keycode == 'v' || keycode == 'V') 
	{
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			Sprite* spr = sprites[i];
			spr->SetMirror(spr->GetMirrorX(), !spr->GetMirrorY());
		}
		SetCanvasDirtySJ::Instance()->SetDirty();
	} 
	// multi color
	else if (keycode == 'm' || keycode == 'M') 
	{
		if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_RGB) {
			RGBColorSettingDlg dlg(m_wnd, NULL, sprites[0]->color.multi, pos);
			if (dlg.ShowModal() == wxID_OK) {
				Colorf col = dlg.GetColor();
				for (int i = 0, n = sprites.size(); i < n; ++i) {
					sprites[i]->color.multi = col;
				}
				SetCanvasDirtySJ::Instance()->SetDirty();
			}
		} else {
			HSLColorSettingDlg dlg(m_wnd, NULL, sprites[0]->color.multi, pos);
			if (dlg.ShowModal() == wxID_OK) {
				Colorf col = dlg.GetColor();
				for (int i = 0, n = sprites.size(); i < n; ++i) {
					sprites[i]->color.multi = col;
				}
				SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
	} 
	// add color
	else if (keycode == 'a' || keycode == 'A') 
	{
		if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_RGB) {
			RGBColorSettingDlg dlg(m_wnd, NULL, sprites[0]->color.add, pos);
			if (dlg.ShowModal() == wxID_OK) {
				Colorf col = dlg.GetColor();
				for (int i = 0, n = sprites.size(); i < n; ++i) {
					sprites[i]->color.add = col;
				}
				SetCanvasDirtySJ::Instance()->SetDirty();
			}
		} else {
			HSLColorSettingDlg dlg(m_wnd, NULL, sprites[0]->color.add, pos);
			if (dlg.ShowModal() == wxID_OK) {
				Colorf col = dlg.GetColor();
				for (int i = 0, n = sprites.size(); i < n; ++i) {
					sprites[i]->color.add = col;
				}
				SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
	}
	// alpha
	else if (keycode == 't' || keycode == 'T') 
	{
		AlphaSettingDlg dlg(m_wnd, sprites[0]->color.multi, pos);
		float old_alpha = sprites[0]->color.multi.a;
		int state = dlg.ShowModal();
		if (state == wxID_OK) {
			float alpha = dlg.GetColor().a;
			for (int i = 0, n = sprites.size(); i < n; ++i) {
				sprites[i]->color.multi.a = alpha;
			}
			SetCanvasDirtySJ::Instance()->SetDirty();
		} else if (state == wxID_CANCEL) {
			sprites[0]->color.multi.a = old_alpha;
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}
}

}