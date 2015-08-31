#include "PackNodeFactory.h"

#include "ImageBuilder.h"
#include "Scale9Builder.h"
#include "IconBuilder.h"
#include "TextureBuilder.h"

#include "LabelBuilder.h"

#include "ComplexBuilder.h"
#include "AnimBuilder.h"

namespace libcoco
{

PackNodeFactory* PackNodeFactory::m_instance = NULL;

PackNodeFactory::PackNodeFactory()
{
	m_builders.push_back(m_img_builder = new ImageBuilder);
	m_builders.push_back(m_scale9_builder = new Scale9Builder);
	m_builders.push_back(m_icon_builder = new IconBuilder);
	m_builders.push_back(m_tex_builder = new TextureBuilder);

	m_builders.push_back(m_label_builder = new LabelBuilder);

	m_builders.push_back(m_complex_builder = new ComplexBuilder(m_export_set));
	m_builders.push_back(m_anim_builder = new AnimBuilder(m_export_set));
}

const IPackNode* PackNodeFactory::Create(const d2d::ISprite* spr)
{
	const IPackNode* node = NULL;
	
	if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(spr)) {
		node = m_img_builder->Create(image);
	} else if (const escale9::Sprite* scale9 = dynamic_cast<const escale9::Sprite*>(spr)) {
		node = m_scale9_builder->Create(scale9);
	} else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr)) {
		node = m_icon_builder->Create(icon);
	} else if (const etexture::Sprite* tex = dynamic_cast<const etexture::Sprite*>(spr)) {
		node = m_tex_builder->Create(&tex->GetSymbol());
	}

	else if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(spr)) {
		node = m_label_builder->Create(font);
	} 

	else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(spr)) {
		node = m_complex_builder->Create(&complex->GetSymbol());
	} else if (const libanim::Sprite* anim = dynamic_cast<const libanim::Sprite*>(spr)) {
		node = m_anim_builder->Create(&anim->GetSymbol());
	}

	return node;
}

void PackNodeFactory::CreateComplex(const ecomplex::Symbol* complex)
{
	m_complex_builder->Create(complex);
}

void PackNodeFactory::CreateAnim(const libanim::Symbol* anim)
{
	m_anim_builder->Create(anim);
}

void PackNodeFactory::ToString(ebuilder::CodeGenerator& gen,
							   const TexturePacker& tp) const
{
	for (int i = 0, n = m_builders.size(); i < n; ++i) {
		m_builders[i]->ToString(gen, tp);
	}
}

PackNodeFactory* PackNodeFactory::Instance()
{
	if (!m_instance) {
		m_instance = new PackNodeFactory;
	}
	return m_instance;
}

}