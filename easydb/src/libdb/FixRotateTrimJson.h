#ifndef _EASYDB_FIX_ROTATE_TRIM_JSON_H_
#define _EASYDB_FIX_ROTATE_TRIM_JSON_H_

#include "ICommand.h"
#include "FixJsonBase.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixRotateTrimJson : public ICommand, private FixJsonBase
{
public:
	FixRotateTrimJson() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new FixRotateTrimJson(); }

protected:
	virtual bool FixSprite(const std::string& filepath, Json::Value& sprite_val) const;

private:
	void Trigger(const std::string& dir);

	void LoadTrimInfo(const std::string& dir);

private:
	struct TrimInfo
	{
		float x, y;
		float angle;
	};

private:
	std::string m_dir;

	std::map<std::string, TrimInfo> m_trim_info;

}; // FixRotateTrimJson 

}

#endif // _EASYDB_FIX_ROTATE_TRIM_JSON_H_