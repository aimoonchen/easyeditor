#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "AnimCurr.h"
#include "S2_Sprite.h"

#include <vector>

namespace s2
{

class AnimSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnimSprite();
	AnimSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnimSprite* Clone() const;
	virtual bool Update(const RenderParams& params, float dt);

	void Start();

	const AnimCurr& GetAnimCurr() const { return m_curr; }

	void SetLoop(bool loop) { m_loop = loop; }
	void SetInterval(float dt) { m_interval = dt; }

	void SetFPS(int fps) { m_fps = fps; }

private:
	AnimCurr m_curr;

	bool m_loop;
	float m_interval;

	int m_fps;

	VI_DUMMY_FUNC

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_