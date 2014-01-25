#include "ScriptsSymbol.h"

namespace d2d
{
	void ScriptsSymbol::loadResources()
	{
		std::ifstream fin(m_filepath.fn_str());
		fin.seekg(0, std::ios::end);   
		m_data.reserve(fin.tellg());
		fin.seekg(0, std::ios::beg);
		m_data.assign((std::istreambuf_iterator<char>(fin)),
			std::istreambuf_iterator<char>());

		std::string str = m_data;
	}
}