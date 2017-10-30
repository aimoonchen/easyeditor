#include "TPParser.h"

#include <ee/Symbol.h>

#include <fstream>

namespace ecoco
{
namespace epd
{

TPParser::TPParser(const std::vector<ee::SymPtr>& syms,
				   const TextureMgr& tex_mgr)
	: m_tex_mgr(tex_mgr)
	, m_tex_size(0)
{
	m_symbol_sorter.prepare(syms);	
}

void TPParser::Parser()
{
	int idx = 0;
	while (const TextureMgr::Entry* tex = m_tex_mgr.GetTexture(idx)) {
		ParserTexture(tex, idx);
		++idx;
	}
	m_tex_size = idx;
}

TPParser::Picture* TPParser::FindPicture(const ee::SymConstPtr& sym) const
{
	auto itr = m_map_symbol2picture.find(sym);
	if (itr == m_map_symbol2picture.end()) {
		return NULL;
	} else {
		return itr->second;
	}
}

void TPParser::DebugInfo() const
{
	std::ofstream fout("d:/zz_debug.txt");

	auto itr = m_map_symbol2picture.begin();
	for ( ; itr != m_map_symbol2picture.end(); ++itr) {
		fout << itr->first->GetFilepath() << "\n";
		fout << itr->first << "\n\n";
	}

	fout.close();
}

void TPParser::ParserTexture(const TextureMgr::Entry* tex, int idx)
{
	const TPAdapter& adapter = tex->adapter;
	const std::vector<TPAdapter::Entry>& frames = adapter.GetFrames();
	bool flip_y = adapter.IsEasyDBData();
	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		const TPAdapter::Entry& entry = frames[i];

		// find symbol
		auto sym = m_symbol_sorter.GetSymbolSet().Query(entry.filename);
		if (!sym) {
			continue;
		}

		Picture* picture = new Picture;

		picture->filename = entry.filename;
		picture->entry = &entry;
		picture->invscale = adapter.GetInvScale();
		picture->tex = idx;

		picture->rotated = entry.rotated;

		float left, right, up, down;
		if (entry.rotated)
		{
			left = static_cast<float>(entry.frame.x);
			right = static_cast<float>(entry.frame.x + entry.frame.h);
			if (flip_y) {
				up = static_cast<float>(adapter.GetHeight() - entry.frame.y);
				down = static_cast<float>(adapter.GetHeight() - (entry.frame.y + entry.frame.w));
			} else {
				up = static_cast<float>(entry.frame.y);
				down = static_cast<float>(entry.frame.y + entry.frame.w);
			}

			picture->scr[0].Set(right, up);
			picture->scr[1].Set(left, up);
			picture->scr[2].Set(left, down);
			picture->scr[3].Set(right, down);
		}
		else
		{
			left = static_cast<float>(entry.frame.x);
			right = static_cast<float>(entry.frame.x + entry.frame.w);
			if (flip_y) {
				up = static_cast<float>(adapter.GetHeight() - entry.frame.y);
				down = static_cast<float>(adapter.GetHeight() - (entry.frame.y + entry.frame.h));
			} else {
				up = static_cast<float>(entry.frame.y);
				down = static_cast<float>(entry.frame.y + entry.frame.h);
			}

			picture->scr[0].Set(left, up);
			picture->scr[1].Set(left, down);
			picture->scr[2].Set(right, down);
			picture->scr[3].Set(right, up);
		}

		picture->offset.x = (entry.sprite_source_size.x + entry.sprite_source_size.w * 0.5f) - entry.src_width * 0.5f;
		picture->offset.y = entry.src_height * 0.5f - (entry.sprite_source_size.y + entry.sprite_source_size.h * 0.5f);

		m_map_symbol2picture.insert(std::make_pair(sym, picture));
	}
}

}
}
