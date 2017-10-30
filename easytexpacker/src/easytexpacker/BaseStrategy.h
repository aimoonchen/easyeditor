#ifndef _EASYTEXPACKER_BASE_STRATEGY_H_
#define _EASYTEXPACKER_BASE_STRATEGY_H_

#include <vector>

namespace ee { class ImageSprite; }

namespace etexpacker
{

class BaseStrategy
{
public:
	virtual ~BaseStrategy() {}

	virtual void Arrange(const std::vector<ee::ImageSprite*>& sprs) = 0;
	virtual int GetTextureAccount() const = 0;

protected:
	void SortByArea(std::vector<ee::ImageSprite*>& sprs, bool isDescend = true) const;
	void SortByMaxEdge(std::vector<ee::ImageSprite*>& sprs, bool isDescend = true) const;
	void SortByTotEdges(std::vector<ee::ImageSprite*>& sprs, bool isDescend = true) const;

protected:
	enum SortStrategy
	{
		e_area,
		e_maxEdge,
		e_totEdges
	};

	class SpriteCmp
	{
	public:
		SpriteCmp(SortStrategy strategy, bool isDescend);

		bool operator() (const std::shared_ptr<const ee::ImageSprite>& s0, const std::shared_ptr<const ee::ImageSprite>& s1) const;

	private:
		bool IsAreaLess(const std::shared_ptr<const ee::ImageSprite>& s0, const std::shared_ptr<const ee::ImageSprite>& s1) const;
		bool IsEdgeLess(const std::shared_ptr<const ee::ImageSprite>& s0, const std::shared_ptr<const ee::ImageSprite>& s1) const;
		bool IsTotEdgesLess(const std::shared_ptr<const ee::ImageSprite>& s0, const std::shared_ptr<const ee::ImageSprite>& s1) const;

	private:
		SortStrategy m_strategy;
		bool m_is_descend;

	}; // SpriteCmp

}; // BaseStrategy

}

#endif // _EASYTEXPACKER_BASE_STRATEGY_H_