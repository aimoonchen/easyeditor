#include "PackToBin.h"
#include "PackNode.h"
#include "PackNodeFactory.h"
#include "ExportNameSet.h"
#include "binary_io.h"
#include "PackIDMgr.h"
#include "Version.h"

#include <ee/StringHelper.h>

#include <easyrespacker.h>

#include <bimp/typedef.h>
#include <simp/simp_define.h>
#include <simp/Page.h>

#include <simp/NodeFactory.h>

#include <map>
#include <algorithm>

//#define DEBUG_PACK_BIN

namespace esprpacker
{

class PackNodeCmp
{
public:
	bool operator () (const PackNode* lhs, const PackNode* rhs) const {
		return lhs->GetID() < rhs->GetID();
	}
}; // PackNodeCmp

void PackToBin::Pack(const std::string& filepath, 
					 const ee::TexturePacker& tp, 
					 bool compress, 
					 float scale)
{
	std::set<int> ref_pkgs;

	// src nodes
	std::vector<PackNode*> nodes;
	PackNodeFactory::Instance()->FetchAll(nodes);
	std::vector<PackNode*>::iterator itr = nodes.begin();
	for ( ; itr != nodes.end(); ) 
	{
		PackNode* node = *itr;
		if (!PackIDMgr::Instance()->IsCurrPkg(node)) {
			ref_pkgs.insert(node->GetPkgID());
			itr = nodes.erase(itr);
		} else {
			++itr;
		}
	}
	if (nodes.empty()) {
		return;
	}
	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());

	// to pages
	std::vector<Page*> pages;
	Page* page = new Page(simp::SIMP_PAGE_SIZE);
	int page_sz = ALIGN_4BYTE(simp::Page::Size());
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		PackNode* node = nodes[i];
		int sz = node->SizeOfUnpackFromBin();
		while (true)
		{
			// head sz
			int num;
			auto& nodes = page->GetNodes();
			if (nodes.empty()) {
				num = 1;
			} else {
				num = node->GetID() - nodes[0]->GetID() + 1;
			}
			int align_n = ALIGN_4BYTE(num);
			int head_sz =
				sizeof(uint8_t) * align_n + // types
				simp::SIZEOF_POINTER * n;   // nodes

			if (page_sz + head_sz + sz <= page->GetSize()) {
				page_sz += sz;
				page->Add(node);
				break;
			}
			if (page->NodeNum() > 0) 
			{
				page->Condense(page_sz + head_sz);
				pages.push_back(page);
				page = new Page(simp::SIMP_PAGE_SIZE);
				page_sz = ALIGN_4BYTE(simp::Page::Size());
			} else {
				page->Enlarge();
			}
		}
	}
	if (page->NodeNum() > 0) 
	{
		int head_sz = sizeof(uint8_t) * ALIGN_4BYTE(page->NodeNum() + 1) 
			        + simp::SIZEOF_POINTER * (page->NodeNum() + 1);
		page->Condense(page_sz + head_sz);
		pages.push_back(page);
	}

	// pack index
	PageIndex(filepath + ".epe", pages, compress, scale, ref_pkgs);

	// pack pages
	for (int i = 0, n = pages.size(); i < n; ++i) {
		std::string path = filepath + "." + ee::StringHelper::ToString(i + 1) + ".epe";
		PackPage(path, *pages[i], tp, compress);
	}
}

void PackToBin::PageIndex(const std::string& filepath, const std::vector<Page*>& pages, 
						  bool compress, float scale, const std::set<int>& ref_pkgs)
{
	const std::map<std::string, int>& exports = ExportNameSet::Instance()->GetData();

	/************************************************************************/
	/* size                                                                 */
	/************************************************************************/

	int sz = 0;

	// version
	sz += sizeof(uint16_t);					// version flag
	sz += sizeof(uint16_t);					// version

	// export
	sz += sizeof(uint16_t);					// num
	std::map<std::string, int>::const_iterator itr = exports.begin();
	for ( ; itr != exports.end(); ++itr) {
		sz += sizeof_pack_str(itr->first);	// name
		sz += sizeof(uint32_t);				// id
	}

	// pages
	sz += sizeof(uint16_t);					// num
	for (int i = 0, n = pages.size(); i < n; ++i) {
		sz += sizeof(uint32_t);				// size
		sz += sizeof(uint32_t);				// min
		sz += sizeof(uint32_t);				// max
	}

	// scale
	sz += sizeof(float);

	// ref others
	sz += sizeof(uint16_t);
	sz += sizeof(uint16_t) * ref_pkgs.size();

	/************************************************************************/
	/* fill                                                                 */
	/************************************************************************/

	uint8_t* buf = new uint8_t[sz];
	uint8_t* ptr = buf;

	// version
	uint16_t version_flag = 0xffff;
	pack(version_flag, &ptr);
	uint16_t version = VERSION;
	pack(version, &ptr);

	// export
	uint16_t export_n = exports.size();
	pack(export_n, &ptr);					// num
	for (itr = exports.begin(); itr != exports.end(); ++itr) {
		pack_str(itr->first, &ptr);			// name
		uint32_t id = itr->second;
		pack(id, &ptr);						// id
	}

	// pages
	uint16_t page_n = pages.size();
	pack(page_n, &ptr);
	for (int i = 0, n = pages.size(); i < n; ++i) 
	{
		uint32_t size = pages[i]->GetSize();
		pack(size, &ptr);

		uint32_t min = pages[i]->GetMinID(),
			     max = pages[i]->GetMaxID();
		pack(min, &ptr);
		pack(max, &ptr);
	}

	// scale
	pack(scale, &ptr);

	// ref others
	uint16_t ref_count = ref_pkgs.size();
	pack(ref_count, &ptr);
	std::set<int>::const_iterator itr_ref = ref_pkgs.begin();
	for ( ; itr_ref != ref_pkgs.end(); ++itr_ref) {
		uint16_t pkg = *itr_ref;
		pack(pkg, &ptr);
	}

	/************************************************************************/
	/* output                                                               */
	/************************************************************************/
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
	if (compress) {
		uint8_t* dst = NULL;
		size_t dst_sz;
		erespacker::Lzma::Compress(&dst, &dst_sz, buf, sz);
		if (dst_sz > 0) {
			fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(dst_sz));
			fout.write(reinterpret_cast<const char*>(dst), dst_sz);
		}
	} else {
		int32_t _sz = -(int)sz;
		fout.write(reinterpret_cast<const char*>(&_sz), sizeof(sz));
		fout.write(reinterpret_cast<const char*>(buf), sz);
	}
	delete[] buf;
	fout.close();
}

void PackToBin::PackPage(const std::string& filepath, const Page& page, 
						 const ee::TexturePacker& tp, bool compress)
{
#ifdef DEBUG_PACK_BIN
	std::vector<int> list0, list1;
	int tot_sz = 0;
#endif // DEBUG_PACK_BIN

	int out_sz = 0;
	const std::vector<PackNode*>& nodes = page.GetNodes();
	for (int i = 0, n = nodes.size(); i < n; ++i) {
#ifdef DEBUG_PACK_BIN
		tot_sz += page.GetNodes()[i]->SizeOfPackToBin();
		list0.push_back(tot_sz);
#endif // DEBUG_PACK_BIN
		out_sz += nodes[i]->SizeOfPackToBin();
	}
	
	uint8_t* buf = new uint8_t[out_sz];
	uint8_t* ptr = buf;
	for (int i = 0, n = nodes.size(); i < n; ++i) {
		nodes[i]->PackToBin(&ptr, tp);
#ifdef DEBUG_PACK_BIN
		list1.push_back(ptr - buf);
#endif // DEBUG_PACK_BIN
	}

	assert(ptr - buf == out_sz);

	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
	if (compress) {
		uint8_t* dst = NULL;
		size_t dst_sz;
		erespacker::Lzma::Compress(&dst, &dst_sz, buf, out_sz);
		if (dst_sz > 0) {
			fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(dst_sz));
			fout.write(reinterpret_cast<const char*>(dst), dst_sz);
		}
	} else {
		int32_t sz = -(int)out_sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(out_sz));
		fout.write(reinterpret_cast<const char*>(buf), out_sz);
	}
	delete[] buf;
	fout.close();
}

/************************************************************************/
/* class PackToBin::Page                                                */
/************************************************************************/

PackToBin::Page::
Page(int size) 
	: m_size(size)
	, m_id_min(INT_MAX)
	, m_id_max(-INT_MAX) 
{}

PackToBin::Page::
~Page() 
{
	for_each(m_nodes.begin(), m_nodes.end(), cu::RemoveRefFunctor<PackNode>());
}

void PackToBin::Page::
Add(PackNode* node)
{
	int id = node->GetNodeID();
	if (id < m_id_min) {
		m_id_min = id;
	}
	if (id > m_id_max) {
		m_id_max = id;
	}
	m_nodes.push_back(node);
}

void PackToBin::Page::
Enlarge() 
{
	m_size *= 2;
	if (m_size > simp::SIMP_PAGE_SIZE_MAX) {
		throw ee::Exception("Page::Enlarge page size too large %d", m_size);
	}
}

void PackToBin::Page::
Condense(int size)
{
	while (size < m_size / 2 && m_size > simp::SIMP_PAGE_SIZE_MIN) {
		m_size /= 2;
	}
}

}