// ���ļ������������޸�Anim��name�ֶ�

#ifndef _EASYDB_FIX_ANIMATION_H_
#define _EASYDB_FIX_ANIMATION_H_

#include <string>

namespace edb
{

class FixAnimation
{
public:
	FixAnimation(const std::string& dir);

private:
	void AddNameFromFile(const std::string& dir) const;

}; // FixAnimation

}

#endif // _EASYDB_FIX_ANIMATION_H_