#include "SymbolSet.h"

#include <ee/FileHelper.h>
#include <ee/Symbol.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

namespace ecoco
{

void SymbolSet::Insert(const ee::SymConstPtr& sym)
{
	std::string filepath = ee::FileHelper::FormatFilepath(sym->GetFilepath());

	if (filepath.find("img_guide_scale9") != std::string::npos) 
	{
		int zz  =0;
	}

	if (!Query(sym) || ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_SCALE9) {
		m_symbol_map.insert(std::make_pair(filepath, sym));
		m_symbol_ordered.push_back(sym);
	}
}

ee::SymConstPtr SymbolSet::Query(const std::string& filepath) const
{
	auto itr_begin = m_symbol_map.lower_bound(filepath);
	auto itr_end = m_symbol_map.upper_bound(filepath);
	if (itr_begin != itr_end) {
		return itr_begin->second;
	} else {
		return NULL;
	}
}

bool SymbolSet::Query(const ee::SymConstPtr& sym) const
{
	std::string filepath = ee::FileHelper::FormatFilepath(sym->GetFilepath());
	auto itr_begin = m_symbol_map.lower_bound(filepath);
	auto itr_end = m_symbol_map.upper_bound(filepath);
	auto itr = itr_begin;
	for ( ; itr != itr_end; ++itr) {
		if (itr->second == sym) {
			return true;
		}
	}

	return false;
}

}