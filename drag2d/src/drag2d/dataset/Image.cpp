#include "Image.h"

#include "dataset/TPNode.h"
#include "render/GL10.h"
#include "render/Shader.h"
#include "render/ShaderMgr.h"
//#include "render/DynamicTexture.h"
#include "render/DynamicTexAndFont.h"
#include "render/RenderList.h"
#include "render/BlendShader.h"
#include "render/ScreenFBO.h"
#include "common/tools.h"
#include "common/Exception.h"
#include "common/Math.h"
#include "common/Config.h"
#include "common/SettingData.h"
#include "common/TodoConfig.h"

#include <fstream>
#include <easyimage.h>
#include <wx/filename.h>

namespace d2d
{

Image::Image(const uint8_t* pixel, int width, int height, int channels)
{
	m_tex.LoadFromMemory(pixel, width, height, channels, GL_RGBA);
}

Image::~Image()
{
	ImageMgr::Instance()->RemoveItem(m_tex.GetFilepath());
}

bool Image::LoadFromFile(const std::string& filepath)
{
	if (!wxFileName::FileExists(filepath)) {
		throw Exception("Image File: %s don't exist!", filepath.c_str());
	}

#ifdef NOT_LOAD_IMAGE
	return true;
#endif

	//////////////////////////////////////////////////////////////////////////
	if (Config::Instance()->GetSettings().open_image_edge_clip) {
		LoadWithClip(filepath);
	} else {
		m_tex.LoadFromFile(filepath);
		m_ori_w = m_tex.GetWidth();
		m_ori_h = m_tex.GetHeight();
	}

	if (m_tex.GetWidth() == 0 || m_tex.GetHeight() == 0) {
		return true;
	}

	// 		// todo
	// 		DynamicTexture::Instance()->Insert(this);
	if (Config::Instance()->IsUseDTex()) {
		DynamicTexAndFont::Instance()->AddImage(this);
	}

	return true;
}

void Image::Reload()
{
	m_tex.Reload();
}

void Image::Draw(const Matrix& mt, const ISprite* spr) const
{
	////////////////////////////////////////////////////////////////////////////
	//// 原始 直接画
 //   	ShaderMgr* shader = ShaderMgr::Instance();
 //   	shader->sprite();
 //   
 //   	float tot_hw = m_width * 0.5f,
 //   		  tot_hh = m_height * 0.5f;
 //   	float txmin = (r.xMin + tot_hw) / m_width,
 //   		txmax = (r.xMax + tot_hw) / m_width,
 //   		tymin = (r.yMin + tot_hh) / m_height,
 //   		tymax = (r.yMax + tot_hh) / m_height;
 //   
 // 	d2d::Vector vertices[4];
 // 	vertices[0] = Math::transVector(Vector(r.xMin, r.yMin), mt);
 // 	vertices[1] = Math::transVector(Vector(r.xMax, r.yMin), mt);
 // 	vertices[2] = Math::transVector(Vector(r.xMax, r.yMax), mt);
 // 	vertices[3] = Math::transVector(Vector(r.xMin, r.yMax), mt);
 // 
 // 	for (int i = 0; i < 4; ++i) {
 // 		scr.TransPosForRender(vertices[i]);
 // 	}
 // 
 // 	float vb[16];
 // 	vb[0] = vertices[0].x;
 // 	vb[1] = vertices[0].y;
 // 	vb[2] = txmin;
 // 	vb[3] = tymin;
 // 	vb[4] = vertices[1].x;
 // 	vb[5] = vertices[1].y;
 // 	vb[6] = txmax;
 // 	vb[7] = tymin;
 // 	vb[8] = vertices[2].x;
 // 	vb[9] = vertices[2].y;
 // 	vb[10] = txmax;
 // 	vb[11] = tymax;
 // 	vb[12] = vertices[3].x;
 // 	vb[13] = vertices[3].y;
 // 	vb[14] = txmin;
 // 	vb[15] = tymax;
 // 
 // 	shader->Draw(vb, m_textureID);

	//////////////////////////////////////////////////////////////////////////
//	用renderlist
// 	d2d::Vector vertices[4];
// 	vertices[0] = Math::transVector(Vector(r.xMin, r.yMin), mt);
// 	vertices[1] = Math::transVector(Vector(r.xMax, r.yMin), mt);
// 	vertices[2] = Math::transVector(Vector(r.xMax, r.yMax), mt);
// 	vertices[3] = Math::transVector(Vector(r.xMin, r.yMax), mt);
// 	for (int i = 0; i < 4; ++i) {
// 		scr.TransPosForRender(vertices[i]);
// 	}
// 
// 	d2d::Vector texcoords[4];
// 	float tot_hw = m_width * 0.5f,
// 		tot_hh = m_height * 0.5f;
// 	float txmin = (r.xMin + tot_hw) / m_width,
// 		txmax = (r.xMax + tot_hw) / m_width,
// 		tymin = (r.yMin + tot_hh) / m_height,
// 		tymax = (r.yMax + tot_hh) / m_height;
// 	texcoords[0].set(txmin, tymin);
// 	texcoords[1].set(txmax, tymin);
// 	texcoords[2].set(txmax, tymax);
// 	texcoords[3].set(txmin, tymax);
// 
// 	RenderList::Instance()->Insert(m_textureID, vertices, texcoords);

	//////////////////////////////////////////////////////////////////////////
	// 用dtex

	float hw = m_tex.GetWidth() * 0.5f,
		hh = m_tex.GetHeight() * 0.5f;

	float px = 0, py = 0;
	if (spr) {
		px = spr->GetPerspective().x;
		py = spr->GetPerspective().y;
	}

 	d2d::Vector vertices[4];
	vertices[0] = Vector(-hw - px, -hh - py);
	vertices[1] = Vector(hw + px, -hh + py);
	vertices[2] = Vector(hw - px, hh - py);
	vertices[3] = Vector(-hw + px, hh + py);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math::transVector(vertices[i] + m_offset, mt);
	}

	int texid;
	d2d::Vector texcoords[4];
	float txmin, txmax, tymin, tymax;
	DynamicTexAndFont* dt = NULL;
	const TPNode* n = NULL;
	if (Config::Instance()->IsUseDTex()) {
		dt = DynamicTexAndFont::Instance();
		n = dt->Query(m_tex.GetFilepath());
	}
 	if (n)
 	{
 		float extend = dt->GetExtend();
 		int width = dt->GetWidth();
 		int height = dt->GetHeight();
 		texid = dt->GetTextureID();
		txmin = (n->GetMinX()+extend) / width;
		txmax = (n->GetMaxX()-extend) / width;
		tymin = (n->GetMinY()+extend) / height;
		tymax = (n->GetMaxY()-extend) / height;

		if (texid != 1) {
			wxLogDebug(_T("img dt's tex = %d"), texid);
		}

 		if (n->IsRotated())
 		{
 			d2d::Vector tmp = vertices[3];
 			vertices[3] = vertices[2];
 			vertices[2] = vertices[1];
 			vertices[1] = vertices[0];
 			vertices[0] = tmp;
 		}
 	}
 	else
	{
		//wxLogDebug("Fail to insert dtex: %s", m_filepath.c_str());

		texid = m_tex.GetTexID();
	 	txmin = tymin = 0;
	 	txmax = tymax = 1;
	}
 	texcoords[0].set(txmin, tymin);
 	texcoords[1].set(txmax, tymin);
 	texcoords[2].set(txmax, tymax);
 	texcoords[3].set(txmin, tymax);

//	RenderList::Instance()->Insert(texid, vertices, texcoords);

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->sprite();

	if (BlendShader* blend_shader = dynamic_cast<BlendShader*>(mgr->GetSpriteShader())) {
		SpriteRenderer* rd = SpriteRenderer::Instance();
		const Camera* cam = rd->GetCamera();
		assert(cam);

		int w = ScreenFBO::Instance()->GetFBO().GetWidth(),
			h = ScreenFBO::Instance()->GetFBO().GetHeight();

		//////////////////////////////////////////////////////////////////////////

		assert(spr);
		d2d::Vector vertices_scr[4];
		d2d::Rect r = spr->GetRect();
		vertices_scr[0] = Math::transVector(Vector(r.xMin, r.yMin), mt);
		vertices_scr[1] = Math::transVector(Vector(r.xMax, r.yMin), mt);
		vertices_scr[2] = Math::transVector(Vector(r.xMax, r.yMax), mt);
		vertices_scr[3] = Math::transVector(Vector(r.xMin, r.yMax), mt);

		d2d::Vector tex_coolds_base[4];
		for (int i = 0; i < 4; ++i) {
			tex_coolds_base[i] = cam->transPosProjectToScreen(vertices_scr[i], w, h);
			tex_coolds_base[i].y = h - 1 - tex_coolds_base[i].y;
			tex_coolds_base[i].x /= w;
			tex_coolds_base[i].y /= h;
			tex_coolds_base[i].x = std::min(std::max(0.0f, tex_coolds_base[i].x), 1.0f);
			tex_coolds_base[i].y = std::min(std::max(0.0f, tex_coolds_base[i].y), 1.0f);
		}

		//////////////////////////////////////////////////////////////////////////

//  		assert(spr);
//  		d2d::Vector vertices_scr[4];
//  		d2d::Rect r = spr->GetRect();
//  		vertices_scr[0] = Math::transVector(Vector(0.0f, 0.0f), mt);
//  		vertices_scr[1] = Math::transVector(Vector(r.xLength(), 0.0f), mt);
//  		vertices_scr[2] = Math::transVector(Vector(r.xLength(), r.yLength()), mt);
//  		vertices_scr[3] = Math::transVector(Vector(0.0f, r.yLength()), mt);
//  
//  		d2d::Vector tex_coolds_base[4];
//  		for (int i = 0; i < 4; ++i) {
//  			tex_coolds_base[i] = cam->transPosProjectToScreen(vertices_scr[i], w, h);
//  			tex_coolds_base[i].x /= w;
//  			tex_coolds_base[i].y /= h;
//  			tex_coolds_base[i].x = std::min(std::max(0.0f, tex_coolds_base[i].x), 1.0f);
//  			tex_coolds_base[i].y = std::min(std::max(0.0f, tex_coolds_base[i].y), 1.0f);
//  		}

		//////////////////////////////////////////////////////////////////////////

		blend_shader->DrawBlend(vertices, texcoords, tex_coolds_base, texid);
	} else {
		mgr->Draw(vertices, texcoords, texid);
	}
}

void Image::LoadWithClip(const std::string& filepath)
{
	m_tex.SetFilepath(filepath);

	int w, h, c, f;
	const uint8_t* pixels = ImageLoader::loadData(filepath, w, h, c, f);

	m_ori_w = w;
	m_ori_h = h;

	if (c != 4) 
	{
		m_tex.LoadFromMemory(pixels, w, h, c, f);
	} 
	else 
	{
		eimage::ImageTrimRaw trim(pixels, w, h);
		Rect r = trim.Trim();

		eimage::ImageClipRaw clip(pixels, w, h, c);
		const uint8_t* c_pixels = clip.Clip(r);

		eimage::ImageVeritalFlip yflip(c_pixels, r.xLength(), r.yLength());
		const uint8_t* flip_pixels = yflip.Revert();

		m_tex.LoadFromMemory(flip_pixels, r.xLength(), r.yLength(), c, f);

		delete[] c_pixels;
		delete[] pixels;

		m_offset.x = r.xCenter() - w * 0.5f;
		m_offset.y = r.yCenter() - h * 0.5f;
	}
}

} // d2d