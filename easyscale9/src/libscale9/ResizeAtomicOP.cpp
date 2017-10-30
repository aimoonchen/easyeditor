#include "ResizeAtomicOP.h"
#include "Symbol.h"

namespace escale9
{

ResizeAtomicOP::ResizeAtomicOP(const std::shared_ptr<Symbol>& sym,
	const sm::vec2& src, const sm::vec2& dst)
	: m_sym(sym)
	, m_src(src)
	, m_dst(dst)
{
}

void ResizeAtomicOP::Undo()
{
	m_sym->Resize(m_src.x, m_src.y);
}

void ResizeAtomicOP::Redo()
{
	m_sym->Resize(m_dst.x, m_dst.y);
}

}