#ifndef _LIBSHAPE_COLOR_MATERIAL_H_
#define _LIBSHAPE_COLOR_MATERIAL_H_

#include "Material.h"

namespace libshape
{

class ColorMaterial : public Material
{
public:
	ColorMaterial(const std::vector<d2d::Vector>& vertices,
		const d2d::Colorf& color);

	virtual Json::Value Store(const std::string& dirpath) const;

	virtual void Draw(const d2d::Matrix& mt) const;

protected:
	virtual void Build(const std::vector<d2d::Vector>& vertices);
	virtual void Build(const std::vector<d2d::Vector>& vertices,
		const std::vector<d2d::Vector>& segments);

private:
	d2d::Colorf m_color;

}; // ColorMaterial

}

#endif // _LIBSHAPE_COLOR_MATERIAL_H_