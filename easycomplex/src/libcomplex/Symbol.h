#ifndef _EASYCOMPLEX_SYMBOL_H_
#define _EASYCOMPLEX_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/ComplexSymbol.h>

#include <vector>

namespace ecomplex
{

class Symbol : public ee::Symbol, public s2::ComplexSymbol
{
public:
	Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual s2::RenderReturn Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;
	virtual void Traverse(ee::Visitor<ee::Sprite>& visitor);

	bool HasActions() const;
	void GetActionNames(std::vector<std::string>& actions) const;

	const std::vector<std::string>& GetOriginNames() const { return m_origin_names; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources();

public:
	struct Group
	{
		std::string name;
		std::vector<ee::Sprite*> members;
	};

	// todo: 

private:
	bool CullingTestOutside(const s2::Sprite* spr, const s2::RenderParams& rp) const;

public:
	std::vector<Group> m_groups;

	std::vector<std::string> m_origin_names;

	bool m_use_render_cache;

	mutable int m_render_version;
	mutable bool m_render_cache_open;

}; // Symbol

}

#endif // _EASYCOMPLEX_SYMBOL_H_