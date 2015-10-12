#ifndef _LIBRESPACKER_TERRAIN_BUILDER_H_
#define _LIBRESPACKER_TERRAIN_BUILDER_H_

#include "INodeBuilder.h"

#include <map>
#include <vector>

namespace eterrain2d { class Symbol; }

namespace librespacker
{

class IPackNode;
class PackAnimation;
class PackPicture;

class Terrain2DBuilder : public INodeBuilder
{
public:
	Terrain2DBuilder();
	virtual ~Terrain2DBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const eterrain2d::Symbol* symbol);

private:
	void Load(const eterrain2d::Symbol* symbol, PackAnimation* anim);

private:
	std::map<const eterrain2d::Symbol*, const PackAnimation*> m_map_data;

	std::vector<const PackPicture*> m_pics;

}; // Terrain2DBuilder

}

#endif // _LIBRESPACKER_TERRAIN_BUILDER_H_