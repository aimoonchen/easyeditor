#ifndef _EASYPARTICLE3D_SYMBOL_H_
#define _EASYPARTICLE3D_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Particle3dSymbol.h>

struct p3d_emitter_cfg;
struct p3d_emitter;

namespace eparticle3d
{

class ParticleSystem;

class Symbol : public ee::Symbol, public s2::Particle3dSymbol
{
public:
	Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;
	virtual void Traverse(ee::Visitor<ee::Sprite>& visitor);

	void SetEmitterCfg(p3d_emitter_cfg* cfg) { m_et_cfg = cfg; }
	const p3d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }

	p3d_emitter* GetEmitter() { return m_et; }

	bool IsLoop() const { return m_loop; }
	bool IsLocal() const { return m_local; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	p3d_emitter_cfg* m_et_cfg;

	p3d_emitter* m_et;

	bool m_loop, m_local;
	
}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_SYMBOL_H_
