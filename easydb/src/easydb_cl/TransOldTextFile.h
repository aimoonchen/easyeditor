#ifndef _EASYDB_TRANS_OLD_TEXT_FILE_H_
#define _EASYDB_TRANS_OLD_TEXT_FILE_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class TransOldTextFile : public ICommand
{
public:
	TransOldTextFile() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new TransOldTextFile(); }

private:
	void Run(const std::string& folder);

private:
	void TransComplex(const std::string& filepath) const;

	bool TransSprite(Json::Value& value) const;

}; // TransOldTextFile


}

#endif // _EASYDB_TRANS_OLD_TEXT_FILE_H_