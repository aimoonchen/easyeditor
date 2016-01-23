#ifndef _EASYDB_PACK_EPT_H_
#define _EASYDB_PACK_EPT_H_

#include "ICommand.h"

#include <drag2d.h>
#include <easyrespacker.h>

namespace edb
{

class PackEPT : public ICommand
{
public:
	PackEPT() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackEPT(); }

	void Trigger(const std::string& tp_json, const std::string& tp_dir, 
		const std::string& out_file, librespacker::TextureType type);

}; // PackEPT

}

#endif // _EASYDB_PACK_EPT_H_