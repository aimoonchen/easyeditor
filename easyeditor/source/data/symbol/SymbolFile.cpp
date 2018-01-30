#include "SymbolFile.h"
#include "SymbolType.h"
#include "FileHelper.h"

#include <sprite2/SymType.h>
#include <s2loader/SymbolFile.h>
#include <s2loader/ExtendSymFile.h>

#include <json/json.h>

#include <fstream>

namespace ee
{

CU_SINGLETON_DEFINITION(SymbolFile);

static const char* TAG_GEN = "gen";

SymbolFile::SymbolFile()
{
	Regist(SYM_ANIS,		"anis");
	Regist(SYM_FONTBLANK,	"fontblank");
	Regist(SYM_FREETYPE,	"freetype");
	Regist(SYM_SCRIPTS,		"scripts");
	Regist(SYM_P3DINV,		"p3dinv");
	Regist(SYM_EJOY2D,		"ejoy2d");
	Regist(SYM_TERRAIN2D,	"terrain2d");
	Regist(SYM_SHADOW,		"shadow");
	Regist(SYM_UI,			"ui");
	Regist(SYM_PSD,			"psd");
	Regist(SYM_UIWND,		"uiwnd");

	Regist(s2::SYM_SCALE9,		"scale9");
	Regist(s2::SYM_ICON,		"icon");
	Regist(s2::SYM_TEXTURE,		"texture");
	Regist(s2::SYM_TEXTBOX,		"text");
	Regist(s2::SYM_COMPLEX,		"complex");
	Regist(s2::SYM_ANIMATION,	"anim");
	Regist(s2::SYM_PARTICLE3D,	"particle");
	Regist(s2::SYM_PARTICLE2D,	"particle2d");
	Regist(s2::SYM_SHAPE,		"shape");
	Regist(s2::SYM_MESH,		"mesh");
	Regist(s2::SYM_MASK,		"mask");
	Regist(s2::SYM_TRAIL,		"trail");
	Regist(s2::SYM_SKELETON,	"skeleton");
}

int SymbolFile::Type(const std::string& filepath) const
{
	int type = s2loader::SymbolFile::Instance()->Type(filepath.c_str());
	if (type != s2::SYM_UNKNOWN && type != s2::SYM_INVALID) {
		return type;
	}
	
	int pos = filepath.rfind('.');
	std::string ext = filepath.substr(pos + 1);
	if (ext == "json" || ext == "bin")
	{
		const std::string filename = filepath.substr(0, filepath.find_last_of('.'));
		int pos = filename.find_last_of('_');
		if (pos == -1) {
			return CheckTypeFromData(filepath);
		}

		std::string tag = filename.substr(pos + 1);
		pos = tag.find(TAG_GEN);
		if (pos != std::string::npos) {
			tag = tag.substr(0, pos - 1);
		}

		std::map<std::string, int>::const_iterator itr = m_tag2type.find(tag);
		if (itr != m_tag2type.end()) {
			return itr->second;
		} 

		return CheckTypeFromData(filepath);
	}
	else if (ext == "lua")
	{
		const std::string name = filepath.substr(0, filepath.find_last_of('.'));
		if (name.find('_') == -1) {
			return s2::SYM_UNKNOWN;
		}
		const std::string json_ext = name.substr(name.find_last_of('_') + 1);
		if (json_ext == "scripts") {
			return SYM_SCRIPTS;
		} else {
			return s2::SYM_UNKNOWN;
		}
	}
	else if (ext == "ttf")
	{
		return SYM_FREETYPE;
	}
	else
	{
		std::string filename = FileHelper::GetFilename(filepath);
		if (filename == SYM_GROUP_TAG) {
			return s2::SYM_COMPLEX;
		} else if (filename == SYM_SHAPE_TAG) {
			return s2::SYM_SHAPE;
		} else if (filename == SYM_TEXT_TAG) {
			return s2::SYM_TEXTBOX;
		}
	}

	return s2::SYM_UNKNOWN;
}

std::string SymbolFile::Tag(int type) const
{
	const CU_STR& tag = s2loader::SymbolFile::Instance()->Tag(type);
	if (tag != s2loader::SymbolFile::Instance()->UNKNOWN_TAG) {
		return tag.c_str();
	}

	std::map<int, std::string>::const_iterator itr = m_type2tag.find(type);
	if (itr != m_type2tag.end()) {
		return itr->second;
	} else {
		return s2loader::SymbolFile::Instance()->UNKNOWN_TAG.c_str();
	}
}

void SymbolFile::Regist(int type, const std::string& tag)
{
	m_type2tag.insert(std::make_pair(type, tag));
	m_tag2type.insert(std::make_pair(tag, type));
}

int SymbolFile::CheckTypeFromData(const std::string& filepath) const
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	int ret = s2::SYM_UNKNOWN;
	int type = s2loader::ExtendSymFile::GetType(val);
	switch (type)
	{
	case s2loader::SYM_SPINE:
		ret = val.isMember("animations") ? s2::SYM_ANIM2 : s2::SYM_SKELETON;
		break;
// 	case s2loader::SYM_BODYMOVIN:
// 		ret = s2::SYM_ANIMATION;
// 		break;
	}
	return ret;
}

}