#include "DynamicTexture.h"

#include "dataset/TPNode.h"
#include "render/ShaderMgr.h"

#include <algorithm>
//#include <gl/GL.h>
#include <gl/glew.h>

namespace d2d
{

const int DynamicTexture::WIDTH = 4096;
const int DynamicTexture::HEIGHT = 4096;
const int DynamicTexture::PADDING = 1;
const int DynamicTexture::EXTRUDE = 1;

DynamicTexture* DynamicTexture::m_instance = NULL;
DynamicTexture* DynamicTexture::Instance()
{
	if (!m_instance) {
		m_instance = new DynamicTexture();
	}
	return m_instance;
}

DynamicTexture::DynamicTexture()
	: DynamicPacker(WIDTH, HEIGHT, PADDING, 0)
	, m_preload_idx(0)
{
	m_extrude = EXTRUDE;
}

void DynamicTexture::Begin()
{
	++m_preload_idx;
}

void DynamicTexture::Insert(Image* img)
{
	// find
	const std::string& filepath = img->GetFilepath();
	if (m_map_images.find(filepath) != m_map_images.end()) {
		return;
	}

	// todo need insert directly
	if (m_preload_idx != 0) {
		img->Retain();
		m_preload_list.push_back(img);
	} else {
		ShaderMgr* shader = ShaderMgr::Instance();
		shader->SetFBO(m_fbo);
 		shader->sprite();
 		glViewport(0, 0, m_width, m_height);
		InsertImage(img);
		// set fbo to force flush
		shader->SetFBO(0);
	}
}

void DynamicTexture::End()
{
	--m_preload_idx;

	if (m_preload_idx > 0 || m_preload_list.empty()) {
		return;
	}

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->SetFBO(m_fbo);
	shader->sprite();

	glViewport(0, 0, m_width, m_height);
	std::sort(m_preload_list.begin(), m_preload_list.end(), ImageSizeCmp());
	std::vector<Image*>::iterator itr = m_preload_list.begin();
	for ( ; itr != m_preload_list.end(); ++itr) {
		Image* img = *itr;
		InsertImage(img);
		img->Release();
	}
	m_preload_list.clear();

	// set fbo to force flush
	// todo dtex连续insert会慢
	shader->SetFBO(0);
}

void DynamicTexture::Remove(Image* img)
{
	const std::string& filepath = img->GetFilepath();
	std::map<std::string, TPNode*>::const_iterator itr 
		= m_map_images.find(filepath);
	if (itr != m_map_images.end()) {
		itr->second->Clear();
		m_map_images.erase(itr);
	}
}

const TPNode* DynamicTexture::Query(const Image& img) const
{
	const std::string& filepath = img.GetFilepath();
	std::map<std::string, TPNode*>::const_iterator itr 
		= m_map_images.find(filepath);
	if (itr != m_map_images.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

void DynamicTexture::ReloadPixels()
{
	ShaderMgr* shader = ShaderMgr::Instance();
	shader->SetFBO(m_fbo);
	shader->sprite();

	glViewport(0, 0, m_width, m_height);
	std::map<std::string, TPNode*>::iterator itr = m_map_images.begin();
	for ( ; itr != m_map_images.end(); ++itr)
	{
		Image* img = ImageMgr::Instance()->GetItem(itr->first);
		assert(img);
		TPNode* n = itr->second;
		assert(n);
		DrawNode(n, img);
	}

	// set fbo to force flush
	// todo dtex之后insert时，不能连续
	shader->SetFBO(0);	
}

void DynamicTexture::InsertImage(const Image* img)
{
	const std::string& filepath = img->GetFilepath();
	if (m_map_images.find(filepath) != m_map_images.end()) {
		return;
	}

	int w = img->GetClippedWidth();
	int h = img->GetClippedHeight();
	d2d::TPNode* n = m_root->Insert(w+m_padding*2, h+m_padding*2);
	if (!n) {
		return;
	}

	DrawNode(n, img);

	m_map_images.insert(std::make_pair(img->GetFilepath(), n));
}

void DynamicTexture::DrawNode(const TPNode* n, const Image* img) const
{
	Rect r_vertex, r_texcoords;
	r_vertex.xMin = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
	r_vertex.xMax = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
	r_vertex.yMin = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;
	r_vertex.yMax = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;

	int ori_width = img->GetOriginWidth(),
		ori_height = img->GetOriginHeight();
	r_texcoords.xMin = r_texcoords.yMin = 0;
	r_texcoords.xMax = r_texcoords.yMax = 1;
	DrawRegion(r_vertex, r_texcoords, img->GetTexID(), n->IsRotated());

	DrawExtrude(img, n);
}

void DynamicTexture::DrawRegion(const Rect& vertex, const Rect& texcoords, int texid, bool is_rotate) const
{
	float vb[16];
	vb[0] = vertex.xMin;
	vb[1] = vertex.yMin;
	vb[4] = vertex.xMax;
	vb[5] = vertex.yMin;
	vb[8] = vertex.xMax;
	vb[9] = vertex.yMax;
	vb[12] = vertex.xMin;
	vb[13] = vertex.yMax;

 	if (is_rotate)
 	{
 		vb[2] = texcoords.xMin;
 		vb[3] = texcoords.yMax;
 		vb[6] = texcoords.xMin;
 		vb[7] = texcoords.yMin;
 		vb[10] = texcoords.xMax;
 		vb[11] = texcoords.yMin;
 		vb[14] = texcoords.xMax;
 		vb[15] = texcoords.yMax;
 	}
 	else
	{
		vb[2] = texcoords.xMin;
		vb[3] = texcoords.yMin;
		vb[6] = texcoords.xMax;
		vb[7] = texcoords.yMin;
		vb[10] = texcoords.xMax;
		vb[11] = texcoords.yMax;
		vb[14] = texcoords.xMin;
		vb[15] = texcoords.yMax;
	}
	ShaderMgr::Instance()->Draw(vb, texid);
}

void DynamicTexture::DrawExtrude(const Image* img, const TPNode* n) const
{
	int ori_width = img->GetOriginWidth(),
		ori_height = img->GetOriginHeight();
	Rect r_vertex, r_texcoords;

	float hw = img->GetClippedWidth() * 0.5f,
		hh = img->GetClippedHeight() * 0.5f;
	if (n->IsRotated())
	{
		// up
		r_texcoords.xMin = (hw - 1 + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (hw + ori_width * 0.5f) / ori_width;
		r_texcoords.yMin = (-hh + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (hh + ori_height * 0.5f) / ori_height;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.yMin = ((float)(n->GetMaxY()+i-m_padding) / m_height) * 2 - 1;
			r_vertex.yMax = ((float)(n->GetMaxY()+1+i-m_padding) / m_height) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->GetTexID(), true);
		}

		// down
		r_texcoords.xMin = (-hw + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (-hw + 1 + ori_width * 0.5f) / ori_width;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.yMin = ((float)(n->GetMinY()-1-i+m_padding) / m_height) * 2 - 1;
			r_vertex.yMax = ((float)(n->GetMinY()-i+m_padding) / m_height) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->GetTexID(), true);
		}

		// left
		r_texcoords.xMin = (-hw + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (hw + ori_width * 0.5f) / ori_width;
		r_texcoords.yMin = (hh - 1 + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (hh + ori_height * 0.5f) / ori_height;
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.xMin = ((float)(n->GetMinX()-1-i+m_padding) / m_width) * 2 - 1;
			r_vertex.xMax = ((float)(n->GetMinX()-i+m_padding) / m_width) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->GetTexID(), true);
		}

		// right
		r_texcoords.yMin = (-hh + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (-hh + 1 + ori_height * 0.5f) / ori_height;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.xMin = ((float)(n->GetMaxX()+i-m_padding) / m_width) * 2 - 1;
			r_vertex.xMax = ((float)(n->GetMaxX()+1+i-m_padding) / m_width) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->GetTexID(), true);
		}		

		// up-left
		r_texcoords.xMin = (-hw + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (-hw + ori_width * 0.5f + 1) / ori_width;
		r_texcoords.yMin = (hh + ori_height * 0.5f - 1) / ori_height;
		r_texcoords.yMax = (hh + ori_height * 0.5f) / ori_height;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding-1) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;		
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding-1) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;		
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// up-right
		r_texcoords.xMin = (hw + ori_width * 0.5f - 1) / ori_width;
		r_texcoords.xMax = (hw + ori_width * 0.5f) / ori_width;
		r_vertex.yMin = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMaxY()-m_padding+1) / m_height) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// down-right
		r_texcoords.yMin = (-hh + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (-hh + ori_height * 0.5f + 1) / ori_height;
		r_vertex.xMin = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMaxX()-m_padding+1) / m_width) * 2 - 1;		
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// down-left
		r_texcoords.xMin = (-hw + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (-hw + ori_width * 0.5f + 1) / ori_width;
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding-1) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;		
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
	}
	else
	{
		// up
		r_texcoords.xMin = (-hw + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (hw + ori_width * 0.5f) / ori_width;
		r_texcoords.yMin = (hh - 1 + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (hh + ori_height * 0.5f) / ori_height;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.yMin = ((float)(n->GetMaxY()+i-m_padding) / m_height) * 2 - 1;
			r_vertex.yMax = ((float)(n->GetMaxY()+1+i-m_padding) / m_height) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
		}

		// down
		r_texcoords.yMin = (-hh + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (-hh + 1 + ori_height * 0.5f) / ori_height;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.yMin = ((float)(n->GetMinY()-1-i+m_padding) / m_height) * 2 - 1;
			r_vertex.yMax = ((float)(n->GetMinY()-i+m_padding) / m_height) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
		}

		// left
		r_texcoords.xMin = (-hw + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (-hw + 1 + ori_width * 0.5f) / ori_width;
		r_texcoords.yMin = (-hh + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (hh + ori_height * 0.5f) / ori_height;
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.xMin = ((float)(n->GetMinX()-1-i+m_padding) / m_width) * 2 - 1;
			r_vertex.xMax = ((float)(n->GetMinX()-i+m_padding) / m_width) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
		}

		// right
		r_texcoords.xMin = (hw - 1 + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (hw + ori_width * 0.5f) / ori_width;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.xMin = ((float)(n->GetMaxX()+i-m_padding) / m_width) * 2 - 1;
			r_vertex.xMax = ((float)(n->GetMaxX()+1+i-m_padding) / m_width) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
		}

		// up-left
		r_texcoords.xMin = (-hw + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (-hw + ori_width * 0.5f + 1) / ori_width;
		r_texcoords.yMin = (hh + ori_height * 0.5f - 1) / ori_height;
		r_texcoords.yMax = (hh + ori_height * 0.5f) / ori_height;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding-1) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
		r_vertex.yMin = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMaxY()-m_padding+1) / m_height) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// up-right
		r_texcoords.xMin = (hw + ori_width * 0.5f - 1) / ori_width;
		r_texcoords.xMax = (hw + ori_width * 0.5f) / ori_width;
		r_vertex.xMin = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMaxX()-m_padding+1) / m_width) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// down-right
		r_texcoords.yMin = (-hh + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (-hh + ori_height * 0.5f + 1) / ori_height;
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding-1) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// down-left
		r_texcoords.xMin = (-hw + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (-hw + ori_width * 0.5f + 1) / ori_width;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding-1) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
	}
}

}