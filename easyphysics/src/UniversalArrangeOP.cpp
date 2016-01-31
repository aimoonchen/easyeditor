#include "UniversalArrangeOP.h"
#include "DragPhysicsOP.h"
#include "PhysicsQuery.h"

namespace ephysics
{

UniversalArrangeOP::UniversalArrangeOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* spritesImpl,
									   ee::PropertySettingPanel* propertyPanel, ee::EditCMPT* callback/* = NULL*/)
	: ee::ZoomViewOP(wnd, stage, true)
{
	m_noPhysics = new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(wnd, stage, spritesImpl, propertyPanel, callback);
	m_editOP = m_noPhysics;
}

UniversalArrangeOP::~UniversalArrangeOP()
{
	m_noPhysics->Release();
	m_noPhysics = NULL;

	for (size_t i = 0, n = m_physics.size(); i < n; ++i)
		m_physics[i].editOP->Release();
	m_physics.clear();

	m_editOP = NULL;
}

bool UniversalArrangeOP::OnKeyDown(int keyCode)
{
	return m_editOP->OnKeyDown(keyCode);	
}

bool UniversalArrangeOP::OnMouseLeftDown(int x, int y)
{
	m_editOP = m_noPhysics;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	for (size_t i = 0, n = m_physics.size(); i < n; ++i)
	{
		if (PhysicsQuery::queryOn(m_physics[i].world, pos))
		{
			m_editOP = m_physics[i].editOP;
			break;
		}
	}

	return m_editOP->OnMouseLeftDown(x, y);
}

bool UniversalArrangeOP::OnMouseLeftUp(int x, int y)
{
	return m_editOP->OnMouseLeftUp(x, y);
}

bool UniversalArrangeOP::OnMouseRightDown(int x, int y)
{
	return m_editOP->OnMouseRightDown(x, y);
}

bool UniversalArrangeOP::OnMouseRightUp(int x, int y)
{
	return m_editOP->OnMouseRightUp(x, y);
}

bool UniversalArrangeOP::OnMouseDrag(int x, int y)
{
	return m_editOP->OnMouseDrag(x, y);
}

bool UniversalArrangeOP::OnDraw() const
{
	return m_editOP->OnDraw();
}

bool UniversalArrangeOP::Clear()
{
	return m_editOP->Clear();
}

void UniversalArrangeOP::addPhysicsEditOP(b2World* world, b2Body* ground)
{
	PhysicsOP physics;
	physics.world = world;
	physics.editOP = new DragPhysicsOP(m_wnd, m_stage, world, ground);
	m_physics.push_back(physics);
}

}
