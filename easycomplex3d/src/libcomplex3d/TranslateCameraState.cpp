#include "TranslateCameraState.h"
#include "StageCanvas.h"

#include <node3/RenderContext.h>
#include <node3/RenderCtxStack.h>

static const float CAM_SPEED = 1 / 500.0f;

namespace ecomplex3d
{

TranslateCameraState::TranslateCameraState(e3d::StageCanvas* canvas)
	: m_canvas(canvas)
	, m_is_open(false)
{
}

void TranslateCameraState::OnMousePress(const sm::ivec2& pos)
{
	m_is_open = true;
	m_last_pos = pos;
}

void TranslateCameraState::OnMouseRelease(const sm::ivec2& pos)
{
	m_is_open = false;	
}

void TranslateCameraState::OnMouseMove(const sm::ivec2& pos)
{
	if (!m_is_open) { return; }
	auto ctx = n3::RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return;
	}

	sm::vec3 offset;
	offset.x = (pos.x - m_last_pos.x) * CAM_SPEED;
	offset.y = -(pos.y - m_last_pos.y) * CAM_SPEED;

	e3d::Camera& cam = m_canvas->GetCamera3();
	cam.Translate(-offset);

	m_last_pos = pos;

	const_cast<n3::RenderContext*>(ctx)->SetModelView(cam.GetModelViewMat());

	m_canvas->Refresh();
}

}