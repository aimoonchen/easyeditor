#ifndef _EASYDB_LR_TO_COMPLEX_H_
#define _EASYDB_LR_TO_COMPLEX_H_

#include "ICommand.h"

#include <json/json.h>

namespace edb
{

class LRToComplex : public ICommand
{
public:
	LRToComplex() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new LRToComplex(); }

	void Run(const std::string& filepath);

private:
	void SeparateBottomLayer(const Json::Value& lr_val, const std::string& filepath);
	void SeparateTopLayer(const Json::Value& lr_val, const std::string& filepath);

	void LoadAllSpriteValue(const Json::Value& src_val, Json::Value& dst_val, bool bottom_layer);
	void LoadSpriteValue(const Json::Value& spr_val, Json::Value& dst_val);

	void OutputComplexFile(Json::Value& complex_val, const std::string& filepath,
		const std::string& tag);

}; // LRToComplex

}

#endif // _EASYDB_LR_TO_COMPLEX_H_