#include "FileIO.h"
#include "Symbol.h"
#include "StripMesh.h"
#include "PointsMesh.h"
#include "SkeletonMesh.h"

#include <ee/FileHelper.h>
#include <ee/panel_msg.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>

#include <fstream>

namespace emesh
{

void FileIO::Store(const char* filepath, const Symbol& sym)
{
	Json::Value value;

	value["name"] = sym.name;

	auto mesh = static_cast<const Mesh*>(sym.GetMesh().get());
	if (mesh) {
		mesh->Store(value);
	} else {
		return;
	}

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	auto& base = sym.GetMesh()->GetBaseSymbol();
	value["base_symbol"] = ee::FileHelper::GetRelativePath(dir, 
		std::dynamic_pointer_cast<const ee::Symbol>(base)->GetFilepath());

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const char* filepath, Symbol& sym)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	ee::SymPtr base_symbol = nullptr;
	if (!value["base_symbol"].isNull())
	{
		std::string dir = ee::FileHelper::GetFileDir(filepath);
		std::string path = ee::FileHelper::GetAbsolutePath(dir, value["base_symbol"].asString());
		base_symbol = ee::SymbolMgr::Instance()->FetchSymbol(path);
	}
	else
	{
		return;
	}

	std::string type = value["type"].asString();
	std::unique_ptr<s2::Mesh> mesh = nullptr;
	if (type == "network" || type == "points") {
		mesh = std::make_unique<PointsMesh>(base_symbol);
	} else {
		throw ee::Exception("Unknown mesh type %s", filepath);
	}

// 	if (type == StripMesh::GetTypeName()) {
// 		mesh = new StripMesh(base_symbol);
// 	} else if (type == PointsMesh::GetTypeName()) {
// 		mesh = new PointsMesh(base_symbol);
// 	} else if (type == SkeletonMesh::GetTypeName()) {
// 		mesh = new SkeletonMesh(base_symbol);
// 	} else {
// 		throw ee::Exception("Unknown mesh type.");
// 	}
	if (mesh)
	{
		static_cast<Mesh*>(mesh.get())->Load(value);
		sym.SetMesh(mesh);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}