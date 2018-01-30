#include "FileIO.h"
#include "ToolbarPanel.h"
#include "MTConfigMgr.h"
#include "MotionTrail.h"
#include "mt_config.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/Symbol.h>

#include <mt_2d.h>
#include <s2loader/TrailSymLoader.h>
#include <sprite2/TrailEmitterCfg.h>

#include <json/json.h>

#include <fstream>

namespace etrail
{

void FileIO::Store(const std::string& filepath, MotionTrail* mt, ToolbarPanel* toolbar)
{
	Json::Value value;
	std::string dir = ee::FileHelper::GetFileDir(filepath);
	toolbar->Store(value, dir);

	value["name"] = mt->name;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const std::string& filepath, MotionTrail* mt, ToolbarPanel* toolbar, ee::LibraryPanel* library)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	toolbar->Load(value, dir);

	mt->name = value["name"].asString();
}

MotionTrail* FileIO::LoadMT(const std::string& filepath)
{
	auto cfg = MTConfigMgr::Instance()->GetConfig(filepath);
	return new MotionTrail(cfg);
}

std::shared_ptr<s2::TrailEmitterCfg> FileIO::LoadMTConfig(const std::string& filepath)
{
	class Loader : public s2loader::TrailSymLoader
	{
	protected:
		virtual s2::SymPtr LoadSymbol(const CU_STR& filepath) const override {
			return ee::SymbolMgr::Instance()->FetchSymbol(filepath.c_str());
		}
	}; // Loader

	Loader adapter;
	adapter.LoadJson(filepath.c_str());

	int sz = SIZEOF_T2D_EMITTER_CFG + SIZEOF_T2D_SYMBOL * MAX_COMPONENTS;
	t2d_emitter_cfg* cfg = (t2d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);
	adapter.Store(cfg);

	return std::make_shared<s2::TrailEmitterCfg>(cfg);
}

}