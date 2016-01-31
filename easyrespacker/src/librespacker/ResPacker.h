#ifndef _LIBRESPACKER_RES_PACKER_H_
#define _LIBRESPACKER_RES_PACKER_H_



#include "typedef.h"
//#include "PackPkgMgr.h"

namespace librespacker
{

class ResPacker
{
public:
	ResPacker(const std::string& json_dir, const std::string& tp_name, 
		const std::string& tp_dir);
// 	ResPacker(const std::string& json_dir, const std::string& tp_name, 
// 		const std::string& tp_dir, const std::string& pkgs_cfg_file);
	
	void OutputLua(const std::string& outfile, float scale = 1) const;

	void OutputEpe(const std::string& outfile, bool compress, float scale = 1) const;
	void OutputEpt(const std::string& outfile, TextureType type, int LOD = 0, float scale = 1) const;

	void OutputUIExtra(const std::string& outfile) const;

	void OutputSprID(const std::string& outfile) const;

	static void OutputEptDesc(const std::string& outfile, const std::string& tp_name);

private:
	void LoadJsonData(const std::string& dir);
	void LoadTPData(const std::string& tp_name);

	void Pack() const;

private:
	std::vector<const ee::Symbol*> m_symbols;

	ee::TexturePacker m_tp;

//	PackPkgMgr m_pkgs;

}; // ResPacker

}

#endif // _LIBRESPACKER_RES_PACKER_H_