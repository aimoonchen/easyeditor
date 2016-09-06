#include "ExportNameSet.h"
#include "check.h"
#include "PackNode.h"

#include <ee/Symbol.h>
#include <ee/Exception.h>

namespace esprpacker
{

SINGLETON_DEFINITION(ExportNameSet);

ExportNameSet::ExportNameSet()
{
}

ExportNameSet::~ExportNameSet()
{
}

void ExportNameSet::Insert(const ee::Symbol* sym, PackNode* node)
{
	std::string export_name = sym->name;
	if (!is_name_valid(export_name)) {
		return;
	}

	std::map<std::string, int>::iterator itr = m_map.find(export_name);
	if (itr != m_map.end()) {
		throw ee::Exception("Duplicate export name %s, file %s", 
			export_name.c_str(), sym->GetFilepath().c_str());
	}

	node->SetName(export_name);
	m_map.insert(std::make_pair(export_name, node->GetID()));
}

}