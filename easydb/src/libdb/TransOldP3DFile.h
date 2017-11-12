#ifndef _EASYDB_TRANS_OLD_P3D_FILE_H_
#define _EASYDB_TRANS_OLD_P3D_FILE_H_

#include "ICommand.h"



namespace edb
{

class TransOldP3DFile : public ICommand
{
public:
	TransOldP3DFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransOldP3DFile(); }

private:
	void Run(const std::string& folder);

private:
	void Trans(const std::string& filepath) const;

}; // TransOldP3DFile


}

#endif // _EASYDB_TRANS_OLD_P3D_FILE_H_