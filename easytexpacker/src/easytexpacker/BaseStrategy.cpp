#include "BaseStrategy.h"
#include "Context.h"

#include <ee/ImageSprite.h>

#include <algorithm>

namespace etexpacker
{

void BaseStrategy::SortByArea(std::vector<ee::ImageSprite*>& sprs, bool isDescend/* = true*/) const
{
	std::sort(sprs.begin(), sprs.end(), SpriteCmp(e_area, isDescend));
}

void BaseStrategy::SortByMaxEdge(std::vector<ee::ImageSprite*>& sprs, bool isDescend/* = true*/) const
{
	std::sort(sprs.begin(), sprs.end(), SpriteCmp(e_maxEdge, isDescend));
}

void BaseStrategy::SortByTotEdges(std::vector<ee::ImageSprite*>& sprs, bool isDescend/* = true*/) const
{
	std::sort(sprs.begin(), sprs.end(), SpriteCmp(e_totEdges, isDescend));
}

BaseStrategy::SpriteCmp::SpriteCmp(SortStrategy strategy, bool isDescend)
{
	m_strategy = strategy;
	m_is_descend = isDescend;
}

bool BaseStrategy::SpriteCmp::operator() (const std::shared_ptr<const ee::ImageSprite>& s0, const std::shared_ptr<const ee::ImageSprite>& s1) const
{
	bool isLess;

	switch (m_strategy)
	{
	case e_area:
		isLess = IsAreaLess(s0, s1);
		break;
	case e_maxEdge:
		isLess = IsEdgeLess(s0, s1);
		break;
	case e_totEdges:
		isLess = IsTotEdgesLess(s0, s1);
		break;
	}

	if (m_is_descend) return !isLess;
	else return isLess;
}

bool BaseStrategy::SpriteCmp::IsAreaLess(const std::shared_ptr<const ee::ImageSprite>& s0, const std::shared_ptr<const ee::ImageSprite>& s1) const
{
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;

	return (s0->GetSymbol()->GetBounding().Width() * s + p) * (s0->GetSymbol().GetBounding().Height() * s + p)
		<= (s1->GetSymbol()->GetBounding().Width() * s + p) * (s1->GetSymbol().GetBounding().Height() * s + p);
}

bool BaseStrategy::SpriteCmp::IsEdgeLess(const std::shared_ptr<const ee::ImageSprite>& s0, const std::shared_ptr<const ee::ImageSprite>& s1) const
{
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;

	return std::max((s0->GetSymbol()->GetBounding().Width() * s + p), (s0->GetSymbol().GetBounding().Height() * s + p))
		<= std::max((s1->GetSymbol()->GetBounding().Width() * s + p), (s1->GetSymbol().GetBounding().Height() * s + p));
}

bool BaseStrategy::SpriteCmp::IsTotEdgesLess(const std::shared_ptr<const ee::ImageSprite>& s0, const std::shared_ptr<const ee::ImageSprite>& s1) const
{
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;

	return (s0->GetSymbol()->GetBounding().Width() * s + p) + (s0->GetSymbol().GetBounding().Height() * s + p)
		<= (s1->GetSymbol()->GetBounding().Width() * s + p) + (s1->GetSymbol().GetBounding().Height() * s + p);
}

}