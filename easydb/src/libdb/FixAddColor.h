#ifndef _EASYDB_FIX_ADD_COLOR_H_
#define _EASYDB_FIX_ADD_COLOR_H_

#include "ICommand.h"
#include "FixJsonBase.h"

#include <SM_Vector.h>

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixAddColor : public ICommand, private FixJsonBase
{
public:
	FixAddColor() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new FixAddColor(); }

protected:
	virtual bool FixSprite(const std::string& filepath, Json::Value& sprite_val) const;

private:
	void Trigger(const std::string& dir) const;

	void FixParticle3d(const std::string& filepath) const;

}; // FixAddColor 


}

#endif // _EASYDB_FIX_ADD_COLOR_H_