#include "ComplexSprite.h"
#include "ComplexSymbol.h"

namespace s2
{

ComplexSprite::ComplexSprite()
	: m_sym(NULL)
{
}

bool ComplexSprite::Update(float dt) const
{
	if (!m_sym) {
		return false;
	}

	bool dirty = false;
	const std::vector<Sprite*>& children = m_sym->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		if (children[i]->Update(dt)) {
			dirty = true;
		}
	}
	return dirty;
}

}