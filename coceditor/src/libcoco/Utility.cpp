#include "Utility.h"

namespace libcoco
{

void Utility::GroupSpritesFromTag(const std::vector<ee::Sprite*>& src, 
								  std::map<std::string, std::vector<ee::Sprite*> >& dst,
								  std::vector<ee::Sprite*>& others)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		ee::Sprite* sprite = src[i];
		if (sprite->tag.empty())
		{
			others.push_back(sprite);
		}
		else
		{
			std::vector<std::string> tags;
			ee::StringHelper::Split(sprite->tag, ";", tags);
			bool is_action = false;
			for (int i = 0, n = tags.size(); i < n; ++i)
			{
				if (tags[i].find("=") != std::string::npos) {
					continue;
				}

				is_action = true;

				std::map<std::string, std::vector<ee::Sprite*> >::iterator itr = 
					dst.find(tags[i]);
				if (itr == dst.end())
				{
					std::vector<ee::Sprite*> sprites;
					sprites.push_back(sprite);
					dst.insert(std::make_pair(tags[i], sprites));
				}
				else
				{
					itr->second.push_back(sprite);
				}
			}

			if (!is_action) {
				others.push_back(sprite);
			}
		}
	}
}

}