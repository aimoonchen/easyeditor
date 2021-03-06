#ifndef _EASYDB_FIX_SPRITE_COLOR_H_
#define _EASYDB_FIX_SPRITE_COLOR_H_

#include "ICommand.h"
#include "FixJsonBase.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixSpriteColor : public ICommand, private FixJsonBase
{
public:
	FixSpriteColor() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new FixSpriteColor(); }

protected:
	virtual bool FixSprite(const std::string& filepath, Json::Value& sprite_val) const;

private:
	void Trigger(const std::string& dir) const;

}; // FixSpriteColor 

}

#endif // _EASYDB_FIX_SPRITE_COLOR_H_
