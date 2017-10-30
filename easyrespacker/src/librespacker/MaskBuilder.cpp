#include "MaskBuilder.h"
#include "PackMask.h"
#include "PackNodeFactory.h"
#include "PackUI.h"
#include "PackTag.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>

#include <easymask.h>

#include <algorithm>

namespace erespacker
{

MaskBuilder::~MaskBuilder()
{
	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<IPackNode>());
}

void MaskBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(m_nodes[i], has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* MaskBuilder::Create(const std::shared_ptr<emask::Symbol>& sym)
{
	PackMask* node = new PackMask;

	node->base = PackNodeFactory::Instance()->Create(std::dynamic_pointer_cast<ee::Sprite>(sym->GetBase()));
	node->mask = PackNodeFactory::Instance()->Create(std::dynamic_pointer_cast<ee::Sprite>(sym->GetMask()));

	m_nodes.push_back(node);

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

}