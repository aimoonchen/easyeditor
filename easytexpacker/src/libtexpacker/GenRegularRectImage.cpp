#include "GenRegularRectImage.h"

#include <ee/StringHelper.h>
#include <ee/FileHelper.h>
#include <ee/ImagePack.h>
#include <ee/ImageData.h>

#include <easyimage.h>

#include <json/json.h>

#include <fstream>

namespace etexpacker
{

void GenRegularRectImage::CreateMulti(const std::string& filepath)
{
	int i = 1;
	while (true) {
		std::string path = filepath + ee::StringHelper::ToString(i) + ".json";
		if (ee::FileHelper::IsFileExist(path)) {
			CreateSingle(path);
		} else {
			break;
		}
		++i;
	}
}

void GenRegularRectImage::CreateSingle(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int width = value["width"].asInt(),
		height = value["height"].asInt();
	ee::ImagePack pack(width, height);

	int i = 0;
	Json::Value spr_val = value["parts"][i++];
	while (!spr_val.isNull()) {
		int x = spr_val["dst"]["x"].asInt(),
			y = spr_val["dst"]["y"].asInt();
		int w = spr_val["dst"]["w"].asInt(),
			h = spr_val["dst"]["h"].asInt();
		std::string filepath = spr_val["filepath"].asString();

		auto img_data = ee::ImageDataMgr::Instance()->GetItem(filepath);
		assert(img_data->GetWidth() == w && img_data->GetHeight() == h 
			|| img_data->GetWidth() == h && img_data->GetHeight() == w);
		bool rot = img_data->GetWidth() != w && img_data->GetHeight() != h;
		pack.AddImage(*img_data, x ,y, w, h, rot, true);

		spr_val = value["parts"][i++];
	}

	std::string out_file = filepath;
	ee::StringHelper::Replace(out_file, ".json", ".png");
	pack.OutputToFile(out_file);
}

}