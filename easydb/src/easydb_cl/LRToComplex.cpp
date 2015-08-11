#include "LRToComplex.h"
#include "check_params.h"

namespace edb
{

std::string LRToComplex::Command() const
{
	return "lr2complex";
}

std::string LRToComplex::Description() const
{
	return "create complex file from lr file";
}

std::string LRToComplex::Usage() const
{
	// lr2complex e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

void LRToComplex::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2]);
}

void LRToComplex::Run(const std::string& filepath)
{
	std::cout << "LRToComplex::Run	filepath: " << filepath << std::endl;

	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	Json::Value complex_val;
// 	for (int i = 0; i < 3; ++i) {
// 		LoadSpriteValue(lr_val["layer"][i]["sprite"], complex_val["sprite"]);
// 	}
	int layer_idx = 0;
	LoadSpriteValue(lr_val["layer"][layer_idx]["sprite"], complex_val["sprite"]);
	layer_idx = 1;
	LoadSpriteValue(lr_val["layer"][layer_idx]["sprite"], complex_val["sprite"]);
	std::string name = d2d::FilenameTools::getFilename(filepath);
	name = "scene_" + name.substr(0, name.find_last_of('_'));	
	complex_val["name"] = name;
	complex_val["use_render_cache"] = false;
	complex_val["xmax"] = 0;
	complex_val["xmin"] = 0;
	complex_val["ymax"] = 0;
	complex_val["ymin"] = 0;

	std::string outfile = filepath.substr(0, filepath.find_last_of("_") + 1);
	wxString tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_complex);
	outfile += tag + ".json";

	std::cout << "LRToComplex::Run	outfile: " << outfile << std::endl;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, complex_val);
	fout.close();
}

void LRToComplex::LoadSpriteValue(const Json::Value& src_val, Json::Value& dst_val)
{
	int idx = 0;
	Json::Value spr_val = src_val[idx++];
	while (!spr_val.isNull()) {
// 		// test character
// 		std::string tag = spr_val["tag"].asString();
// 		if (!tag.empty()) {
// 			spr_val = src_val[idx++];
// 			continue;
// 		}

		std::string filepath = spr_val["filepath"].asString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_particle3d)) {
			;
		} else {
			Json::Value spr_val_fix = spr_val;
			std::string suffix = "_shape.json";
			int pos = filepath.find(suffix);
			if (pos!= std::string::npos) {
				std::string fix_filepath = filepath.substr(0, pos) + ".png";
				if (d2d::FilenameTools::IsFileExist(fix_filepath)) {
					spr_val_fix["filepath"] = fix_filepath;
				}
			}	
			dst_val[dst_val.size()] = spr_val_fix;
		}

		spr_val = src_val[idx++];
	}
}

}