#ifndef _EASYEDITOR_SCRIPTS_SYMBOL_H_
#define _EASYEDITOR_SCRIPTS_SYMBOL_H_

#include "Symbol.h"

namespace ee
{

class ScriptsSymbol : public Symbol
{
public:
	ScriptsSymbol() {}
	virtual ~ScriptsSymbol() {}

	//
	// Cloneable interface
	//
	virtual ScriptsSymbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const {}
	virtual void Draw(const Matrix& mt, const ColorTrans& col = ColorTrans(), 
		const Sprite* spr = NULL, const Sprite* root = NULL) const {}
	virtual Rect GetSize(const Sprite* sprite = NULL) const;

	const std::string GetContent() const { return m_data; }

protected:
	virtual void LoadResources();

private:
	std::string m_data;

}; // ScriptsSymbol

}

#endif // _EASYEDITOR_SCRIPTS_SYMBOL_H_