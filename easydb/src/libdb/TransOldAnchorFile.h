#ifndef _EASYDB_TRANS_OLD_ANCHOR_FILE_H_
#define _EASYDB_TRANS_OLD_ANCHOR_FILE_H_

#include <ee/Sprite.h>

#include "ICommand.h"

namespace edb
{

class TransOldAnchorFile : public ICommand
{
public:
	TransOldAnchorFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransOldAnchorFile(); }

private:
	void Run(const std::string& folder);

private:
	void TransComplex(const std::string& filepath) const;
	void TransAnimation(const std::string& filepath) const;

	bool IsAnchor(const ee::SprConstPtr& spr) const;

}; // TransOldAnchorFile


}

#endif // _EASYDB_TRANS_OLD_ANCHOR_FILE_H_