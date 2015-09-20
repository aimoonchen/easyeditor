#ifndef _LIBCOCO_COMPLEX_BUILDER_H_
#define _LIBCOCO_COMPLEX_BUILDER_H_

#include "INodeBuilder.h"

#include <drag2d.h>

namespace ecomplex { class Symbol; }

namespace libcoco
{

class IPackNode;
class PackAnimation;
class ExportNameSet;

class ComplexBuilder : public INodeBuilder
{
public:
	ComplexBuilder(ExportNameSet& export_set);
	virtual ~ComplexBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const ecomplex::Symbol* symbol);

private:
	void Load(const ecomplex::Symbol* symbol, PackAnimation* anim);

	static void GroupFromTag(const std::vector<d2d::ISprite*>& src, 
		std::map<std::string, std::vector<d2d::ISprite*> >& dst,
		std::vector<d2d::ISprite*>& others);

private:
	ExportNameSet& m_export_set;

	std::map<const ecomplex::Symbol*, const PackAnimation*> m_map_data;

}; // ComplexBuilder

}

#endif // _LIBCOCO_COMPLEX_BUILDER_H_
