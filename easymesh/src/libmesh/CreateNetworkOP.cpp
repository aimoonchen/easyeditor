#include "CreateNetworkOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Mesh.h"
#include "Network.h"

#include <ee/Image.h>
#include <ee/panel_msg.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/DrawMesh.h>

namespace emesh
{

CreateNetworkOP::CreateNetworkOP(StagePanel* stage)
	: eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>(stage, stage->GetStageImpl(), stage, NULL, new ee::OneFloatValueStatic(5), NULL)
	, m_stage(stage)
	, m_selected_inner(NULL)
{
	SetLoop(true);
}

bool CreateNetworkOP::OnKeyDown(int keyCode)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnKeyDown(keyCode))
		return true;

	switch (keyCode)
	{
	case 'a': case 'A':
	case 'd': case 'D':
	case 's': case 'S':
	case 'w': case 'W':
		if (IsDirty()) {
			m_stage->UpdateSymbol();
			ResetDirty();
		}
		break;
	}

	return false;
}

bool CreateNetworkOP::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Network* mesh = static_cast<Network*>(m_stage->GetMesh());
	if (!mesh) {
		return false;
	}
	sm::vec2* selected = mesh->QueryInner(pos);
	if (selected) {
		m_selected_inner = selected;
		return false;
	} else if (mesh->InsertInner(pos)) {
		RefreshAll();
		return false;
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftDown(x, y)) {
		return true;
	}

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNetworkOP::OnMouseLeftUp(int x, int y)
{
	if (m_selected_inner) {
		m_selected_inner = NULL;
		RefreshAll();
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNetworkOP::OnMouseRightDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Network* nw = static_cast<Network*>(m_stage->GetMesh());
	if (nw->RemoveInner(pos)) {
		RefreshAll();
		return false;
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightDown(x, y)) {
		return true;
	}

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNetworkOP::OnMouseRightUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNetworkOP::OnMouseDrag(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_selected_inner) {
		*m_selected_inner = m_stage->TransPosScrToProj(x, y);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool CreateNetworkOP::OnDraw() const
{
	if (Mesh* mesh = m_stage->GetMesh()) {
		ee::SpriteRenderer::Instance()->Draw(mesh->GetBaseSymbol());
		s2::DrawMesh::DrawInfoUV(mesh);
	}

	eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnDraw();

	return false;
}

bool CreateNetworkOP::Clear()
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::Clear()) {
		return true;
	}
	m_selected_inner = NULL;
	return false;
}

void CreateNetworkOP::RefreshAll()
{
	m_stage->UpdateSymbol();
	ResetDirty();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}