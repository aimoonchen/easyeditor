#include "CombineAOP.h"

namespace ee
{

void CombineAOP::Insert(const std::shared_ptr<AtomicOP>& atomic)
{
	m_atomics.push_back(atomic);
}

void CombineAOP::Undo()
{
	for (int i = m_atomics.size() - 1; i >= 0; --i) {
		m_atomics[i]->Undo();
	}
}

void CombineAOP::Redo()
{
	for (int i = 0, n = m_atomics.size(); i < n; ++i) {
		m_atomics[i]->Redo();
	}
}

// todo
Json::Value CombineAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value ret;
	return ret;
}

}