#pragma once

#include <ee/Symbol.h>

#include <sprite2/ModelSymbol.h>

namespace ee3
{

class NodeSymbol : public ee::Symbol, public s2::ModelSymbol
{
public:

	static ee::SymPtr Create() { return std::make_shared<NodeSymbol>(); }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const override;

	virtual bool LoadResources() override;

private:
	bool LoadModelFromFile(const std::string& filepath);

}; // NodeSymbol

}