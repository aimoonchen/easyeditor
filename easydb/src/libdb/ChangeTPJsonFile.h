#ifndef _EASYDB_CHANGE_TEXTURE_PACKER_JSON_FILE_H_
#define _EASYDB_CHANGE_TEXTURE_PACKER_JSON_FILE_H_

#include "ICommand.h"

#include <wx/arrstr.h>

namespace edb
{

class ChangeTPJsonFile : public ICommand
{
public:
	ChangeTPJsonFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new ChangeTPJsonFile(); }

private:
	void Run(const std::string& dir);

	void TranslateFrameXY(int dx, int dy);

private:
	wxArrayString m_files;

}; // ChangeTPJsonFile

}

#endif // _EASYDB_CHANGE_TEXTURE_PACKER_JSON_FILE_H_