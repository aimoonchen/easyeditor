#ifndef _EASYUI_CODE_H_
#define _EASYUI_CODE_H_

#include <drag2d.h>

namespace ebuilder { class CodeGenerator; }

namespace eui 
{

class Code
{
public:
	Code(ebuilder::CodeGenerator& gen);

	void ResolveUI(const std::vector<d2d::ISprite*>& sprites);
	void ResolveText(const std::vector<d2d::ISprite*>& sprites);

private:
	ebuilder::CodeGenerator& m_gen;

}; // Code

}

#endif // _EASYUI_CODE_H_