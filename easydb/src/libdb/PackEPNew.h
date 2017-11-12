#ifndef _EASYDB_PACK_EP_NEW_H_
#define _EASYDB_PACK_EP_NEW_H_

#include "ICommand.h"

#include <easycoco.h>

namespace edb
{

class PackEPNew : public ICommand
{
public:
	PackEPNew() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackEPNew(); }

	void Trigger(const std::string& json_dir, const std::string& tp_json, 
		const std::string& tp_dir, const std::string& out_file, 
		const std::string& type, int LOD, float scale);

}; // PackEPNew

}

#endif // _EASYDB_PACK_EP_NEW_H_