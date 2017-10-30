#include "AnimBuilder.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "PackUI.h"
#include "PackTag.h"

#include <ee/Visitor.h>

#include <easyanim.h>

namespace erespacker
{

AnimBuilder::AnimBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

AnimBuilder::~AnimBuilder()
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void AnimBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
 	auto itr = m_map_data.begin();
 	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackAnimation*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
 	}
}

const IPackNode* AnimBuilder::Create(const std::shared_ptr<const libanim::Symbol>& sym)
{
	auto itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackAnimation* node = new PackAnimation;
	Load(sym, node);
	m_map_data.insert(std::make_pair(sym, node));

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

void AnimBuilder::Load(const std::shared_ptr<const libanim::Symbol>& sym, PackAnimation* anim)
{
	m_export_set.LoadExport(sym, anim);

	int idx = 0;
	for (int i = 1, n = sym->GetMaxFrameIdx(); i <= n; ++i)
	{
		PackAnimation::Frame frame;

		CU_VEC<s2::SprPtr> sprs;
		sym->CreateFrameSprites(i, sprs);
		for (int i = 0, n = sprs.size(); i < n; ++i) {
			auto& spr = std::dynamic_pointer_cast<ee::Sprite>(sprs[i]);
			bool new_comp = anim->CreateFramePart(spr, frame);
			if (new_comp) {
				PackTag::Instance()->AddTask(sym->GetFilepath(), idx++, spr);
			}
		}

		anim->frames.push_back(frame);
	}

	PackAnimation::Action action;
	action.size = anim->frames.size();
	anim->actions.push_back(action);
}

}