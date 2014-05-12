// 用MD5检查重复的图片，并修复引用路径

#ifndef _EASYDB_FIX_IMAGES_H_
#define _EASYDB_FIX_IMAGES_H_

#include <string>
#include <map>
#include <vector>

namespace edb
{

class FixImages
{
public:
	FixImages(const std::string& imgdir, const std::string& jsondir);

private:
	void ProcessImageFiles(const std::string& imgdir);

	void ProcessJsonFiles(const std::string& jsondir);
	void FixImagePath(const std::string& animpath);

	void RemoveImages();

private:
	std::map<std::string, std::string> m_map_md5_2_image;
	std::map<std::string, std::string> m_map_image_2_md5;

	std::vector<std::string> m_to_remove;

}; // FixImages

}

#endif // _EASYDB_FIX_IMAGES_H_