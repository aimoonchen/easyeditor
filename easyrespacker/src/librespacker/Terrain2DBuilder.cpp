#include "Terrain2DBuilder.h"
#include "PackAnimation.h"
#include "PackPicture.h"
#include "PackUI.h"
#include "PackTag.h"

#include <ee/Exception.h>
#include <ee/ImageSymbol.h>

#include <easyterrain2d.h>

namespace erespacker
{

Terrain2DBuilder::Terrain2DBuilder()
{
}

Terrain2DBuilder::~Terrain2DBuilder()
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void Terrain2DBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackAnimation*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}

	for (int i = 0, n = m_pics.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(const_cast<PackPicture*>(m_pics[i]), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* Terrain2DBuilder::Create(const std::shared_ptr<const eterrain2d::Symbol>& sym)
{
	auto itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackAnimation* node = new PackAnimation;
	Load(sym, node);
	m_map_data.insert(std::make_pair(sym, node));

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

void Terrain2DBuilder::Load(const std::shared_ptr<const eterrain2d::Symbol>& sym, PackAnimation* anim)
{
// 	const std::vector<eterrain2d::OceanMesh*> oceans = sym->GetOceans();
// 	// todo
// 	if (oceans.size() != 1) {
// 		throw ee::Exception("Terrain2DBuilder::Load oceans.size() != 1, filepath: %s", sym->GetFilepath().c_str());
// 	}
// 
// 	eterrain2d::OceanMesh* ocean = oceans[0];
// 	
// 	// todo
// 	ocean->OpenWave(false);
// 	ocean->OpenBlend(false);
// 
// 	static const float FPS = 30;
// 	if (ocean->GetUVMoveSpeed().y == 0) {
// 		throw ee::Exception("Terrain2DBuilder::Load ocean speed 0, filepath: %s", sym->GetFilepath().c_str());
// 	}
// 	int frame = (int)(fabs(FPS / ocean->GetUVMoveSpeed().y));
// 
// 	ocean->Build();
// 
// 	for (int i = 0; i < frame; ++i)
// 	{
// 		PackAnimation::Frame frame;
// 		PackPicture* pic = new PackPicture;
// 
// 		const std::vector<eterrain2d::MeshShape*>& meshes = ocean->GetMeshes();
// 		for (int j = 0, m = meshes.size(); j < m; ++j)
// 		{
// 			const eterrain2d::MeshShape* mesh = meshes[j];
// 			const std::vector<s2::MeshTriangle*>& tris = mesh->GetTriangles();
// 			for (int k = 0, l = tris.size(); k < l; ++k)
// 			{
// 				s2::MeshTriangle* tri = tris[k];
// 
// 				PackPicture::Quad quad;
// 
// 				quad.img = ocean->GetImage0()->GetImage();
// 
// 				for (int i = 0; i < 3; ++i) {
// 					quad.texture_coord[i] = tri->nodes[i]->uv;
// 				}
// 				quad.texture_coord[3] = quad.texture_coord[2];
// 
// 				for (int i = 0; i < 3; ++i) {
// 					quad.screen_coord[i] = tri->nodes[i]->xy;
// 				}
// 				quad.screen_coord[3] = quad.screen_coord[2];
// 
// 				pic->quads.push_back(quad);
// 			}
// 		}
// 		m_pics.push_back(pic);
// 
// 		int comp_idx = anim->components.size();
// 		PackAnimation::Component comp;
// 		comp.node = pic;
// 		anim->components.push_back(comp);
// 
// 		PackAnimation::Part part;
// 		part.comp_idx = comp_idx;
// 		frame.parts.push_back(part);
// 
// 		anim->frames.push_back(frame);
// 
// 		ocean->Update(1 / FPS);
// 	}
// 
// 	PackAnimation::Action action;
// 	action.size = anim->frames.size();
// 	anim->actions.push_back(action);
}

}