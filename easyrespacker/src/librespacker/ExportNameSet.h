#ifndef _EASYRESPACKER_EXPORT_NAME_SET_H_
#define _EASYRESPACKER_EXPORT_NAME_SET_H_

#include <ee/Symbol.h>

#include <map>

namespace erespacker
{

class IPackNode;

class ExportNameSet
{
public:
	
	void LoadExport(const ee::SymConstPtr& sym, IPackNode* node);

	const std::map<std::string, int>& GetData() const { return m_map; }

	void Clear() { m_map.clear(); }

private:
	std::map<std::string, int> m_map;

}; // ExportNameSet

}

#endif // _EASYRESPACKER_EXPORT_NAME_SET_H_