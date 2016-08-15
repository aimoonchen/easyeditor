#ifndef _EASYTERRAIN2D_SYMBOL_H_
#define _EASYTERRAIN2D_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/DummySymbol.h>

#include <vector>

#include <time.h>

namespace eterrain2d
{

class OceanMesh;

class Symbol : public ee::Symbol, public s2::DummySymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* sprite = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

	std::vector<OceanMesh*>& GetOceans() { return m_oceans; }
	const std::vector<OceanMesh*>& GetOceans() const { return m_oceans; }

	void SetUpdateOpen(bool open) { m_update = open; }

	float GetOceanAngle() const { return m_angle; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void Clear();

private:
	bool m_update;

	std::vector<OceanMesh*> m_oceans;

	float m_angle;

	mutable clock_t m_time;

}; // Symbol

}

#endif // _EASYTERRAIN2D_SYMBOL_H_