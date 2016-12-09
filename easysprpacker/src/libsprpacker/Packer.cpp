#include "Packer.h"
#include "PackNodeFactory.h"
#include "PackToLua.h"
#include "PackToBin.h"
#include "PackNode.h"
#include "PackIDMgr.h"

#include <ee/TextureFactory.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>
#include <ee/Symbol.h>
#include <ee/SymbolFile.h>

#include <easyrespacker.h>

#include <CU_RefCountObj.h>
#include <dtex.h>
#include <gimg_typedef.h>
#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>

#include <wx/arrstr.h>
#include <wx/filename.h>

#include <algorithm>

namespace esprpacker
{

Packer::Packer(const std::string& json_dir, const std::string& tp_name, 
			   const std::string& tp_dir)
	: m_tp(tp_dir)
{
	ee::TextureFactory::Instance()->InitTexturePacker(tp_dir);

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool old_cfg = data.load_image;
	data.load_image = false;

	PackNodeFactory::Instance()->SetFilesDir(json_dir);

	LoadTPData(tp_name);
	LoadJsonData(json_dir);

	Pack();

	data.load_image = old_cfg;
}

Packer::~Packer()
{
	for_each(m_syms.begin(), m_syms.end(), cu::RemoveRefFunctor<const ee::Symbol>());
//	PackNodeFactory::Instance()->Release();
}

void Packer::OutputLua(const std::string& outfile, float scale) const
{
	PackToLua::Pack(outfile, m_tp, scale);
}

void Packer::OutputEpe(const std::string& outfile, bool compress, float scale) const
{
	PackToBin::Pack(outfile, m_tp, compress, scale);
}

void Packer::OutputEpt(const std::string& outfile, int LOD, float scale) const
{
	OutputEptDesc(outfile);
	erespacker::PackToBin::PackEPT(outfile, m_tp, LOD, scale);
}

void Packer::OutputSprID(const std::string& pkg_name, const std::string& res_dir) const
{
	std::vector<PackNode*> nodes;
	PackNodeFactory::Instance()->FetchAll(nodes);
	if (nodes.empty()) {
		return;
	}

	PackIDMgr* id_mgr = PackIDMgr::Instance();

	Json::Value value;
	for (int i = 0, n = nodes.size(); i < n; ++i) 
	{
		PackNode* node = nodes[i];
		if (!id_mgr->IsCurrPkg(node->GetFilepath())) {
			continue;
		}

		std::string filepath = gum::FilepathHelper::Relative(res_dir, node->GetFilepath());
		if (id_mgr->IsCurrImgCut()) 
		{
			std::string img, json, ori;
			id_mgr->GetCurrImgCutPath(img, json, ori);
			if (filepath.compare(0, img.size(), img) == 0) {
				continue;
			} else if (filepath.compare(0, json.size(), json) == 0) {
				filepath = filepath.substr(json.size() + 1);
				ee::StringHelper::ReplaceAll(filepath, "%", "\\");
				ee::StringHelper::ReplaceAll(filepath, "_complex.json", ".png");
			} else if (filepath.compare(0, ori.size(), ori) == 0) {
				filepath = filepath.substr(ori.size() + 1);
			} else {
				throw ee::Exception("Packer::OutputSprID: err file %s", filepath.c_str());
			}
		} 

		Json::Value item;
		item["file"] = filepath;
		item["id"] = node->GetID();
		value[value.size()] = item;
	}

	std::string filepath = PackIDMgr::Instance()->GetSprIDFile(pkg_name);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();	
}

void Packer::LoadJsonData(const std::string& dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	std::vector<std::string> filepaths;
	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		int type = ee::SymbolFile::Instance()->Type(filepath);
		if (type == s2::SYM_COMPLEX || 
			type == s2::SYM_ANIMATION ||
			type == s2::SYM_PARTICLE3D ||
			type == s2::SYM_TRAIL) {
			filepaths.push_back(filepath);
		}
	}

	std::sort(filepaths.begin(), filepaths.end());
	for (int i = 0, n = filepaths.size(); i < n; ++i) {
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepaths[i]);
		m_syms.push_back(sym);
	}
}

void Packer::LoadTPData(const std::string& tp_name)
{
	int i = 1;
	while (true) {
		std::string tp_path = tp_name + ee::StringHelper::ToString(i) + ".json";
		if (ee::FileHelper::IsFileExist(tp_path)) {
			m_tp.Add(tp_path);
			ee::TextureFactory::Instance()->AddTextureFromConfig(tp_path);
		} else {
			break;
		}
		++i;
	}
}

void Packer::Pack() const
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	for (int i = 0, n = m_syms.size(); i < n; ++i) {
		factory->Create(m_syms[i]);
	}
}

void Packer::OutputEptDesc(const std::string& outfile) const
{
	std::string filepath = outfile + ".ept";
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	

	const std::vector<const ee::TexturePacker::Texture*>& textures = m_tp.GetTextures();

	int count = textures.size();

	int out_sz = 0;
	out_sz += sizeof(int);						// count
	out_sz += count * sizeof(uint16_t) * 2;		// size
	out_sz += count * sizeof(uint16_t);			// type

	// file header
	out_sz = -out_sz;
	fout.write(reinterpret_cast<const char*>(&out_sz), sizeof(out_sz));

	// count
	fout.write(reinterpret_cast<const char*>(&count), sizeof(count));

	// size & type
	for (int i = 0; i < count; ++i) 
	{
		const ee::TexturePacker::Texture* tex = textures[i];

		// size
		uint16_t w = tex->width,
			h = tex->height;
		fout.write(reinterpret_cast<const char*>(&w), sizeof(w));
		fout.write(reinterpret_cast<const char*>(&h), sizeof(h));

		// type
		uint16_t type = GIT_PNG;
		if (tex->format == "pvr") {
			type = GIT_PVR;
		} else if (tex->format == "etc1") {
			type = GIT_ETC1;
		} else if (tex->format == "etc2") {
			type = GIT_ETC2;
		}
		fout.write(reinterpret_cast<const char*>(&type), sizeof(type));
	}

	fout.close();
}

}