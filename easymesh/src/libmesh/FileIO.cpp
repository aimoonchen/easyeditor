#include "FileIO.h"
#include "Symbol.h"
#include "Strip.h"
#include "Mesh.h"

namespace emesh
{

void FileIO::store(const char* filepath, const Symbol* symbol)
{
	Json::Value value;

	value["name"] = symbol->name;

	const Shape* shape = symbol->getShape();
	if (shape) {
		shape->Store(value);
	} else {
		return;
	}

	wxString dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	value["image"] = ee::FileHelper::GetRelativePath(dir, symbol->GetImagePath()).ToStdString();

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::load(const char* filepath, Symbol* symbol)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (!value["image"].isNull())
	{
		wxString dir = ee::FileHelper::GetFileDir(filepath);
		std::string path = ee::FileHelper::GetAbsolutePath(dir, value["image"].asString());
		// todo Release symbol
		//symbol = ee::SymbolMgr::Instance()->fetchSymbol(path);
		symbol->LoadImage(path);
// 		symbol->Release();
	}
	else
	{
		return;
	}

	std::string type = value["type"].asString();
	Shape* shape = NULL;
	if (type == Strip::GetType()) {
		shape = new Strip(*symbol->getImage());
	} else if (type == Mesh::GetType()) {
		shape = new Mesh(*symbol->getImage());		
	}
	if (shape)
	{
		shape->Load(value);
		symbol->SetShape(shape);
		shape->Release();
	}
}

}