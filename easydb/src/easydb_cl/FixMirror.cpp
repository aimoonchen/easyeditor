#include "FixMirror.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <SM_Vector.h>
#include <sprite2/Color.h>
#include <glue/trans_color.h>

#include <fstream>

namespace edb
{

std::string FixMirror::Command() const
{
	return "fix-mirror";
}

std::string FixMirror::Description() const
{
	return "fix err mirror";
}

std::string FixMirror::Usage() const
{
	return Command() + " [dir]";
}

int FixMirror::Run(int argc, char *argv[])
{
	// fix-mirror D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

bool FixMirror::FixSprite(const std::string& filepath, Json::Value& val) const
{
	bool dirty = false;

	sm::vec2 fix_s;
	if (val.isMember("scale")) {
		fix_s.x = val["scale"].asDouble();
	} else if (val.isMember("x scale")) {
		fix_s.x = val["x scale"].asDouble();
	} else {
		fix_s.x = 1;
	}
	if (val.isMember("scale")) {
		fix_s.y = val["scale"].asDouble();
	} else if (val.isMember("y scale")) {
		fix_s.y = val["y scale"].asDouble();
	} else {
		fix_s.y = 1;
	}
	
	if (val.isMember("x mirror")) 
	{
		bool mirror = val["x mirror"].asBool();
		if (mirror && fix_s.x >= 0 ||
			!mirror && fix_s.x < 0) {
			dirty = true;
			fix_s.x = -fix_s.x;
		}
	}

	if (val.isMember("y mirror")) 
	{
		bool mirror = val["y mirror"].asBool();
		if (mirror && fix_s.y >= 0 ||
			!mirror && fix_s.y < 0) {
			dirty = true;
			fix_s.y = -fix_s.y;
		}
	}

	if (dirty) {
		val["x scale"] = fix_s.x;
		val["y scale"] = fix_s.y;
	}

	return dirty;
}

void FixMirror::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			FixComplex(filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			FixAnim(filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_scale9)) {
			FixScale9(filepath);
		}
	}
}

}