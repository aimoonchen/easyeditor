#include "MirrorSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"

namespace ee
{

MirrorSpriteAOP::MirrorSpriteAOP(Sprite* spr, 
								 bool old_mirror_x, bool old_mirror_y,
								 bool new_mirror_x, bool new_mirror_y)
	: m_spr(spr)
	, m_old_mirror_x(old_mirror_x)
	, m_old_mirror_y(old_mirror_y)
	, m_new_mirror_x(new_mirror_x)
	, m_new_mirror_y(new_mirror_y)
{
	m_spr->AddReference();
}

MirrorSpriteAOP::~MirrorSpriteAOP()
{
	m_spr->RemoveReference();
}

void MirrorSpriteAOP::Undo()
{
	m_spr->SetMirror(sm::bvec2(m_old_mirror_x, m_old_mirror_y));
}

void MirrorSpriteAOP::Redo()
{
	m_spr->SetMirror(sm::bvec2(m_new_mirror_x, m_new_mirror_y));
}

Json::Value MirrorSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value val;
	val["type"] = AT_SHEAR;
	val["new_mirror_x"] = m_new_mirror_x;
	val["new_mirror_y"] = m_new_mirror_y;
	val["old_mirror_x"] = m_old_mirror_x;
	val["old_mirror_y"] = m_old_mirror_y;
	return val;
}

}