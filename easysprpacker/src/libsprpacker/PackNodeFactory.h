#ifndef _EASYSPRPACKER_PACK_NODE_FACTORY_H_
#define _EASYSPRPACKER_PACK_NODE_FACTORY_H_

#include <CU_Singleton.h>

#include <string>
#include <vector>

namespace ee { class Sprite; class Symbol; }

namespace esprpacker
{

class PackNode;
class NodeBuilder;

class PackNodeFactory
{
public:
	const PackNode* Create(const ee::Sprite* spr, bool force_curr = false);
	const PackNode* Create(const ee::Symbol* sym);

	void FetchAll(std::vector<PackNode*>& nodes) const;

	void SetFilesDir(const std::string& files_dir) {
		m_files_dir = files_dir;
	}

	void Clear();

private:
	static void FetchAllBuilder(std::vector<NodeBuilder*>& builders);

private:
	std::string m_files_dir;

	SINGLETON_DECLARATION(PackNodeFactory);

}; // PackNodeFactory

}

#endif // _EASYSPRPACKER_PACK_NODE_FACTORY_H_