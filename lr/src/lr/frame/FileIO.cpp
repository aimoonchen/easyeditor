#include "FileIO.h"

#include "frame/SettingCfg.h"
#include "view/LibraryPanel.h"
#include "view/StagePanel.h"

namespace lr
{

void FileIO::load(const char* filename, StagePanel* stage,
				  LibraryPanel* library)
{
// 	d2d::SymbolMgr::Instance()->clear();
// 	d2d::BitmapMgr::Instance()->clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filename);

	// sprites
	LoadSprites(value["sprite"], stage, dir);

	// layers
	library->LoadFromFile(value["layer"], dir);

	// size
	if (!value["size"].isNull()) {
		SettingCfg* cfg = SettingCfg::Instance();
		cfg->m_view_width = value["size"]["width"].asInt();
		cfg->m_view_height = value["size"]["height"].asInt();
	}
}

void FileIO::store(const char* filename, StagePanel* stage,
				   LibraryPanel* library)
{
	Json::Value value;

	std::string dir = d2d::FilenameTools::getFileDir(filename) + "\\";

	// sprites
	StoreSprites(value["sprite"], stage, dir);

	// layers
	library->StoreToFile(value["layer"], dir);

	// size
	SettingCfg* cfg = SettingCfg::Instance();
	value["size"]["width"] = cfg->m_view_width;
	value["size"]["height"] = cfg->m_view_height;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::LoadSprites(const Json::Value& value, StagePanel* stage, 
						 const std::string& dir)
{
	int i = 0;
	Json::Value spr_val = value[i++];
	while (!spr_val.isNull()) {
		wxString filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = NULL;
		try {
			wxString shape_tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape);
			wxString shape_filepath = d2d::FilenameTools::getFilenameAddTag(filepath, shape_tag, "json");
			if (d2d::FilenameTools::isExist(shape_filepath)) {
				symbol = d2d::SymbolMgr::Instance()->fetchSymbol(shape_filepath);
			} else {
				symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			}

			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			sprite->load(spr_val);
			stage->insertSprite(sprite);

			sprite->Release();
			symbol->Release();
		} catch (d2d::Exception& e) {
			std::cout << "Symbol::loadResources error! File:" << filepath << std::endl;
			std::cout << e.what();
		}

		spr_val = value[i++];
	}
}

void FileIO::StoreSprites(Json::Value& value, StagePanel* stage,
						  const std::string& dir)
{
	std::vector<d2d::ISprite*> sprites;
	stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* spr = sprites[i];

		Json::Value spr_val;
		spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir,
			spr->getSymbol().getFilepath()).ToStdString();
		spr->store(spr_val);

		value[i] = spr_val;
	}
}

}