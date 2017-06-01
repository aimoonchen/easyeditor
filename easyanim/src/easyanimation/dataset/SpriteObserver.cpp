#include "SpriteObserver.h"
#include "KeyFrame.h"
#include "Layer.h"

#include <ee/Sprite.h>

namespace eanim
{

SpriteObserver::SpriteObserver(const Layer& layer)
	: m_layer(layer)
	, m_enable(false)
{
}

void SpriteObserver::OnSetPosition(ee::Sprite* spr, const sm::vec2& pos)
{
	if (!m_enable) {
		return;
	}

	std::map<ee::Sprite*, int>::iterator itr_sprite = m_map2frame.find(spr);
	if (itr_sprite == m_map2frame.end()) {
		return;
	}

	m_enable = false;

	sm::vec2 trans = pos - spr->GetPosition();
	const std::map<int, KeyFrame*>& frames = m_layer.GetAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<ee::Sprite*>& sprs = frame->GetAllSprites();
		for (int i = 0, n = sprs.size(); i < n; ++i) {
			if (sprs[i]->GetName() == spr->GetName()) {
				sprs[i]->Translate(trans);
			}
		}
	}

	m_enable = true;
}

void SpriteObserver::OnSetAngle(ee::Sprite* spr, float angle)
{
	if (!m_enable) {
		return;
	}

	std::map<ee::Sprite*, int>::iterator itr_sprite = m_map2frame.find(spr);
	if (itr_sprite == m_map2frame.end())
		return;

	m_enable = false;

	float rot = angle - spr->GetAngle();
	const std::map<int, KeyFrame*>& frames = m_layer.GetAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<ee::Sprite*>& sprs = frame->GetAllSprites();
		for (int i = 0, n = sprs.size(); i < n; ++i)
		{
			if (sprs[i]->GetName() == spr->GetName())
				sprs[i]->Rotate(rot);
		}
	}

	m_enable = true;
}

void SpriteObserver::OnSetScale(ee::Sprite* spr, const sm::vec2& scale)
{
	if (!m_enable) {
		return;
	}

	std::map<ee::Sprite*, int>::iterator itr_sprite = m_map2frame.find(spr);
	if (itr_sprite == m_map2frame.end())
		return;

	m_enable = false;

	sm::vec2 d_scale = scale / spr->GetScale();
	const std::map<int, KeyFrame*>& frames = m_layer.GetAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<ee::Sprite*>& sprs = frame->GetAllSprites();
		for (int i = 0, n = sprs.size(); i < n; ++i)
		{
			if (sprs[i]->GetName() == spr->GetName())
				sprs[i]->Scale(d_scale);
		}
	}

	m_enable = true;
}

void SpriteObserver::OnSetShear(ee::Sprite* spr, const sm::vec2& shear)
{
	
}

void SpriteObserver::OnSetOffset(ee::Sprite* spr, const sm::vec2& offset)
{
	
}

void SpriteObserver::Insert(const ee::Sprite* spr, int frame)
{
	m_map2frame.insert(std::make_pair(const_cast<ee::Sprite*>(spr), frame));
}

void SpriteObserver::Remove(const ee::Sprite* spr)
{
	std::map<ee::Sprite*, int>::iterator itr 
		= m_map2frame.find(const_cast<ee::Sprite*>(spr));
	if (itr != m_map2frame.end())
		m_map2frame.erase(itr);
}

}