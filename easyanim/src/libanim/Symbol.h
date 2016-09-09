#ifndef _EASYANIM_SYMBOL_H_
#define _EASYANIM_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/AnimSymbol.h>

#include <vector>

namespace eanim
{

class LayersLoader;
class Sprite;

class Symbol : public ee::Symbol, public s2::AnimSymbol
{
public:
	Symbol();

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;
	virtual void Traverse(ee::Visitor<ee::Sprite>& visitor);

	void LoadFromFile(LayersLoader& loader);

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

}; // Symbol

}

#endif // _EASYANIM_SYMBOL_H_