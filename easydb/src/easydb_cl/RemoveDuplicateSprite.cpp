#include "RemoveDuplicateSprite.h"
#include "check_params.h"

namespace edb
{

std::string RemoveDuplicateSprite::Command() const
{
	return "rm-dup-spr";
}

std::string RemoveDuplicateSprite::Description() const
{
	return "remove duplicate sprites";
}

std::string RemoveDuplicateSprite::Usage() const
{
	// rm-dup-spr e:/test2/trans.txt spr-key
	std::string usage = Command() + " [filepath] [sprites' key]";
	return usage;
}

void RemoveDuplicateSprite::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2], argv[3]);
}

void RemoveDuplicateSprite::Run(const std::string& filepath, const std::string& key)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Json::Value old_spr_val = value[key];
	Json::Value new_spr_value;
	RemoveDuplicate(old_spr_val, new_spr_value);
	value[key] = new_spr_value;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RemoveDuplicateSprite::RemoveDuplicate(const Json::Value& old_val, Json::Value& new_val) const
{
	std::map<std::string, std::vector<Sprite*> > map_spr;
	int idx = 0;
	Json::Value spr_val = old_val[idx++];
	while (!spr_val.isNull()) {
		Sprite* spr = new Sprite(spr_val);
		std::map<std::string, std::vector<Sprite*> >::iterator itr
			= map_spr.find(spr->filepath);
		if (itr == map_spr.end()) {
			std::vector<Sprite*> sprites;
			sprites.push_back(spr);
			map_spr.insert(std::make_pair(spr->filepath, sprites));
		} else {
			std::vector<Sprite*>& sprites = itr->second;
			bool find = false;
			for (int i = 0, n = sprites.size(); i < n; ++i) {
				if (*spr == *sprites[i]) {
					find = true;
					break;
				}
			}
			if (find) {
				delete spr;
			} else {
				sprites.push_back(spr);
			}
		}
		
		spr_val = old_val[idx++];
	}

	std::vector<Sprite*> removed;
	std::map<std::string, std::vector<Sprite*> >::const_iterator itr
		= map_spr.begin();
	for ( ; itr != map_spr.end(); ++itr) {
		copy(itr->second.begin(), itr->second.end(), back_inserter(removed));
	}
	std::sort(removed.begin(), removed.end(), SprCmp());

	for (int i = 0, n = removed.size(); i < n; ++i) {
		new_val[i] = removed[i]->json_val;
	}

	for_each(removed.begin(), removed.end(), DeletePointerFunctor<Sprite>());
}

//////////////////////////////////////////////////////////////////////////
// class RemoveDuplicateSprite::Sprite
//////////////////////////////////////////////////////////////////////////

RemoveDuplicateSprite::Sprite::
Sprite(const Json::Value& json_val)
	: json_val(json_val)
{
	static int curr_id = 0;
	id = curr_id++;

	filepath = json_val["filepath"].asString();
	pos.x = json_val["position"]["x"].asDouble();
	pos.y = json_val["position"]["y"].asDouble();
	angle = json_val["angle"].asDouble();
	scale.x = json_val["x scale"].asDouble();
	scale.y = json_val["y scale"].asDouble();
	shear.x = json_val["x shear"].asDouble();
	shear.y = json_val["y shear"].asDouble();
}

bool RemoveDuplicateSprite::Sprite::
operator == (const Sprite& spr) const
{
	return filepath == spr.filepath
		&& pos == spr.pos
		&& angle == spr.angle
		&& scale == spr.scale
		&& shear == spr.shear;
}

}