#include "LabelNew.h"
#include "DynamicFont.h"

#include "dataset/text_util.h"
#include "dataset/TPNode.h"
#include "view/Screen.h"
#include "render/ShaderNew.h"

namespace d2d
{

void LabelNew::Print(const Screen& screen, const char* text, const Vector& pos) const
{
	DynamicFont* dfont = DynamicFont::Instance();
	std::string utf8 = string2utf8(text);
	std::vector<int> unicodes;
	utf8_to_unicode(utf8.c_str(), unicodes);

	float x = pos.x,
		  y = pos.y;
	const float WIDTH = 20,
				HEIGHT = 20;
	Vector vertices[4], texcoords[4];
	float txmin, txmax, tymin, tymax;
	const float tex_width = dfont->GetWidth(),
		tex_height = dfont->GetHeight();
	const int tex_id = dfont->GetTextureID();
	for (int i = 0, n = unicodes.size(); i < n; ++i) 
	{
		const TPNode* r = dfont->LookUp(unicodes[i], 20, 0, 0);
		if (!r) {
			continue;
		}

		vertices[0].set(x, y);
		vertices[1].set(x+WIDTH, y);
		vertices[2].set(x+WIDTH, y+HEIGHT);
		vertices[3].set(x, y+HEIGHT);
		for (int i = 0; i < 4; ++i) {
			screen.TransPosForRender(vertices[i]);
		}

		txmin = r->GetMinX() / tex_width;
		txmax = r->GetMaxX() / tex_width;
		tymin = r->GetMinY() / tex_height;
		tymax = r->GetMaxY() / tex_height;

		if (r->IsRotated())
		{
			d2d::Vector tmp = vertices[3];
			vertices[3] = vertices[2];
			vertices[2] = vertices[1];
			vertices[1] = vertices[0];
			vertices[0] = tmp;
		}

		texcoords[0].set(txmin, tymin);
		texcoords[1].set(txmax, tymin);
		texcoords[2].set(txmax, tymax);
		texcoords[3].set(txmin, tymax);

		ShaderNew* shader = ShaderNew::Instance();
		shader->sprite();
		shader->Draw(vertices, texcoords, tex_id);

		x += WIDTH;
	}
}

}