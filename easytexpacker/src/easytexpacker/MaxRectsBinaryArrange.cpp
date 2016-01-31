#include "MaxRectsBinaryArrange.h"

#include <easytexpacker.h>

namespace etexpacker
{

void MaxRectsBinaryArrange::arrange(const std::vector<ee::ImageSprite*>& sprites)
{
	int sz = sprites.size();
	std::vector<libtexpacker::RectSize> rects;
	rects.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		ee::Rect r = sprites[i]->GetSymbol().GetSize();
		rects.push_back(libtexpacker::RectSize(r.Width(), r.Height()));
	}
	std::vector<libtexpacker::Rect> output;

	libtexpacker::MaxRectsBinaryPack2 pack;
	pack.Pack(libtexpacker::PACK_AUTO, 2048, 2048, 2048, rects, output);

	assert(sprites.size() == output.size());
	for (int i = 0; i < sz; ++i) {
		ee::Sprite* sprite = sprites[i];
		const libtexpacker::Rect& r = output[i];
		ee::Vector pos;
		pos.x = r.x + r.width * 0.5f;
		pos.y = r.y + r.height * 0.5f;
		sprite->SetTransform(pos, sprite->GetAngle());
	}
}

}