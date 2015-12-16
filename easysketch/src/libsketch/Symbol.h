#ifndef _LIBSKETCH_SYMBOL_H_
#define _LIBSKETCH_SYMBOL_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// IObject interface
	//	
	virtual Symbol* Clone() const;

	//
	// ISymbol interface
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::ISprite* spr = NULL, const d2d::ISprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const;

	static d2d::ISymbol* Create() { return new Symbol(); }

	void SetModel(e3d::IModel* model);

	void SetAABB(const e3d::AABB& aabb) { m_aabb = aabb; }
	const e3d::AABB& GetAABB() const { return m_aabb; }

protected:
	virtual void LoadResources();

private:
	e3d::IModel* m_model;

	e3d::AABB m_aabb;

}; // Symbol

}

#endif // _LIBSKETCH_SYMBOL_H_