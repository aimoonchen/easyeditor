#include "PackNodeFactory.h"
#include "SymBuilder.h"
#include "SprBuilder.h"
#include "SprWithTransBuilder.h"
#include "SprBuilder2.h"
#include "LabelBuilder.h"
#include "ComplexBuilder.h"

#include "PackAnchor.h"
#include "PackImage.h"
#include "PackScale9.h"
#include "PackScale9Spr.h"
#include "PackIcon.h"
#include "PackIconSpr.h"
#include "PackTexture.h"
#include "PackTextureSpr.h"
#include "PackComplex.h"
#include "PackComplexSpr.h"
#include "PackAnimation.h"
#include "PackAnimationSpr.h"
#include "PackAnim2.h"
#include "PackAnim2Spr.h"
#include "PackParticle3d.h"
#include "PackParticle3dSpr.h"
#include "PackParticle2d.h"
#include "PackParticle2dSpr.h"
#include "PackShape.h"
#include "PackShapeSpr.h"
#include "PackMesh.h"
#include "PackMeshSpr.h"
#include "PackMask.h"
#include "PackMaskSpr.h"
#include "PackTrail.h"
#include "PackTrailSpr.h"
#include "PackAudio.h"
#include "PackAudioSpr.h"

#include <easyscale9.h>
#include <easyicon.h>
#include <easytexture.h>
#include <easytext.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyanim2.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easyshape.h>
#include <easymesh.h>
#include <easymask.h>
#include <easytrail.h>
#include <easyrespacker.h>
#include <easyaudio.h>

#include <ee/ImageSprite.h>
#include <ee/ImageSymbol.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>
#include <ee/FetchAllVisitor.h>

#include <simp/simp_types.h>

namespace esprpacker
{

CU_SINGLETON_DEFINITION(PackNodeFactory);

SymBuilder<ee::ImageSymbol, PackImage>*										IMAGE_BUILDER;
SymBuilder<escale9::Symbol, PackScale9>*									SCALE9_BUILDER;
SymBuilder<eicon::Symbol, PackIcon>*										ICON_BUILDER;
SymBuilder<etexture::Symbol, PackTexture>*									TEXTURE_BUILDER;
SymBuilder<libanim::Symbol, PackAnimation>*									ANIM_BUILDER;
SymBuilder<libanim2::Symbol, PackAnim2>*									ANIM2_BUILDER;
SymBuilder<eparticle3d::Symbol, PackParticle3d>*							P3D_BUILDER;
SymBuilder<eparticle2d::Symbol, PackParticle2d>*							P2D_BUILDER;
SymBuilder<eshape::Symbol, PackShape>*										SHAPE_BUILDER;
SymBuilder<emesh::Symbol, PackMesh>*										MESH_BUILDER;
SymBuilder<emask::Symbol, PackMask>*										MASK_BUILDER;
SymBuilder<etrail::Symbol, PackTrail>*										TRAIL_BUILDER;
SymBuilder<eaudio::Symbol, PackAudio>*										AUDIO_BUILDER;

//SprBuilder<escale9::Symbol, escale9::Sprite, PackScale9Spr>*				SCALE9_SPR_BUILDER;
//SprBuilder<eicon::Symbol, eicon::Sprite, PackIconSpr>*						ICON_SPR_BUILDER;
//SprBuilder<etexture::Symbol, etexture::Sprite, PackTextureSpr>*				TEXTURE_SPR_BUILDER;
//SprWithTransBuilder<ecomplex::Symbol, ecomplex::Sprite, PackComplexSpr>*	COMPLEX_SPR_BUILDER;
//SprWithTransBuilder<libanim::Symbol, libanim::Sprite, PackAnimationSpr>*	ANIM_SPR_BUILDER;
//SprBuilder<libanim2::Symbol, libanim2::Sprite, PackAnim2Spr>*				ANIM2_SPR_BUILDER;
//SprBuilder<eparticle3d::Symbol, eparticle3d::Sprite, PackParticle3dSpr>*	P3D_SPR_BUILDER;
//SprBuilder<eparticle2d::Symbol, eparticle2d::Sprite, PackParticle2dSpr>*	P2D_SPR_BUILDER;
//SprBuilder<eshape::Symbol, eshape::Sprite, PackShapeSpr>*					SHAPE_SPR_BUILDER;
//SprBuilder<emesh::Symbol, emesh::Sprite, PackMeshSpr>*						MESH_SPR_BUILDER;
//SprBuilder<emask::Symbol, emask::Sprite, PackMaskSpr>*						MASK_SPR_BUILDER;
//SprBuilder<etrail::Symbol, etrail::Sprite, PackTrailSpr>*					TRAIL_SPR_BUILDER;

SprBuilder2<escale9::Sprite, PackScale9Spr>*								SCALE9_SPR_BUILDER;
SprBuilder2<eicon::Sprite, PackIconSpr>*									ICON_SPR_BUILDER;
SprBuilder2<etexture::Sprite, PackTextureSpr>*								TEXTURE_SPR_BUILDER;
SprBuilder2<ecomplex::Sprite, PackComplexSpr>*								COMPLEX_SPR_BUILDER;
SprBuilder2<libanim::Sprite, PackAnimationSpr>*								ANIM_SPR_BUILDER;
SprBuilder2<libanim2::Sprite, PackAnim2Spr>*								ANIM2_SPR_BUILDER;
SprBuilder2<eparticle3d::Sprite, PackParticle3dSpr>*						P3D_SPR_BUILDER;
SprBuilder2<eparticle2d::Sprite, PackParticle2dSpr>*						P2D_SPR_BUILDER;
SprBuilder2<eshape::Sprite, PackShapeSpr>*									SHAPE_SPR_BUILDER;
SprBuilder2<emesh::Sprite, PackMeshSpr>*									MESH_SPR_BUILDER;
SprBuilder2<emask::Sprite, PackMaskSpr>*									MASK_SPR_BUILDER;
SprBuilder2<etrail::Sprite, PackTrailSpr>*									TRAIL_SPR_BUILDER;
SprBuilder2<eaudio::Sprite, PackAudioSpr>*									AUDIO_SPR_BUILDER;

PackNodeFactory::PackNodeFactory()
{
	IMAGE_BUILDER		= new SymBuilder<ee::ImageSymbol, PackImage>();
	SCALE9_BUILDER		= new SymBuilder<escale9::Symbol, PackScale9>();
	ICON_BUILDER		= new SymBuilder<eicon::Symbol, PackIcon>();
	TEXTURE_BUILDER		= new SymBuilder<etexture::Symbol, PackTexture>();
	ANIM_BUILDER		= new SymBuilder<libanim::Symbol, PackAnimation>(true);
	ANIM2_BUILDER		= new SymBuilder<libanim2::Symbol, PackAnim2>(true);
	P3D_BUILDER			= new SymBuilder<eparticle3d::Symbol, PackParticle3d>(true);
	P2D_BUILDER			= new SymBuilder<eparticle2d::Symbol, PackParticle2d>(true);
	SHAPE_BUILDER		= new SymBuilder<eshape::Symbol, PackShape>();
	MESH_BUILDER		= new SymBuilder<emesh::Symbol, PackMesh>();
	MASK_BUILDER		= new SymBuilder<emask::Symbol, PackMask>();
	TRAIL_BUILDER		= new SymBuilder<etrail::Symbol, PackTrail>(true);
	AUDIO_BUILDER		= new SymBuilder<eaudio::Symbol, PackAudio>();

// 	SCALE9_SPR_BUILDER	= new SprBuilder<escale9::Symbol, escale9::Sprite, PackScale9Spr>();
// 	ICON_SPR_BUILDER	= new SprBuilder<eicon::Symbol, eicon::Sprite, PackIconSpr>();
// 	TEXTURE_SPR_BUILDER	= new SprBuilder<etexture::Symbol, etexture::Sprite, PackTextureSpr>();
// 	COMPLEX_SPR_BUILDER	= new SprWithTransBuilder<ecomplex::Symbol, ecomplex::Sprite, PackComplexSpr>();
// 	ANIM_SPR_BUILDER	= new SprWithTransBuilder<libanim::Symbol, libanim::Sprite, PackAnimationSpr>();
// 	ANIM2_SPR_BUILDER	= new SprBuilder<libanim2::Symbol, libanim2::Sprite, PackAnim2Spr>();
// 	P3D_SPR_BUILDER		= new SprBuilder<eparticle3d::Symbol, eparticle3d::Sprite, PackParticle3dSpr>();
// 	P2D_SPR_BUILDER		= new SprBuilder<eparticle2d::Symbol, eparticle2d::Sprite, PackParticle2dSpr>();
// 	SHAPE_SPR_BUILDER	= new SprBuilder<eshape::Symbol, eshape::Sprite, PackShapeSpr>();
// 	MESH_SPR_BUILDER	= new SprBuilder<emesh::Symbol, emesh::Sprite, PackMeshSpr>();
// 	MASK_SPR_BUILDER	= new SprBuilder<emask::Symbol, emask::Sprite, PackMaskSpr>();
// 	TRAIL_SPR_BUILDER	= new SprBuilder<etrail::Symbol, etrail::Sprite, PackTrailSpr>();

	SCALE9_SPR_BUILDER	= new SprBuilder2<escale9::Sprite, PackScale9Spr>();
	ICON_SPR_BUILDER	= new SprBuilder2<eicon::Sprite, PackIconSpr>();
	TEXTURE_SPR_BUILDER	= new SprBuilder2<etexture::Sprite, PackTextureSpr>();
	COMPLEX_SPR_BUILDER	= new SprBuilder2<ecomplex::Sprite, PackComplexSpr>();
	ANIM_SPR_BUILDER	= new SprBuilder2<libanim::Sprite, PackAnimationSpr>();
	ANIM2_SPR_BUILDER	= new SprBuilder2<libanim2::Sprite, PackAnim2Spr>();
	P3D_SPR_BUILDER		= new SprBuilder2<eparticle3d::Sprite, PackParticle3dSpr>();
	P2D_SPR_BUILDER		= new SprBuilder2<eparticle2d::Sprite, PackParticle2dSpr>();
	SHAPE_SPR_BUILDER	= new SprBuilder2<eshape::Sprite, PackShapeSpr>();
	MESH_SPR_BUILDER	= new SprBuilder2<emesh::Sprite, PackMeshSpr>();
	MASK_SPR_BUILDER	= new SprBuilder2<emask::Sprite, PackMaskSpr>();
	TRAIL_SPR_BUILDER	= new SprBuilder2<etrail::Sprite, PackTrailSpr>();
	AUDIO_SPR_BUILDER	= new SprBuilder2<eaudio::Sprite, PackAudioSpr>();
}

PackNodeFactory::~PackNodeFactory()
{
}

const PackNode* PackNodeFactory::Create(const ee::SprConstPtr& spr, bool force_curr)
{
	const PackNode* node = NULL;

	if (spr->IsAnchor()) {
		node = new PackAnchor();

		const std::string& filepath = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath();	
		erespacker::PackUI::Instance()->OnKnownPackID(filepath, 0xffffffff);
		erespacker::PackTag::Instance()->OnKnownPackID(filepath, 0xffffffff);
	}
	// image
	else if (auto image = std::dynamic_pointer_cast<const ee::ImageSprite>(spr)) {
		node = IMAGE_BUILDER->Create(std::dynamic_pointer_cast<ee::ImageSymbol>(image->GetSymbol()));
	}
	// scale9 spr
	else if (auto s9 = std::dynamic_pointer_cast<const escale9::Sprite>(spr)) {
		node = SCALE9_SPR_BUILDER->Create(s9, force_curr);
	}
	// icon spr
	else if (auto icon = std::dynamic_pointer_cast<const eicon::Sprite>(spr)) {
		node = ICON_SPR_BUILDER->Create(icon, force_curr);
	}
	// texture
	else if (auto texture = std::dynamic_pointer_cast<const etexture::Sprite>(spr)) {
		node = TEXTURE_SPR_BUILDER->Create(texture, force_curr);
	}
	// label
	else if (auto label = std::dynamic_pointer_cast<const etext::Sprite>(spr)) {
		node = LabelBuilder::Instance()->Create(label);
	}
	// complex
	else if (auto complex = std::dynamic_pointer_cast<const ecomplex::Sprite>(spr)) {
		node = COMPLEX_SPR_BUILDER->Create(complex, force_curr);
	}
	// animation
	else if (auto anim = std::dynamic_pointer_cast<const libanim::Sprite>(spr)) {
		node = ANIM_SPR_BUILDER->Create(anim, force_curr);
	}
	// anim2
	else if (auto anim2 = std::dynamic_pointer_cast<const libanim2::Sprite>(spr)) {
		node = ANIM2_SPR_BUILDER->Create(anim2, force_curr);
	}
	// particle3d
	else if (auto p3d = std::dynamic_pointer_cast<const eparticle3d::Sprite>(spr)) {
		node = P3D_SPR_BUILDER->Create(p3d, force_curr);
	}
	// particle2d
	else if (auto p2d = std::dynamic_pointer_cast<const eparticle2d::Sprite>(spr)) {
		node = P2D_SPR_BUILDER->Create(p2d, force_curr);
	}
	// shape
	else if (auto shape = std::dynamic_pointer_cast<const eshape::Sprite>(spr)) {
		node = SHAPE_SPR_BUILDER->Create(shape, force_curr);
	}
	// mesh
	else if (auto mesh = std::dynamic_pointer_cast<const emesh::Sprite>(spr)) {
		node = MESH_SPR_BUILDER->Create(mesh, force_curr);
	}
	// mask
	else if (auto mask = std::dynamic_pointer_cast<const emask::Sprite>(spr)) {
		node = MASK_SPR_BUILDER->Create(mask, force_curr);
	}
	// trail
	else if (auto trail = std::dynamic_pointer_cast<const etrail::Sprite>(spr)) {
		node = TRAIL_SPR_BUILDER->Create(trail, force_curr);
	}
	// audio
	else if (auto audio = std::dynamic_pointer_cast<const eaudio::Sprite>(spr)) {
		node = AUDIO_SPR_BUILDER->Create(audio, force_curr);
	}

	else {
		throw ee::Exception("PackNodeFactory::Create unknown sprite type.");
	}

	assert(node && (spr->IsAnchor() || node->GetID() != 0xffffffff));

	return node;
}

const PackNode* PackNodeFactory::Create(const ee::SymConstPtr& sym)
{
	const PackNode* node = NULL;

	// image
	if (auto image = std::dynamic_pointer_cast<const ee::ImageSymbol>(sym)) {
		node = IMAGE_BUILDER->Create(image);
	}
	// scale9
	else if (auto s9 = std::dynamic_pointer_cast<const escale9::Symbol>(sym)) {
		node = SCALE9_BUILDER->Create(s9);
	}
	// icon
	else if (auto icon = std::dynamic_pointer_cast<const eicon::Symbol>(sym)) {
		node = ICON_BUILDER->Create(icon);
	}
	// texture
	else if (auto texture = std::dynamic_pointer_cast<const etexture::Symbol>(sym)) {
		node = TEXTURE_BUILDER->Create(texture);
	}
	// complex
	else if (auto complex = std::dynamic_pointer_cast<const ecomplex::Symbol>(sym)) {
		node = ComplexBuilder::Instance()->Create(complex);
	}
	// anim
	else if (auto anim = std::dynamic_pointer_cast<const libanim::Symbol>(sym)) {
		node = ANIM_BUILDER->Create(anim);
	}
	// anim2
	else if (auto anim2 = std::dynamic_pointer_cast<const libanim2::Symbol>(sym)) {
		node = ANIM2_BUILDER->Create(anim2);
	}
	// particle3d
	else if (auto p3d = std::dynamic_pointer_cast<const eparticle3d::Symbol>(sym)) {
		node = P3D_BUILDER->Create(p3d);
	}
	// particle2d
	else if (auto p2d = std::dynamic_pointer_cast<const eparticle2d::Symbol>(sym)) {
		node = P2D_BUILDER->Create(p2d);
	}
	// shape
	else if (auto shape = std::dynamic_pointer_cast<const eshape::Symbol>(sym)) {
		node = SHAPE_BUILDER->Create(shape);
	}
	// mesh
	else if (auto mesh = std::dynamic_pointer_cast<const emesh::Symbol>(sym)) {
		node = MESH_BUILDER->Create(mesh);
	}
	// mask
	else if (auto mask = std::dynamic_pointer_cast<const emask::Symbol>(sym)) {
		node = MASK_BUILDER->Create(mask);
	}
	// trail
	else if (auto trail = std::dynamic_pointer_cast<const etrail::Symbol>(sym)) {
		node = TRAIL_BUILDER->Create(trail);
	}
	// audio
	else if (auto audio = std::dynamic_pointer_cast<const eaudio::Symbol>(sym)) {
		node = AUDIO_BUILDER->Create(audio);
	}

	else {
		throw ee::Exception("PackNodeFactory::Create unknown symbol type.");
	}

	assert(node && node->GetID() != 0xffffffff);

	erespacker::PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetID());
	erespacker::PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetID());	

	return node;
}

void PackNodeFactory::FetchAll(std::vector<PackNode*>& nodes) const
{
	nodes.clear();

	std::vector<NodeBuilder*> builders;
	FetchAllBuilder(builders);
	for (int i = 0, n = builders.size(); i < n; ++i) {
		builders[i]->Traverse(ee::FetchAllVisitor<PackNode>(nodes));
	}
}

void PackNodeFactory::Clear()
{
	std::vector<NodeBuilder*> builders;
	FetchAllBuilder(builders);
	for (int i = 0, n = builders.size(); i < n; ++i) {
		builders[i]->Clear();
	}
}

void PackNodeFactory::FetchAllBuilder(std::vector<NodeBuilder*>& builders)
{
	builders.clear();

	builders.push_back(IMAGE_BUILDER);
	builders.push_back(SCALE9_BUILDER);
	builders.push_back(ICON_BUILDER);
	builders.push_back(TEXTURE_BUILDER);
	builders.push_back(LabelBuilder::Instance());
	builders.push_back(ComplexBuilder::Instance());
	builders.push_back(ANIM_BUILDER);
	builders.push_back(ANIM2_BUILDER);
	builders.push_back(P3D_BUILDER);
	builders.push_back(P2D_BUILDER);
	builders.push_back(SHAPE_BUILDER);
	builders.push_back(MESH_BUILDER);
	builders.push_back(MASK_BUILDER);
	builders.push_back(TRAIL_BUILDER);
	builders.push_back(AUDIO_BUILDER);

	builders.push_back(SCALE9_SPR_BUILDER);
	builders.push_back(ICON_SPR_BUILDER);
	builders.push_back(TEXTURE_SPR_BUILDER);
	builders.push_back(COMPLEX_SPR_BUILDER);
	builders.push_back(ANIM_SPR_BUILDER);
	builders.push_back(ANIM2_SPR_BUILDER);
	builders.push_back(P3D_SPR_BUILDER);
	builders.push_back(P2D_SPR_BUILDER);
	builders.push_back(SHAPE_SPR_BUILDER);
	builders.push_back(MESH_SPR_BUILDER);
	builders.push_back(MASK_SPR_BUILDER);
	builders.push_back(TRAIL_SPR_BUILDER);
	builders.push_back(AUDIO_SPR_BUILDER);
}

}