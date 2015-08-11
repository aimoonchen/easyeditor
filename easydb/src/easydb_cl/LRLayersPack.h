#ifndef _EASYDB_LR_LAYERS_PACK_H_
#define _EASYDB_LR_LAYERS_PACK_H_

#include "ICommand.h"

#include <drag2d.h>

namespace lr { class Grids; }

namespace edb
{

class LRLayersPack : public ICommand
{
public:
	LRLayersPack() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRLayersPack(); }

	void Run(const std::string& filepath);

private:
	void ParserShape(d2d::IShape* shape, const d2d::Vector& offset, const lr::Grids& grids, 
		bool force_grids, Json::Value& out_val);
	void ParserShapeLayer(const Json::Value& src_val, const lr::Grids& grids, bool force_grids,
		int layer_idx, const char* name, Json::Value& out_val);
	void ParserPoint(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val);
	void ParserCamera(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val);
	void ParserCharacter(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val);

	void ParserSpecial(const Json::Value& src_val, Json::Value& out_val);
	void ParserSpecialLayer(const Json::Value& spr_val, const std::string& name, Json::Value& out_val);
	void ParserParticleLayer(const Json::Value& spr_val, Json::Value& out_val);

private:
	std::string m_dir;

}; // LRLayersPack

}

#endif // _EASYDB_LR_LAYERS_PACK_H_