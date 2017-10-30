#ifndef _EASYDB_UNPACK_LUA_DESC_FILE_H_
#define _EASYDB_UNPACK_LUA_DESC_FILE_H_

#include "ICommand.h"

#include <ee/Image.h>

#include <easycoco.h>

namespace edb
{

class UnpackLuaDescFile : public ICommand
{
public:
	UnpackLuaDescFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new UnpackLuaDescFile(); }

	void Trigger(const std::string& lua_file, const std::string& img_name);

private:
	static void LoadAllImages(const std::string& img_name, std::vector<ee::ImagePtr>& images);

}; // UnpackLuaDescFile

}

#endif // _EASYDB_UNPACK_LUA_DESC_FILE_H_