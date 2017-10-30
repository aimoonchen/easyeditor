#include "SymbolLoader.h"
#include "SymbolMgr.h"
#include "Symbol.h"
#include "SymbolFactory.h"

namespace ee
{

s2::SymPtr SymbolLoader::Create(const std::string& filepath, int type) const
{
	return SymbolMgr::Instance()->FetchSymbol(filepath, type);
}

s2::SymPtr SymbolLoader::Create(int type) const
{
	return SymbolFactory::Create(type);
}

}