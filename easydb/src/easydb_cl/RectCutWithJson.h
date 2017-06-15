#ifndef _EASYDB_RECT_CUT_WITH_JSON_H_
#define _EASYDB_RECT_CUT_WITH_JSON_H_

#include "ICommand.h"

#include <json/json.h>

namespace etexpacker { class ImageTrimData; }

namespace edb
{

class RectCutWithJson : public ICommand
{
public:
	RectCutWithJson() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new RectCutWithJson(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_dir,
		const etexpacker::ImageTrimData* trim);

private:
	void RectCutImage(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& filepath, const etexpacker::ImageTrimData* trim) const;

	void FixComplex(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixAnim(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixScale9(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixParticle3d(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixMesh(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixMask(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;

	void FixFilepath(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& file_dir, Json::Value& val, const std::string& key) const;
	void FixImageFilepath(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& file_dir, Json::Value& val, const std::string& key) const;
	void FixImageFilepathInPkg(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& file_dir, Json::Value& val, const std::string& key) const;
	void FixFilepathOutPkg(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& file_dir, Json::Value& val, const std::string& key) const;	

	void FixJsonFilepath(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& file_dir, Json::Value& val, const std::string& key) const;

	void FixGroup(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& file_dir, Json::Value& val) const;

	bool IsIgnored(const std::string& filepath) const;

private:
	std::string m_ignore_dir;

}; // RectCutWithJson

}

#endif // _EASYDB_RECT_CUT_WITH_JSON_H_