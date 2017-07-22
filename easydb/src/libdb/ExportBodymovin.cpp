#include "ExportBodymovin.h"
#include "check_params.h"
#include "utility.h"

#include <ee/SpriteLoader.h>
#include <ee/SymbolLoader.h>
#include <ee/SymbolFile.h>

#include <easyanim.h>

#include <gum/FilepathHelper.h>
#include <gum/BodymovinAnimLoader.h>
#include <gum/BodymovinParser.h>

#include <fstream>

namespace edb
{

std::string ExportBodymovin::Command() const
{
	return "export-bodymovin";
}

std::string ExportBodymovin::Description() const
{
	return "export-bodymovin";
}

std::string ExportBodymovin::Usage() const
{
	std::string usage = Command() + " [src file] [dst dir]";
	return usage;
}

int ExportBodymovin::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[3])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	Trigger(argv[2], argv[3]);

	return 0;
}

void ExportBodymovin::Trigger(const std::string& src_file, const std::string& dst_dir)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(src_file.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = gum::FilepathHelper::Dir(src_file);

	gum::BodymovinParser parser;
	parser.Parse(val, dir);

	ee::SymbolLoader sym_loader;
	ee::SpriteLoader spr_loader;

	std::map<std::string, s2::Sprite*> map_assets;
	const std::vector<gum::BodymovinParser::Asset>& assets = parser.GetAssets();
	for (int i = 0, n = assets.size(); i < n; ++i)
	{
		const gum::BodymovinParser::Asset& a = assets[i];
		if (a.layers.empty()) 
		{
			std::string filepath = gum::FilepathHelper::Absolute(".", a.filepath);
			s2::Sprite* spr = spr_loader.Create(filepath);
			map_assets.insert(std::make_pair(a.id, spr));
		}
		else
		{
			libanim::Symbol* sym = new libanim::Symbol();
			gum::BodymovinAnimLoader loader(sym, &sym_loader, &spr_loader);
			loader.LoadLayers(map_assets, a.layers, parser.GetFrameRate(), parser.GetWidth(), parser.GetHeight());
			std::string filepath = dst_dir + "\\" + a.id + "_" + ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION) + ".json";
			libanim::FileSaver::Store(filepath, *sym);
			sym->RemoveReference();
		}
	}

	std::map<std::string, s2::Sprite*>::iterator itr = map_assets.begin();
	for ( ; itr != map_assets.end(); ++itr) {
		itr->second->RemoveReference();
	}
}

}