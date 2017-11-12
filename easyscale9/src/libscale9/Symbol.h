#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Scale9Symbol.h>
#include <gum/Scale9SymLoader.h>

#include <json/json.h>

namespace escale9
{

class Symbol : public ee::Symbol, public s2::Scale9Symbol
{
public:
	Symbol();

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual bool LoadResources() override;

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_