#pragma once

#include "EditOpState.h"

namespace ee { class StageCanvas; }
namespace n3 { class Camera; }

namespace enode3d
{

class CamRotateState : public EditOpState
{
public:
	CamRotateState(ee::StageCanvas& canvas, n3::Camera& cam, 
		const sm::vec2& pos);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	ee::StageCanvas& m_canvas;

	n3::Camera& m_cam;

	sm::vec2 m_last_pos;

}; // CamRotateState

}