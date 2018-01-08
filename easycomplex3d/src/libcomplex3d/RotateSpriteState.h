#ifndef _EASYCOMPLEX3D_ROTATE_SPRITE_STATE_H_
#define _EASYCOMPLEX3D_ROTATE_SPRITE_STATE_H_

#include "IEditState.h"

#include <ee/SpriteSelection.h>
#include <ee/Visitor.h>

namespace e3d { class Camera; }

namespace ecomplex3d
{

class StagePanel;
class Sprite;

class RotateSpriteState : public IEditState
{
public:
	RotateSpriteState(StagePanel* stage,
		const ee::SpriteSelection& selection);

	virtual void OnMousePress(const sm::ivec2& pos) override;
	virtual void OnMouseRelease(const sm::ivec2& pos) override;
	virtual void OnMouseMove(const sm::ivec2& pos) override;	

private:
	void Rotate(const e3d::Camera& cam, const sm::ivec2& start, const sm::ivec2& end);

private:
	class Visitor : public ee::RefVisitor<ee::Sprite>
	{
	public:
		Visitor(StagePanel* stage, const e3d::Camera& cam, const sm::ivec2& start, const sm::ivec2& end) 
			: m_stage(stage), m_cam(cam), m_start(start), m_end(end) {}
		virtual void Visit(const ee::SprPtr& spr, bool& next) override;

	private:
		StagePanel* m_stage;
		const e3d::Camera& m_cam;
		sm::ivec2 m_start, m_end;

	}; // Visitor

private:
	StagePanel* m_stage;
	const ee::SpriteSelection& m_selection;

	sm::ivec2 m_last_pos;

}; // RotateSpriteState

}

#endif // _EASYCOMPLEX3D_ROTATE_SPRITE_STATE_H_