#ifndef _EASYDB_GEN_PKG_REF_INFO_H_
#define _EASYDB_GEN_PKG_REF_INFO_H_

#include "ICommand.h"

namespace edb
{

class GenPkgRefInfo : public ICommand
{
public:
	GenPkgRefInfo();

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new GenPkgRefInfo(); }

	void Trigger(const std::string& src_dir, const std::string& pkg_cfg,
		const std::string& dst_filepath);

}; // GenPkgRefInfo

}

#endif // _EASYDB_GEN_PKG_REF_INFO_H_