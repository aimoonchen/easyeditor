#ifndef _SG_FILE_IO_H_
#define _SG_FILE_IO_H_

#include <json/json.h>
#include <drag2d.h>

namespace sg
{

class StagePanel;

class FileIO
{
public:
	static void load(const char* filename, StagePanel* stage,
		d2d::LibraryPanel* library);
	static void store(const char* filename, StagePanel* stage);

private:
	static d2d::ISprite* load(const Json::Value& value, StagePanel* stage);
	static Json::Value store(const d2d::ISprite* sprite, StagePanel* stage);

	static void SetSymbolUserData(d2d::ISymbol* symbol);

}; // FileIO

}

#endif // _SG_FILE_IO_H_