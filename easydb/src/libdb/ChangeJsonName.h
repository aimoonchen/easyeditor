#ifndef _EASYDB_CHANGE_JSON_NAME_H_
#define _EASYDB_CHANGE_JSON_NAME_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class ChangeJsonName : public ICommand
{
public:
	ChangeJsonName() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new ChangeJsonName(); }

private:
	void Trigger(const std::string& dir, const std::string& postfix) const;

}; // ChangeJsonName 

}

#endif // _EASYDB_CHANGE_JSON_NAME_H_
