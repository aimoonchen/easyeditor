#include "AnchorBuilder.h"
#include "PackAnchor.h"

namespace librespacker
{

AnchorBuilder::AnchorBuilder()
{
}

AnchorBuilder::~AnchorBuilder()
{
	for_each(m_anchors.begin(), m_anchors.end(), DeletePointerFunctor<const PackAnchor>());
}

void AnchorBuilder::Traverse(ee::Visitor& visitor) const
{
}

const IPackNode* AnchorBuilder::Create(const ee::Sprite* spr)
{
	return new PackAnchor();
}

}