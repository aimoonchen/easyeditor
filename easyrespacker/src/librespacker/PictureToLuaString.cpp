#include "PictureToLuaString.h"
#include "typedef.h"

#include <ee/TexturePacker.h>
#include <ee/Image.h>
#include <ee/ImageData.h>

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace erespacker
{

void PictureToLuaString::Pack(const PackPicture* pic, 
							  ebuilder::CodeGenerator& gen, 
							  const ee::TexturePacker& tp,
							  float scale)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + pic->GetFilepath());

	lua::assign_with_end(gen, "type", "\"picture\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(pic->GetSprID()));

	for (int i = 0, n = pic->quads.size(); i < n; ++i) {
		PackQuad(pic->quads[i], gen, tp, scale);
	}

	gen.detab();
	gen.line("},");
}

void PictureToLuaString::PackQuad(const PackPicture::Quad& quad, 
								  ebuilder::CodeGenerator& gen, 
								  const ee::TexturePacker& tp,
								  float scale)
{
	int idx = tp.QueryIdx(quad.img->GetFilepath());
	if (idx == -1) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	std::string tex_str = lua::assign("tex", ee::StringHelper::ToString(idx));

	char buff[256];

	int src[8];
	PackPicture::GetImgSrcPos(tp, quad.img.get(), quad.texture_coord, src);
	for (int i = 0; i < 8; ++i) {
		src[i] *= scale;
	}
	sprintf(buff, "src = { %d, %d, %d, %d, %d, %d, %d, %d }", 
		src[0], src[1], src[2], src[3], src[4], src[5], src[6], src[7]);
	std::string src_str = buff;

	int screen[8];
	for (int i = 0; i < 4; ++i) {
		screen[i*2] = floor(quad.screen_coord[i].x * SCALE + 0.5f);
		screen[i*2+1] = -floor(quad.screen_coord[i].y * SCALE + 0.5f);
	}
	sprintf(buff, "screen = { %d, %d, %d, %d, %d, %d, %d, %d }", 
		screen[0], screen[1], screen[2], screen[3], screen[4], screen[5], screen[6], screen[7]);
	std::string screen_str = buff;

	lua::tableassign(gen, "", 3, tex_str, src_str, screen_str);
}

}