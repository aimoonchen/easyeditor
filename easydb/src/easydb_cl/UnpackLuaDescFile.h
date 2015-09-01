#ifndef _EASYDB_UNPACK_LUA_DESC_FILE_H_
#define _EASYDB_UNPACK_LUA_DESC_FILE_H_

#include "ICommand.h"

#include <drag2d.h>
#include <easycoco.h>

namespace edb
{

class UnpackLuaDescFile : public ICommand
{
public:
	UnpackLuaDescFile() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new UnpackLuaDescFile(); }

	void Trigger(const std::string& lua_file, const std::string& img_name);

}; // UnpackLuaDescFile

}

#endif // _EASYDB_UNPACK_LUA_DESC_FILE_H_