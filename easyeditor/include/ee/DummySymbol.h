#ifndef _EASYEDITOR_DUMMY_SYMBOL_H_
#define _EASYEDITOR_DUMMY_SYMBOL_H_

#include "Symbol.h"

namespace ee
{

class DummySymbol : public Symbol
{
public:
	DummySymbol(const std::string& filepath);
	DummySymbol(const std::string& filepath, int width, int height);

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual int Type() const { return -1; }
	virtual void Traverse(const s2::SymbolVisitor& visitor) {}
	virtual s2::RenderReturn Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const { return s2::RENDER_NO_DATA; }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources() { return true; }

private:
	sm::rect m_bounding;

}; // DummySymbol

}

#endif // _EASYEDITOR_DUMMY_SYMBOL_H_