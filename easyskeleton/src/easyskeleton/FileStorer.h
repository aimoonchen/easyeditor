#ifndef _EASYSKELETON_FILE_STORE_H_
#define _EASYSKELETON_FILE_STORE_H_

#include <json/json.h>

#include <string>
#include <vector>

namespace ee { class Sprite; }
namespace s2 { class Joint; }

namespace eskeleton
{

class Joint;

class FileStorer
{
public:
	static void Store(const std::string& filepath, const std::vector<ee::SprPtr>& sprs);

private:
	static Json::Value StoreSprite(const std::vector<ee::SprPtr>& sprs, const std::string& dir);

	static Json::Value StoreSkeleton(const std::vector<ee::SprPtr>& sprs);
	static Json::Value StoreJoint(const Joint* joint, const std::map<const s2::Joint*, int>& map_joint_id);
	
}; // FileStorer

}

#endif // _EASYSKELETON_FILE_STORE_H_