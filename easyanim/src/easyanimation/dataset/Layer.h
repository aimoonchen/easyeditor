#ifndef _EASYANIM_LAYER_H_
#define _EASYANIM_LAYER_H_

#include "SpriteObserver.h"

#include <cu/CU_RefCountObj.h>

namespace ee { class AtomicOP; }

namespace eanim
{

class KeyFrame;

class Layer : public cu::RefCountObj
{
public:
	Layer();
	~Layer();

	bool IsKeyFrame(int time) const;

	ee::AtomicOP* RemoveFrameRegion(int begin, int end);

	void InsertNullFrame(int time);
	void RemoveNullFrame(int time);

	void InsertKeyFrame(KeyFrame* frame);
	void InsertKeyFrame(int time);
	void RemoveKeyFrame(int time);

	void ChangeKeyFrame(KeyFrame* frame, int to);

	const std::map<int, KeyFrame*>& GetAllFrames() const {
		return m_frames;
	}

	KeyFrame* GetCurrKeyFrame(int time);
	KeyFrame* GetNextKeyFrame(int time);
	KeyFrame* GetPrevKeyFrame(int time);

	int GetMaxFrameTime() const;
	KeyFrame* GetEndFrame() const;

	void SetName(const CU_STR& name) { m_name = name; }
	const CU_STR& GetName() const { return m_name; }

	void SetEditable(bool editable) { m_editable = editable; }
	bool IsEditable() const { return m_editable; }

	void SetVisible(bool visible) { m_visible = visible; }
	bool IsVisible() const { return m_visible; }

	SpriteObserver& GetSpriteObserver() { return m_sprite_observer; }

	void Clear();

private:
	std::pair<std::map<int, KeyFrame*>::iterator, bool> 
		InsertKeyFrame(int index, KeyFrame* frame);

private:
	CU_STR m_name;

	bool m_editable, m_visible;

	std::map<int, KeyFrame*> m_frames;

	SpriteObserver m_sprite_observer;

}; // Layer

}

#endif // _EASYANIM_LAYER_H_